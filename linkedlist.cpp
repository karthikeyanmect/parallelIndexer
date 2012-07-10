#include "linkedlist.h"
#include <iostream>
using namespace std;

linkedlist::linkedlist() {
	size = 0;
}
linkedlist::~linkedlist() {
	for (int i = 0; i < size; i++) {
		node* temp = currentNode;
		currentNode = currentNode->next;
		if (temp != NULL)
			delete temp;
	}
}
void linkedlist::addNode(word* sym) {
	node *temp;
	temp = new node;
	temp->s = sym;
	if (this->size == 0) {
		this->lastNode = temp;
		this->firstNode = temp;
	} else {
		(this->lastNode)->next = temp;
		this->lastNode = temp;
	}
	size++;
}
word* linkedlist::topNode() {
	return ((this->firstNode)->s);
}
word* linkedlist::endNode() {
	return ((this->lastNode)->s);
}

int linkedlist::getSize() const {
	return size;
}

void linkedlist::setSize(int size) {
	this->size = size;
}

bool linkedlist::findWord(word* sym) {
	node *ptr;
	ptr = firstNode;
	string temp = *(sym->getWordValue());
	for (int i = 0; i < size; i++) {

		if (*(ptr->s->getWordValue()) == temp) {
			return true;
		}
		ptr = ptr->next;

	}

	return false;
}
word* linkedlist::getWord(string value) {
	node *ptr = NULL;
	ptr = firstNode;
	for (int i = 0; i < size; i++) {
		if (*(ptr->s->getWordValue()) == value)
			return ptr->s;
		ptr = ptr->next;
	}
	return NULL;
}
void linkedlist::display() {
	node *ptr;
	ptr = firstNode;
	for (int i = 0; i < size; i++) {
		std::cout << "           " << *((ptr->s)->getWordValue()) << "-----"
				<< (ptr->s->getOccurrence()) << std::endl;
		ptr = ptr->next;
	}
}
void linkedlist::stratIterate() {
	currentNode = firstNode;
}
word* linkedlist::iterate() {
	word* ret = NULL;
	if (size > 0)
		if (currentNode->next != NULL) {
			ret = currentNode->s;
			currentNode = currentNode->next;
		} else {
			currentNode = NULL;
		}

	return ret;
}
