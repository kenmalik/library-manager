#ifndef CS003A_PROJECT2_PERSON_H
#define CS003A_PROJECT2_PERSON_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Person
{
public:
	// Constructors
	Person();
	Person(int, string, int);

	// Setters
	void setId(int);
	void setName(string);
	void setCount(int);

	// Getters
	int getId() const;
	string getName() const;
	int getCount() const;
	virtual int* getCode() = 0;
	int getRole() const;
	// Precondition  : Person object has a valid id.
	// Postcondition : Determines if object is a teacher or student. Returns 0 or
	//                 or 1, respectively.
	
	// Member functions
	virtual int removeFromCode(int) = 0;
	// Precondition  : Book code provided and person object is renting the book.
	// Postcondition : Removes book code from person object's list of rented books.

	virtual bool isRenting(int) const = 0;
	// Precondition  : Book code provided.
	// Postcondition : If user is renting the book, returns true, otherwise false.

private:
	int id;      // Person's id number
	string name; // Person's name
	int count;   // How many books person has rented
};

class Student : public Person
{
public:
	// Constructors
	Student();
	Student(int, string, int, int[]);

	// Setters
	void setCode(int[]);

	// Getters
	int* getCode();

	// Member functions
	int removeFromCode(int);
	bool isRenting(int) const;

private:
	int code[2]; // Stores codes of books person has rented
};

class Teacher : public Person
{
public:
	// Constructors
	Teacher();
	Teacher(int, string, int, int[]);

	// Setters
	void setCode(int[]);

	// Getters
	int* getCode();

	// Member functions
	int removeFromCode(int);
	bool isRenting(int) const;

private:
	int code[3]; // Stores codes of books person has rented
};

typedef Person* PersonPtr;

struct PersonNode
{
	PersonPtr person;
	PersonNode* link;
};
typedef PersonNode* PersonNodePtr;

class Roster
{
public:
	// Constructors
	Roster();
	Roster(PersonNodePtr[]);

	// Setters
	void setRole(PersonNodePtr[]);

	// Getters
	PersonNodePtr* getRole();

	// Member Functions
	void readPersonData(string);
	// Precondition  : Person input file's name provided.
	// Postcondition : Creates linked list of students and teachers, their role
	//                 determined by their id numbers and their order in the list
	//                 sorted by ascending id. Puts these linked lists into object's
	//                 role[] array.

	PersonPtr findPerson(const int) const;
	// Precondition  : Person's id number provided.
	// Postcondition : If person object with matching id found in linked list,
	//                 returns pointer to the object, otherwise returns NULL.

private:
	PersonNodePtr role[2]; // Stores linked list header for each person role
	                       // (teacher and student)

	// Private Member Functions
	void insertPersonNodeByCode(PersonNodePtr&, PersonNodePtr&);
	// Precondition  : Pointer to (teacher or student) linked list head and
	//                 pointer to person's linked list node provided.
	// Postcondition : Inserts person's node into specified linked list by 
	//                 ascending id order. 

	int getRoleFromId(const int) const;
	// Precondition  : Id number provided.
	// Postcondition : Determines if id would belong to a teacher or student and
	//                 returns 0 or 1, respectively.
};


#endif // !CS003A_PROJECT2_PERSON_H