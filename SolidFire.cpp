#define WORKER 3 // Number of Worker Threads
#define MASTERID 0 // By Default Manager/Mater thread's ID is ZERO
#include <fstream>
#include <sstream>
#include<iostream>
#include<dirent.h>
#include<string.h>
#include <cstdlib>
#include "threadArgument.h"
#include "InputQueue.h"
#include "wordMgmt.h"
#include "word.h"
#include "TopKWords.h"
using namespace std;

pthread_t workerThread[4];
pthread_mutex_t countMutex;                 //Mutex for Termination Count variable
pthread_cond_t countThresholdCondtion;      //Condition Variable for Termiantion Count Variable
InputQueue** iQueue = new InputQueue *[4];  //Input Queue one for each worker
wordMgmt* hashTable = new wordMgmt();       //Common Table for saving words (Atomic hash Table)
TopKWords* topList = new TopKWords();       //A Muliset based list of top K words
int globalTermination = 0;                 // a global variable for declaring termination
bool readingDoneByMaster = false;          // variable to indicate the end of reading file list by master thread 
bool terminateProgram = false;          

//Fucntion to check termination Condition
bool isTerminate(unsigned int threadId) {
    return __sync_bool_compare_and_swap(&terminateProgram, false, false);
}
//Fucntion to declare termination
void setTerminate(unsigned int threadId) {
    pthread_mutex_lock(&countMutex);
    globalTermination++;
    if (globalTermination == WORKER) {
        pthread_cond_signal(&countThresholdCondtion);
    }
    pthread_mutex_unlock(&countMutex);
}
//Function to read a file and add words to hashTable
//gets ThreadId and complete path as input
int readFile(string pathOfFile, unsigned int threadId) {
    std::ifstream file(pathOfFile.c_str());
    if (file) {
        string line, singleWord;
        while (getline(file, line)) {
            std::istringstream is(line);
            while (is >> singleWord) {
                string* wordToTake = new string(singleWord);
                word* newWord = new word(wordToTake);
                hashTable->insertWord(newWord, threadId);
            }
        }
    }
    file.close();
    file.clear();
    return 1;
}
//called by each thread at time of creation
void *threadFunction(void* threadArg) {
    unsigned int threadId = ((threadArgument*) threadArg)->getMyId();

    while (isTerminate(threadId)) {
        string* fileName = iQueue[threadId]->dequeueFile(threadId);
        if (fileName != NULL) {
            readFile(*fileName, threadId);
        } else {
            if (readingDoneByMaster) {
                setTerminate(threadId);
            }
        }
    }
    pthread_exit(NULL);
}
//to create thread
void spawnWorkers() {
    int retVal;
    for (int count = 1; count <= WORKER; count++) {
        threadArgument* arg = new threadArgument(count);
        retVal = pthread_create(&workerThread[count], NULL, threadFunction,
                (void *) (arg));
    }
}

//called recursively by manager thread to find all txt files in a  directory
//it adds those files to workers input queue
int readDirectory(char* direc) {
    static int nextQueue = 1;
    string direcString(direc);
    DIR *dir;
    struct dirent *ent;
    int len;
    //Finds all directories except . and .. and calls the same function
    dir = opendir(direc);
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0
                    && strcmp(ent->d_name, "..") != 0) {
                string newStr = direcString + "/" + ent->d_name;
                char *nextDirec = new char[newStr.size() + 1];
                newStr[newStr.size()] = 0;
                memcpy(nextDirec, newStr.c_str(), newStr.size());
                readDirectory(nextDirec);
            }
        }
    }
    //Finds all txt files and add those to the workers inputqueue
    dir = opendir(direc);
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            len = strlen(ent->d_name);
            if (len >= 4) {
                if (strcmp(".txt", &(ent->d_name[len - 4])) == 0) {
                    string *fileName = new string(direcString);
                    *fileName = *fileName + "/" + ent->d_name;
                    iQueue[(nextQueue++)]->enqueueFile(fileName, MASTERID);
                    if (nextQueue == 4)
                        nextQueue = 1;
                }
            }
        }
        closedir(dir);
    }
    return 1;

}
void setTerminationCond() {
    if (globalTermination >= WORKER) {
        terminateProgram = 1;
       //Termination Condition Reached
    } else {
        globalTermination = 0;
    }
}

int main(int argc, char* argv[]) {
    //initializing
    pthread_mutex_init(&countMutex, NULL);
    pthread_cond_init(&countThresholdCondtion, NULL);

    for (int i = 1; i <= WORKER; i++) {
        iQueue[i] = new InputQueue();
    }

    //create Threads    
    spawnWorkers();
    //Recusively read filenames and add it to Workers' Input Queue
    readDirectory(argv[1]);
    readingDoneByMaster = true;
    //wait for the worker to update the termination count    
    pthread_mutex_lock(&countMutex);
    while (globalTermination < WORKER) {
        pthread_cond_wait(&countThresholdCondtion, &countMutex);
        setTerminationCond();
    }
    pthread_mutex_unlock(&countMutex);
    // Displays the bucket hased Table    
    hashTable->displayTable();

    //updates the content of hashTable to a Multiset to get top K elements    
    word* fetch;
    for (int i = 0; i < hashTable->getBucketCount(); i++) {
        hashTable->startIterateTable(i);
        fetch = hashTable->iterateTable(i);
        if (fetch != NULL) {
            topList->addWord(fetch);
        } else
            continue;
    }
    //printing k elements    
    cout<<"******************** High Frequency Words *****************"<<endl;
    topList->printTopKWords(10);
    return 0;
}
