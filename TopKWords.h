#ifndef TOPKWORDS_H_
#define TOPKWORDS_H_
#include "word.h"
#include <set>

class sortBefore: public std::binary_function<word *, word *, bool> {
public:
	bool operator()(word *a, word *b) {
		return a->getOccurrence() > b->getOccurrence();

	}
};
class TopKWords {
private:
	multiset<word*, sortBefore> *topKMultiset;
public:
	TopKWords() {
		topKMultiset = new multiset<word*, sortBefore> ;
	}
	~TopKWords() {

	}
	void addWord(word* wordToAdd) {
		topKMultiset->insert(wordToAdd);
	}
	void printTopKWords(int k) {
		word* ret;
		for (int i = 0; i < k; i++) {
			ret = *(topKMultiset->begin());
			if (ret != NULL) {
				cout << *(ret->getWordValue()) << " - " << ret->getOccurrence()
						<< endl;
				topKMultiset->erase(topKMultiset->begin());
			}
		}
	}

};

#endif /* TOPKWORDS_H_ */
