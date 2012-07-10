#include "wordMgmt.h"
#include <iostream>
using namespace std;

wordMgmt::wordMgmt() {
	bucketLock = new AtomicState *[BUCKET_COUNT];
	for (int i = 0; i < BUCKET_COUNT; i++) {
		this->buckets[i] = new linkedlist();
		bucketLock[i] = new AtomicState();
	}
}
wordMgmt::~wordMgmt() {
	delete bucketLock;
	delete[] buckets;
}

int wordMgmt::getBucketNo(string str) {
	int hash = 0;
	unsigned int i = 0;
	while (i < str.length()) {
		hash = hash + int(str[i]);
		i++;
	}
	return hash % BUCKET_COUNT;
}
void wordMgmt::insertWord(word* s, unsigned int threadId) {
	string* value = s->getWordValue();
	int index = getBucketNo(*value);
	getBucketLock(index, threadId);
	if (!getWordAndIncrementOccurence(s))
		(this->buckets[index])->addNode(s);
	releaseBucketLock(index, threadId);
}
bool wordMgmt::searchWord(word sym, unsigned int threadId) {
	bool ret;
	int index = getBucketNo(*(sym.getWordValue()));
	getBucketLock(index, threadId);
	ret = ((this->buckets[index])->findWord(&sym));
	releaseBucketLock(index, threadId);
	return ret;
}
//getWord without Lock
bool wordMgmt::getWordAndIncrementOccurence(word* sym) {
	word* wordNode;
	int index = getBucketNo(*(sym->getWordValue()));
	wordNode = ((this->buckets[index])->getWord(*(sym->getWordValue())));
	if (wordNode != NULL) {
		wordNode->increaseOccurence();
		return true;
	} else
		return false;

}
word* wordMgmt::getWord(string value, unsigned int threadId) {
	word* ret;
	int index = getBucketNo(value);
	getBucketLock(index, threadId);
	ret = ((this->buckets[index])->getWord(value));
	releaseBucketLock(index, threadId);
	return ret;
}

//Not Thread Safe
void wordMgmt::displayTable() {
	for (int i = 0; i < BUCKET_COUNT; i++) {
		cout << "BUCKET " << i << endl;
		this->buckets[i]->display();
	}
}
//Not Thread Safe
void wordMgmt::startIterateTable(int bucketNumber) {
	this->buckets[bucketNumber]->stratIterate();
}
//Not Thread Safe
word* wordMgmt::iterateTable(int bucketNumber) {
	return this->buckets[bucketNumber]->iterate();

}
void wordMgmt::getBucketLock(int bucketId, unsigned int threadId) {
	while (!bucketLock[bucketId]->setLock(threadId))
	;
}
int wordMgmt::getBucketCount() {
	return BUCKET_COUNT;
}

void wordMgmt::releaseBucketLock(int bucketId, unsigned int threadId) {
	bucketLock[bucketId]->releaseLock(threadId);
}
