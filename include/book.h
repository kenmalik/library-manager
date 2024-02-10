#ifndef CS003A_PROJECT2_BOOK_H
#define CS003A_PROJECT2_BOOK_H

#include <iostream>
#include <fstream>
using namespace std;

class Book
{
public:
	// Constructors
	Book();
	Book(int, string, int, int);

	// Setters
	void setCode(int);
	void setTitle(string);
	void setAvailable(int);
	void setRented(int);

	// Getters
	int getCode() const;
	string getTitle() const;

	string getTitle(bool) const;
	// Precondition  : Bool indicating whether or not want title with underscores
	//                 provided (0 for without, 1 for with).
	// Postcondition : Appropriately returns title with or without underscores.

	int getAvailable() const;
	int getRented() const;

	int getSection() const;
	// Precondition  : Book object has a valid code.
	// Postcondition : Determines if object is a children's book, computer book,
	//                 or novel. Returns 0, 1, or 2 respectively.

	virtual int getAge() const = 0;
	virtual string getPublisher() const = 0;
	virtual int getPublish_Date() const = 0;

private:
	int code;      // Book code
	string title;  // Book title
	int available; // How many of book available to rent
	int rented;    // How many of book currently rented
};

class ChildrenBook : public Book
{
public:
	// Constructors
	ChildrenBook();
	ChildrenBook(int, string, int, int, int);

	// Setters
	void setAge(int);

	// Getters
	int getAge() const;
	string getPublisher() const;
	int getPublish_Date() const;

private:
	int age; // Intended age of book
};

class ComputerBook : public Book
{
public:
	// Constructors
	ComputerBook();
	ComputerBook(int, string, int, int, string);

	// Setters
	void setPublisher(string);

	// Getters
	string getPublisher() const;
	int getAge() const;
	int getPublish_Date() const;

private:
	string publisher; // Publisher of book
};

class Novel : public Book
{
public:
	// Constructors
	Novel();
	Novel(int, string, int, int, int);

	// Setters
	void setPublish_Date(int);
	
	// Getters
	int getPublish_Date() const;
	int getAge() const;
	string getPublisher() const;

private:
	int publish_date;
};

typedef Book* BookPtr;

struct BookNode
{
	BookPtr book;
	BookNode* link;
};
typedef BookNode* BookNodePtr;

class Library
{
public:
	// Constructors
	Library();
	Library(BookNodePtr[]);

	// Setters
	void setSection(BookNodePtr[]);

	// Getters
	BookNodePtr* getSection();

	// Member Functions
	void readBookData(string);
	// Precondition  : Book input file's name provided.
	// Postcondition : Creates linked list of children's books, computer books, 
	//                 and novels, their role determined by their book code and 
	//                 their order in the list sorted by ascending book code.
	//                 Puts these linked lists into object's section[] array.

	BookPtr findBook(const int, const string) const;
	// Precondition  : Book code and title provided.
	// Postcondition : If book object with matching code and title found, returns
	//                 pointer to the object, otherwise returns NULL. If code and
	//                 title do not match, throws error.

	BookPtr findBook(const int code) const;
	// Precondition  : Book code provided.
	// Postcondition : If book object with matching code found, returns pointer 
	//                 to the object, otherwise returns NULL.

	BookPtr findBook(const string title) const;
	// Precondition  : Book title provided.
	// Postcondition : If book object with matching title found, returns pointer
	//                 to the object, otherwise returns NULL.

private:
	BookNodePtr section[3]; // Stores linked list head for each section of books
	                        // (children's, computer, and novel)

	// Private Member Functions
	void insertBookNodeByCode(BookNodePtr&, BookNodePtr&, const int);
	// Precondition  : Pointer to (children's book, computer book, or novel) 
	//                 linked list head and pointer to book's linked list
	//                 node provided.
	// Postcondition : Inserts book's node into specified linked list by 
	//                 ascending book code order.

	int getLibrarySectionFromCode(const int) const;
	// Precondition  : Book code provided.
	// Postcondition : Determines if code would belong to a children's book, 
	//                 computer book, or novel and returns 0, 1, or 3 respectively.
};

#endif // !CS003A_PROJECT2_BOOK_H
