#ifndef WORD_H_
#define WORD_H_
#include <string>
using namespace std;
class word {
private:
	string* wordValue;
	int occurrence;

public:
	int getOccurrence() {
		return occurrence;
	}

	string* getWordValue() {
		return wordValue;
	}

	void setOccurrence(int occurrence) {
		this->occurrence = occurrence;
	}
	void increaseOccurence() {
		this->occurrence++;
	}

	void setWordValue(string* wordValue) {
		this->wordValue = wordValue;
	}
	//Types of Constructor
	word(string* wordValue) {
		setWordValue(wordValue);
		setOccurrence(1);
	}
	word() {

	}
	word(string* wordValue, int occurence) {
		setWordValue(wordValue);
		setOccurrence(occurence);
	}
	virtual ~word() {
		delete wordValue;
	}
};
#endif /* WORD_H_ */
