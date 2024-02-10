// 10597063, Kenji, Longid
// Date: 8.13.23
// Project2, v1

#include <iostream>
#include <string>
#include <iomanip>

#include "book.h"
#include "person.h"
using namespace std;

int mainMenu();
// Precondition  : Function called from control loop.
// Postcondition : Takes user input and returns selected menu number to 
//                 control loop.

void bookSearchMenu(const Library&);
// Precondition  : Library object provided.
// Postcondition : Takes user input and prints book info if found. If book is
//                 not found or provided title and code do not match, prints error.

void printBookData(const Book&);
// Precondition  : Book object provided.
// Postcondition : Prints information stored in book object.

void bookRentMenu(Library&, Roster&);
// Precondition  : Library and roster object provided.
// Postcondition : Takes user input and prompts user confirmation to rent book.
//                 Alters book and person data appropriately. If user already
//                 rented the max amount of books or the book is unavailable,
//                 prints error.

void bookReturnMenu(Library&, Roster&);
// Precondition  : Library and roster object provided.
// Postcondition : Takes user input and prompts user confirmation to return book.
//                 Alters book and person data appropriately. If user already
//                 did not rent out the book, prints error.

void showPersonInfoMenu(const Library&, const Roster&);
// Precondition  : Library and roster object provided.
// Postcondition : Prints amount of books user rented and the data for each
//                 rented book.

void showAllBooksMenu(Library&);
// Precondition  : Library object provided.
// Postcondition : Prints data for every book stored in library object.

int main() {
	Library lib;
	lib.readBookData("resources/book.txt");

	Roster rost;
	rost.readPersonData("resources/person.txt");

	bool loopMenu = 1;
	while (loopMenu) {
		switch (mainMenu()) {
		case 1:
			bookSearchMenu(lib);
			break;
		case 2:
			bookRentMenu(lib, rost);
			break;
		case 3:
			bookReturnMenu(lib, rost);
			break;
		case 4:
			showPersonInfoMenu(lib, rost);
			break;
		case 5:
			showAllBooksMenu(lib);
			break;
		case 6:
			loopMenu = 0;
			break;
		default:
			cout << "Not an option" << endl;
		}
	}

	// Delete roster linked list
	for (int i = 0; i < 2; i++) {
		PersonNodePtr iter, prev = rost.getRole()[i];
		for (iter = rost.getRole()[i]->link; iter != NULL; iter = iter->link) {
			delete prev;
			prev = iter;
		}
		delete prev;
	}

	// Delete library linked list
	for (int i = 0; i < 3; i++) {
		BookNodePtr iter, prev = lib.getSection()[i];
		for (iter = lib.getSection()[i]->link; iter != NULL; iter = iter->link) {
			delete prev;
			prev = iter;
		}
		delete prev;
	}

	return 0;
}

int mainMenu() {
	cout << "------------------------" << endl
		<< "          Menu          " << endl
		<< "------------------------" << endl;

	cout << " 1. Search a book       " << endl
		<< " 2. Rent a book         " << endl
		<< " 3. Return a book       " << endl
		<< " 4. Show my information " << endl
		<< " 5. Show all books      " << endl
		<< " 6. Exit                " << endl;

	int selection;
	cout << "\nSelect (1~6) : ";
	cin >> selection;
	
	return selection;
}

void bookSearchMenu(const Library& lib) {
	int tempCode;
	string tempTitle;

	cout << "\nEnter code  : ";
	cin >> tempCode;
	cout << "Enter title : ";
	cin >> tempTitle;

	try {
		BookPtr bookMatch = lib.findBook(tempCode, tempTitle);

		if (bookMatch) // If book found, print data
			printBookData(*bookMatch);
		else
			throw "Book not found";
	}
	catch (const char* errMsg) {
		cout << "Error : " << errMsg << endl;
		cout << endl;
	}
}

void printBookData(const Book& book) {
	int libSection = book.getSection();

	cout << endl << book.getTitle() << "(" << book.getCode() << ") exists." << endl;

	if (libSection == 0) { // If children's book
		cout << "category : children's" << endl;
		cout << "age : " << book.getAge() << endl;
	}
	else if (libSection == 1) { // If computer book
		cout << "category : computer" << endl;
		cout << "publisher : " << book.getPublisher() << endl;
	}
	else if (libSection == 2) { // Novel
		cout << "category : novel" << endl;
		cout << "publish date : " << book.getPublish_Date() << endl;
	}

	cout << book.getAvailable() << " available, "
		<< book.getRented() << " rented" << endl;
	cout << endl;
}

