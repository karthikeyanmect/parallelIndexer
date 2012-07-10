#ifndef INPUTQUEUE_H_
#define INPUTQUEUE_H_

#include<iostream>
#include<queue>
#include"AtomicState.h"
#include<string>
using namespace std;


class InputQueue {
private:
	queue<string*> *input;
	AtomicState* lock;
	pthread_mutex_t inputMutex;

public:
	InputQueue();
	virtual ~InputQueue();
	void enqueueFile(string* filename, unsigned int threadId);
	string* dequeueFile(unsigned int threadId);
	void emptyQueue();
	void acquireLock(unsigned int threadId);
	void releaseLock(unsigned int threadId);
	int getSize(unsigned int threadId);

};

#endif /* INPUTQUEUE_H_ */
