#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "book.h"
using namespace std;

// Base Constructors
Book::Book() : code(0), title(""), available(0), rented(0) {}
Book::Book(int code, string title, int available, int rented) {
    this->code = code;
    this->title = title;
    this->available = available;
    this->rented = rented;
}

// Base Setters
void Book::setCode(int code) { this->code = code; }
void Book::setTitle(string title) { this->title = title; }
void Book::setAvailable(int available) { this->available = available; }
void Book::setRented(int rented) { this->rented = rented; }

// Base Getters
int Book::getCode() const { return code; }
string Book::getTitle() const { return title; }

string Book::getTitle(bool withUnderscores) const {
    string tempTitle = title;
    if (!withUnderscores) {
		// Replace underscores with spaces
        for (int i = 0; i < tempTitle.length(); i++) {
            if (tempTitle[i] == '_')
                tempTitle[i] = ' ';
        }
    }
    return tempTitle;
}

int Book::getAvailable() const { return available; }
int Book::getRented() const { return rented; }

int Book::getSection() const {
	try {
		if (code > 1000 && code <= 2000) // If children's book code
			return 0;
		else if (code > 2000 && code <= 3000) // If computer book code
			return 1;
		else if (code > 3000 && code <= 4000) // If novel
			return 2;
		else throw code;
	}
	catch (int errCode) {
		cout << "Error : " << errCode << " is an invalid book code." << endl;
		return -1;
	}
}

// Children's Book Constructors
ChildrenBook::ChildrenBook() : age(0) {}
ChildrenBook::ChildrenBook(int code, string title, int available, int rented, int age) : Book(code, title, available, rented) 
{
    this->age = age;
}

// Children's Book Setters
void ChildrenBook::setAge(int age) { this->age = age; }

// Children's Book Getters
int ChildrenBook::getAge() const { return age; }
string ChildrenBook::getPublisher() const { return ""; }
int ChildrenBook::getPublish_Date() const { return 0; }

// Computer Book Constructors
ComputerBook::ComputerBook() : publisher("") {}
ComputerBook::ComputerBook(int code, string title, int available, int rented, string publisher) : Book(code, title, available, rented) {
    this->publisher = publisher;
}

// Computer Book Setters
void ComputerBook::setPublisher(string publisher) { this->publisher = publisher; }

// Computer Book Getters
string ComputerBook::getPublisher() const { return publisher; }
int ComputerBook::getAge() const { return 0; }
int ComputerBook::getPublish_Date() const { return 0; }

// Novel Constructors
Novel::Novel() : publish_date(0) {}
Novel::Novel(int code, string title, int available, int rented, int publish_date) : Book(code, title, available, rented) {
    this->publish_date = publish_date;
}

// Novel Setters
void Novel::setPublish_Date(int publish_date) { this->publish_date = publish_date; }

// Novel Getters
int Novel::getPublish_Date() const { return publish_date; }
int Novel::getAge() const { return 0; }
string Novel::getPublisher() const { return ""; }

// Library Constructors
Library::Library() : section() {}
Library::Library(BookNodePtr section[]) {
    for (int i = 0; i < 3; i++)
        this->section[i] = section[i];
}

// Library Setters
void Library::setSection(BookNodePtr section[]) {
    for (int i = 0; i < 3; i++)
        this->section[i] = section[i];
}

// Library Getters
BookNodePtr* Library::getSection() { return section; }

