//This has table is based on chaining implemented using linked list as buckets
//each linkedlist is protected by an atmoic lock
//so multiple threads can update the talbe at same time
#ifndef WORDMGMT_H_
#define WORDMGMT_H_
#include "AtomicState.h"
#include <string>
#include "word.h"
#include "linkedlist.h"

#define BUCKET_COUNT 100

using namespace std;
class wordMgmt {
public:
	wordMgmt();
	virtual ~wordMgmt();
	int getBucketNo(string str);
	void insertWord(word* s, unsigned int threadId);
	bool searchWord(word sym, unsigned int threadId);
	bool getWordAndIncrementOccurence(word* sym);
	word* getWord(string value, unsigned int threadId);
	//this is not thread safe
	void displayTable();

	void startIterateTable(int bucketNumber);
	word* iterateTable(int bucketNumber);

	void getBucketLock(int bucketId, unsigned int threadId);
	void releaseBucketLock(int bucketId, unsigned int threadId);

	int getBucketCount();
private:
	word s;
	linkedlist *buckets[BUCKET_COUNT];
	AtomicState** bucketLock;
};

#endif /* WORDMGMT_H_ */
