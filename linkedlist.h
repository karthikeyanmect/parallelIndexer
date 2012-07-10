#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "word.h"
#include<string>
using namespace std;

struct node {
	word* s;
	node *next;
};

class linkedlist {
private:
	node *lastNode;
	node *firstNode;
	node* currentNode; //for iterator
	int size;
public:
	// Constructor Creating New Node
	linkedlist();
	~linkedlist();
	void addNode(word* sym);
	word* topNode();
	word* endNode();
	int getSize() const;
	void setSize(int size);
	bool findWord(word* sym);
	word* getWord(string value);
	word* getWordPtr(string value);
	void display();
	word* iterate();
	void stratIterate();
};

#endif /* LINKEDLIST_H_ */