void bookRentMenu(Library& lib, Roster& rost) {
	int tempId;
	string tempTitle;
	cout << "\nEnter your id    : ";
	cin >> tempId;
	cout << "Enter book title : ";
	cin >> tempTitle;

	// Get pointers to person and book objects
	PersonPtr personMatch = rost.findPerson(tempId);
	BookPtr bookMatch = lib.findBook(tempTitle);

	try {
		if (personMatch == NULL)
			throw "Person not found";
		if (bookMatch == NULL)
			throw "Book not found";

		if (bookMatch->getAvailable() <= 0) // If no available copies
			throw "Book is unavailable";

		if (personMatch->getRole() == 0 && personMatch->getCount() >= 3) // If teacher already rented 3 books
			throw "Already rented max amount of books (3)";
		else if (personMatch->getRole() == 1 && personMatch->getCount() >= 2) // If student already rented 2 books
			throw "Already rented max amount of books (2)";
	}
	catch (const char* errMsg) {
		cout << "Error : " << errMsg << "\n\n";
		return;
	}

	// Book rental confirmation
	char input;
	cout << "\nYou are " << personMatch->getName()
		<< ". You rented " << personMatch->getCount() << " book." << endl;
	cout << "Do you want to rent '" << bookMatch->getTitle() << "' (y/n) ? ";
	cin >> input;
	
	if (input == 'y') {
		// Change person and book data in linkedlist to reflect rental
		personMatch->getCode()[personMatch->getCount()] = bookMatch->getCode();
		personMatch->setCount(personMatch->getCount() + 1);

		bookMatch->setAvailable(bookMatch->getAvailable() - 1);
		bookMatch->setRented(bookMatch->getRented() + 1);

		cout << "***** Rent succeed. Check your info!" << endl;
	}

	cout << endl;
}

void bookReturnMenu(Library& lib, Roster& rost) {
	int tempId, tempCode;
	cout << "\nEnter your id    : ";
	cin >> tempId;
	cout << "Enter the book code to return : ";
	cin >> tempCode;

	// Get pointers to person and book objects
	PersonPtr personMatch = rost.findPerson(tempId);
	BookPtr bookMatch = lib.findBook(tempCode);

	try {
		if (personMatch == NULL)
			throw "Person not found";
		if (bookMatch == NULL)
			throw "Book not found";

		if (!personMatch->isRenting(tempCode)) // If person is not renting book
			throw "You did not rent out this book";


	}
	catch (const char* errMsg) {
		cout << "Error : " << errMsg << "\n\n";
		return;
	}

	// Book return confirmation
	char input;
	cout << "Do you want to return '" << bookMatch->getTitle() << "' (y/n)? ";
	cin >> input;

	if (input == 'y') {
		// Change person and book data in linkedlist to reflect return
		personMatch->setCount(personMatch->getCount() - 1);
		personMatch->removeFromCode(tempCode);

		bookMatch->setAvailable(bookMatch->getAvailable() + 1);
		bookMatch->setRented(bookMatch->getRented() - 1);

		cout << "***** Return succeed. Check your info!" << endl;
	}

	cout << endl;
}

void showPersonInfoMenu(const Library& lib, const Roster& rost) {
	int tempId;
	string tempName;
	cout << "\nEnter your id   : ";
	cin >> tempId;
	cout << "Enter your name : ";
	cin >> tempName;

	// Get pointer to person object
	PersonPtr personMatch = rost.findPerson(tempId);

	cout << "\nYou rented " << personMatch->getCount() << " books." << endl;

	// Print rented books
	for (int i = 0; i < personMatch->getCount(); i++) { 
		BookPtr bookMatch = lib.findBook(personMatch->getCode()[i]);
		int libSection = bookMatch->getSection();

		cout << "* " << bookMatch->getTitle() << "(" << bookMatch->getCode() << ") - ";
		
		if (libSection == 0) // If children's book
			cout << "age " << bookMatch->getAge();
		else if (libSection == 1) // If computer book
			cout << "publisher " << bookMatch->getPublisher();
		else if (libSection == 2) // If novel
			cout << "publish date " << bookMatch->getPublish_Date();
		cout << endl;
	}

	cout << endl;
}

void showAllBooksMenu(Library& lib) {
	BookNodePtr iter;

	cout << "\n==========================================================" << endl;
	cout << "                 Children Books       " << endl;
	cout << "==========================================================" << endl;
	cout << " code              title             age available rented " << endl;
	cout << "----------------------------------------------------------" << endl;
	
	// Print all children's books
	for (iter = lib.getSection()[0]; iter != NULL; iter = iter->link) {
		cout << right << setw(5) << iter->book->getCode()
			<< setw(30) << iter->book->getTitle(0) << "  "
			<< setw(2) << iter->book->getAge() << "     "
			<< setw(3) << iter->book->getAvailable() << "    "
			<< setw(3) << iter->book->getRented() << endl;
	}

	cout << "\n=========================================================================" << endl;
	cout << "                 Computer Books       " << endl;
	cout << "=========================================================================" << endl;
	cout << " code              title                   publisher    available rented " << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	
	// Print all computer books
	for (iter = lib.getSection()[1]; iter != NULL; iter = iter->link) {
		cout << right << setw(5) << iter->book->getCode()
			<< setw(30) << iter->book->getTitle(0) << "  "
			<< setw(18) << iter->book->getPublisher() << "    "
			<< setw(3) << iter->book->getAvailable() << "    "
			<< setw(3) << iter->book->getRented() << endl;
	}

	cout << "\n=====================================================================" << endl;
	cout << "                 Novels" << endl;
	cout << "=====================================================================" << endl;
	cout << " code              title              publish date   available rented " << endl;
	cout << "---------------------------------------------------------------------" << endl;

	// Print all novels
	for (iter = lib.getSection()[2]; iter != NULL; iter = iter->link) {
		cout << right << setw(5) << iter->book->getCode()
			<< setw(30) << iter->book->getTitle(0) << "       "
			<< setw(5) << iter->book->getPublish_Date() << "         "
			<< setw(3) << iter->book->getAvailable() << "    "
			<< setw(3) << iter->book->getRented() << endl;
	}

	cout << endl;
}
