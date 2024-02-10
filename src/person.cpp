#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "person.h"
using namespace std;

// Base Constructors
Person::Person() : id(0), name(""), count(0) {}
Person::Person(int id, string name, int count) {
    this->id = id;
    this->name = name;
    this->count = count;
}

// Base Setters
void Person::setId(int id) { this->id = id; }
void Person::setName(string name) { this->name = name; }
void Person::setCount(int count) { this->count = count; }

// Base Getters
int Person::getId() const { return id; }
string Person::getName() const { return name; }
int Person::getCount() const { return count; }
int Person::getRole() const {
    try {
        if (id > 0 && id <= 100) // If teacher id
            return 0;
        else if (id > 100 && id <= 300) // If student id
            return 1;
        else throw id;
    }
    catch (int errId) {
        cout << "Error : " << errId << " is an invalid ID." << endl;
        return -1;
    }
}

// Student Constructors
Student::Student() : code() {}
Student::Student(int id, string name, int count, int code[]) : Person(id, name, count) {
    for (int i = 0; i < 2; i++)
        this->code[i] = code[i];
}

// Student Setters
void Student::setCode(int code[]) {
    for (int i = 0; i < 2; i++)
        this->code[i] = code[i];
}

// Student Getters
int* Student::getCode() { return code; }

// Student Member Functions
int Student::removeFromCode(int code) {
    int tempCode = 0;
    bool codeRemoved = 0;

    int i;
    // Search for code in rented list
    for (i = 0; i < 2; i++) {
        if (this->code[i] == code) { // If book code found in rented list
            tempCode = this->code[i];
            this->code[i] = 0;
            codeRemoved = 1;
            break;
        }
    }

    // Move items in rented list to fill removed code's space
    for (i = i + 1; codeRemoved && i < 2; i++)
        swap(this->code[i], this->code[i - 1]);

    return tempCode;
}

bool Student::isRenting(int bookCode) const {
    for (int i = 0; i < this->getCount(); i++) {
        if (code[i] == bookCode) // If book code found in rented list
            return 1;
    }
    return 0;
}

// Teacher Constructors
Teacher::Teacher() : code() {}
Teacher::Teacher(int id, string name, int count, int code[]) : Person(id, name, count) {
    for (int i = 0; i < 3; i++)
        this->code[i] = code[i];
}

// Teacher Setters
void Teacher::setCode(int code[]) {
    for (int i = 0; i < 3; i++)
        this->code[i] = code[i];
}

// Teacher Getters
int* Teacher::getCode() { return code; }

// Teacher Member Functions
int Teacher::removeFromCode(int code) {
    int tempCode = 0;
    bool codeRemoved = 0;

    int i;
    // Search for code in rented list
    for (i = 0; i < 3; i++) {
        if (this->code[i] == code) { // If book code found in rented list
            tempCode = this->code[i];
            this->code[i] = 0;
            codeRemoved = 1;
            break;
        }
    }

    // Move items in rented list to fill removed code's space
    for (i = i + 1; codeRemoved && i < 3; i++)
        swap(this->code[i], this->code[i - 1]);

    return tempCode;
}

bool Teacher::isRenting(int bookCode) const {
    for (int i = 0; i < this->getCount(); i++) {
        if (code[i] == bookCode) // If book code found in rented list
            return 1;
    }
    return 0;
}

// Roster Constructors
Roster::Roster() : role() {}
Roster::Roster(PersonNodePtr role[]) {
    for (int i = 0; i < 2; i++)
        this->role[i] = role[i];
}

// Roster Setters
void Roster::setRole(PersonNodePtr role[]) {
    for (int i = 0; i < 2; i++)
        this->role[i] = role[i];
}

// Roster Getters
PersonNodePtr* Roster::getRole() { return role; }

// Roster Member Functions
void Roster::readPersonData(string filename) {
    ifstream ifs;
    ifs.open(filename);

    if (ifs.fail())
        throw std::invalid_argument(filename);

    PersonNodePtr ptr = NULL;
    PersonNodePtr teacherHead = NULL, studentHead = NULL;

    int tempId, tempRentedBookCount;
    string tempName;
    while (ifs >> tempId) {
        try {
            ptr = new PersonNode;
            ifs >> tempName >> tempRentedBookCount;
            if (tempId > 0 && tempId <= 100) { // If teacher id
                if (tempRentedBookCount > 3)
                    throw "Too many book codes";

                // Create and fill dynamic teacher object
                ptr->person = new Teacher;
                ptr->person->setId(tempId);
                ptr->person->setName(tempName);
                ptr->person->setCount(tempRentedBookCount);
                for (int i = 0; i < tempRentedBookCount; i++)
                    ifs >> ptr->person->getCode()[i];

                insertPersonNodeByCode(teacherHead, ptr);
            }
            else if (tempId > 100 && tempId <= 300) { // If student id
                if (tempRentedBookCount > 2)
                    throw "Too many book codes";

                // Create and fill dynamic student object
                ptr->person = new Student;
                ptr->person->setId(tempId);
                ptr->person->setName(tempName);
                ptr->person->setCount(tempRentedBookCount);
                for (int i = 0; i < tempRentedBookCount; i++)
                    ifs >> ptr->person->getCode()[i];

                insertPersonNodeByCode(studentHead, ptr);
            }
            else throw tempId;
        }
        catch (int errId) { // If id not within 1-300
            cout << errId << " is not a valid id." << endl;

            // Clear rest of input line and delete node
            string tempLine;
            getline(ifs, tempLine);
            delete ptr;
        }
        catch (const char* errorMessage) {
            cout << "Error : " << errorMessage << endl;

            // Clear rest of input line and delete node
            string tempLine;
            getline(ifs, tempLine);
            delete ptr;
        }
    }

    ifs.close();

    role[0] = teacherHead;
    role[1] = studentHead;
}
PersonPtr Roster::findPerson(const int id) const {
    int roleIdx = getRoleFromId(id);

    if (roleIdx == -1) // If invalid id
        return NULL;

    for (PersonNodePtr idx = role[roleIdx]; idx != NULL; idx = idx->link) {
        if (idx->person->getId() == id) // If id match found
            return idx->person;
    }

    return NULL;
}

// Roster Private Member Functions
void Roster::insertPersonNodeByCode(PersonNodePtr& head, PersonNodePtr& nodePtr) {
    PersonNodePtr iter = NULL, prev = NULL;
    for (iter = head; iter != NULL; iter = iter->link) {
        if (nodePtr->person->getId() < iter->person->getId()) // If the node getting sorted has
            break;                                            // a smaller id than iterator's
        prev = iter;
    }

    if (iter == head) { // If node getting sorted ended up having the smallest id in list
        nodePtr->link = head;
        head = nodePtr;
    }
    else {
        nodePtr->link = prev->link;
        prev->link = nodePtr;
    }
}

int Roster::getRoleFromId(const int id) const {
    try {
        if (id > 0 && id <= 100) // If teacher id
            return 0;
        else if (id > 100 && id <= 300) // If student id
            return 1;
        else throw id;
    }
    catch (int errId) {
        cout << "Error : " << errId << " is an invalid ID." << endl;
        return -1;
    }
}