// Library Member Functions
void Library::readBookData(string filename) {
	ifstream ifs;
	ifs.open(filename);
    if (ifs.fail())
        throw std::invalid_argument(filename);
	
	BookNodePtr ptr = NULL;
	BookNodePtr childrenBookHead = NULL, computerBookHead = NULL, novelHead = NULL;

	int tempBookCode, tempBookAvailable, tempBookRent;
	string tempBookTitle;
	while (ifs >> tempBookCode) {
		try {
			ptr = new BookNode;
			if (tempBookCode > 1000 && tempBookCode <= 2000) { // If children's book code
				int tempBookAge;
				ifs >> tempBookTitle >> tempBookAge >> tempBookAvailable >> tempBookRent;

				// Create and fill dynamic children's book object
				ptr->book = new ChildrenBook(tempBookCode, tempBookTitle,
					tempBookAvailable, tempBookRent, tempBookAge);
				insertBookNodeByCode(childrenBookHead, ptr, tempBookCode);
			}
			else if (tempBookCode > 2000 && tempBookCode <= 3000) { // If computer book code
				string tempBookPublisher;
				ifs >> tempBookTitle >> tempBookPublisher >> tempBookAvailable >> tempBookRent;

				// Create and fill dynamic computer book object
				ptr->book = new ComputerBook(tempBookCode, tempBookTitle,
					tempBookAvailable, tempBookRent, tempBookPublisher);
				insertBookNodeByCode(computerBookHead, ptr, tempBookCode);
			}
			else if (tempBookCode > 3000 && tempBookCode <= 4000) { // If novel code
				int tempPublished_Date;
				ifs >> tempBookTitle >> tempPublished_Date >> tempBookAvailable >> tempBookRent;

				// Create and fill dynamic novel object
				ptr->book = new Novel(tempBookCode, tempBookTitle,
					tempBookAvailable, tempBookRent, tempPublished_Date);
				insertBookNodeByCode(novelHead, ptr, tempBookCode);
			}
			else throw tempBookCode;
		}
		catch (int bookCode) { // If book code not within 1001-4000
			cout << "Error : " << bookCode << " is not a valid book code." << endl;

			// Clear rest of input line and delete node
			string tLine;
			getline(ifs, tLine);
			delete ptr;
		}
	}

	ifs.close();

	section[0] = childrenBookHead;
	section[1] = computerBookHead;
	section[2] = novelHead;
}

BookPtr Library::findBook(const int code, const string title) const {
	int libSection = getLibrarySectionFromCode(code);

	if (libSection == -1) // If invalid code
		return NULL;

	for (BookNodePtr iter = section[libSection]; iter != NULL; iter = iter->link) {
		if (iter->book->getCode() == code && iter->book->getTitle() == title) // If inputted title and code are found and match
			return iter->book;
		else if ((iter->book->getCode() == code && iter->book->getTitle() != title) // If code found but inputted title doesn't match
			|| (iter->book->getTitle() == title && iter->book->getCode() != code)) // If title found but inputted code doesn't match
			throw "Code and title do not match";
	}

	return NULL;
}

BookPtr Library::findBook(const int code) const {
	int libSection = getLibrarySectionFromCode(code);

	if (libSection == -1) // If invalid code
		return NULL;

	for (BookNodePtr iter = section[libSection]; iter != NULL; iter = iter->link) {
		if (iter->book->getCode() == code) // If code found in linked list
			return iter->book;
	}

	return NULL;
}

BookPtr Library::findBook(const string title) const {
	for (int i = 0; i < 3; i++) {
		for (BookNodePtr iter = section[i]; iter != NULL; iter = iter->link) {
			if (iter->book->getTitle() == title) // If title found in library
				return iter->book;
		}
	}

	return NULL;
}

// Library Private Member Functions
void Library::insertBookNodeByCode(BookNodePtr& head, BookNodePtr& nodePtr, const int code) {
	BookNodePtr idx = NULL, prev = NULL;
	for (idx = head; idx != NULL; idx = idx->link) {
		if (code < idx->book->getCode()) // If the node getting sorted has
			break;                       // a smaller code than iterator's
		prev = idx;
	}

	if (idx == head) { // If node getting sorted ended up having the smallest code in list
		nodePtr->link = head;
		head = nodePtr;
	}
	else {
		nodePtr->link = prev->link;
		prev->link = nodePtr;
	}
}

int Library::getLibrarySectionFromCode(const int code) const {
	try {
		if (code > 1000 && code <= 2000) // If children's book code
			return 0;
		else if (code > 2000 && code <= 3000) // If computer book code
			return 1;
		else if (code > 3000 && code <= 4000) // If novel
			return 2;
		else throw code;
	}
	catch (int errCode) {
		cout << "Error : " << errCode << " is an invalid book code." << endl;
		return -1;
	}
}
