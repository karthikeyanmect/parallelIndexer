#include "InputQueue.h"
#include <assert.h>

//Each worker has one input queue protected by a mutex 
InputQueue::~InputQueue() {
	delete input;
	pthread_mutex_destroy(&inputMutex);
}
InputQueue::InputQueue() {
	input = new queue<string*> ;
	pthread_mutex_init(&inputMutex, NULL);
}

void InputQueue::enqueueFile(string* filename, unsigned int threadId) {
	acquireLock(threadId);
	input->push(filename);
	releaseLock(threadId);
}

string* InputQueue::dequeueFile(unsigned int threadId) {
	string* ret = NULL;
	acquireLock(threadId);
	if (input->size() > 0) {
		ret = input->front();
		input->pop();
	}
	releaseLock(threadId);
	return ret;
}
void InputQueue::acquireLock(unsigned int threadId) {
	pthread_mutex_lock(&inputMutex);
}
void InputQueue::releaseLock(unsigned int threadId) {
	pthread_mutex_unlock(&inputMutex);
}

int InputQueue::getSize(unsigned int threadId) {
	int ret;
	acquireLock(threadId);
	ret = input->size();
	releaseLock(threadId);
	return ret;
}
