/* Hash Table - Data Structure for Student Information
   Author: Ayan Masud
   Date: 2/6/25
 */

#include <iostream>
#include <cstring>
#include "node.h"
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

void hashFunction(Node** &table, Node* added, int shift, int &tsize);
Node* add(Node* head, Node* current, Node* added);
void print(Node** table);
void printChain(Node* next);
void del(Node** table, int id);
Node* delChain(Node* head, Node* current, int id);

int length = 0; // keeps track of how many nodes are in the linked list (useful when adding the first node or removing the only node)
int tsize = 100; // size of the hash table
int main() {
  srand(time(NULL));
  
  cout << "'ADD' to add a student" << endl; // information as to what you can do
  cout << "'RANDOM' to add a randomly generated student" << endl;
  cout << "'PRINT' to print students" << endl;
  cout << "'DELETE' to delete a student" << endl;
  cout << "'QUIT' to leave" << endl;
  
  Node** table = new Node*[tsize]; // hash table

  char fNameArr[15][20];
  ifstream ffile("firstnames.txt");
  string randFstr;
  for(int i = 0; getline(ffile, randFstr); ++i) {
    strcpy(fNameArr[i], randFstr.c_str()); // had to get assigned to a string\
 originally so this line converts it into a char array
    //cout << fNameArr[i] << endl;
  }
  ffile.close();

  char lNameArr[15][20];
  ifstream lfile("lastnames.txt");
  string randLstr;
  for(int i = 0; getline(lfile, randLstr); ++i) {
    strcpy(lNameArr[i], randLstr.c_str()); // had to get assigned to a string\
 originally so this line converts it into a char array
    //cout << lNameArr[i] << endl;
  }
  lfile.close();
  
  while (true)
  {
    char cmd[8];
    cin.getline(cmd, 8); // get the command from the player
    
    if (strcmp(cmd, "ADD") == 0) { // adds a student
      Student* student = new Student(); // adds a new student to modify
      Node* current = new Node(student); // this node will carry this student
      
      cout << "First Name: "; // gets the users input
      cin >> student->fname;
      cout << "Last Name: ";
      cin >> student->lname;
      cout << "Student ID: ";
      cin >> student->id;
      cout << "GPA: ";
      cin >> student->gpa;

      hashFunction(table, current, 0, tsize);
    }
    else if (strcmp(cmd, "RANDOM") == 0) { // print the students in the list
      cout << "How many random students would you like to add? ";
      int amount = 0;
      cin >> amount;

      for (int i = 0; i < amount; i++) {
	Student* student = new Student();
	Node* current = new Node(student);

	// source for file line extraction https://www.geeksforgeeks.org/read-a-file-line-by-line-in-cpp/
      
	int randLine1 = (rand() % 15); // random line in list
	int randLine2 = (rand() % 15); // random line in list

	int randID = (rand() % 20000) + 500000; // random id num
        //cout << "id" << randID;

	float randGPA = (rand() % 401) / 100.0f; // random gpa
        //cout << endl << "gpa" << randGPA;

        // assign the students values
	strcpy(student->fname, fNameArr[randLine1]);
	//cout << fNameArr[randLine1] << endl;
	//cout << current->getStudent()->fname;
        strcpy(student->lname, lNameArr[randLine2]);
	student->id = randID;
	student->gpa = randGPA;

	int len = strlen(student->fname);
	
	hashFunction(table, current, 0, tsize);
      }

    }
    else if (strcmp(cmd, "PRINT") == 0) { // print the students in the list
      print(table);
    }
    else if (strcmp(cmd, "DELETE") == 0) { // delete a student
      int iderase = -1;
      cout << "Enter the ID of the person to delete: ";
      cin >> iderase;

      del(table, iderase);
    }
    else if (strcmp(cmd, "QUIT") == 0) { // quit
      cout << endl << "Cya!" << endl;
      break;
    }
    cout << endl;
  }
  
  return 0;
}

void hashFunction(Node** &table, Node* added, int shift, int &tsize) { // hash function for determining the key, adding the students node to the table, making sure the table is sized correctly with less than 4 collisions
  int id = added->getStudent()->id;

  // sum of the digits in the student ID
  int sum = 0;
  int n = id;
  while (n != 0) {
    sum += n % 10;
    n /= 10;
  }

  // formula for key: add the individual digits of the student id together and that sum to the student id unaltered. the last 2 digits is the key
  int key = ((id + sum) % 100) + (100 * shift);
  //cout << key;
  
  // resize the table by 100 if the key doesnt fit inside tsize
  while (key >= tsize) {
    int newSize = tsize + 100;
    Node** newTable = new Node*[newSize]();
    for (int i = 0; i < tsize; i++) { // copy existing stuff into new table
      newTable[i] = table[i];
    }
    delete[] table; // delete old table info from memory
    table = newTable;
    tsize = newSize;
  }

  // adding node into the table
  if (table[key] == nullptr) { // first node at this key
    table[key] = added;
  } else { // not first node so we need linked list
    Node* temp = table[key];
    int count = 2;
    while (temp->getNext() != nullptr) { // go through list until last one
      temp = temp->getNext();
      count++; // keep track of amount of nodes in list
    }
    if (count < 4) { // only allow 3 collisions in the linked list
      temp->setNext(added);
    } else { // recursion for shifting it 100 down the table
      hashFunction(table, added, shift + 1, tsize);
    }
  }
}

void print(Node** table) { // iterate through the hash table and print the linked list at the head of anything thats not null
  for (int i = 0; i < tsize; i++) {
    if (table[i] != NULL) {
      printChain(table[i]);
    }
  }
}

void printChain(Node* next) { // print the linked list
  if(next != NULL) {
    cout << endl << next->getStudent()->fname << endl << next->getStudent()->lname << endl << "ID: " << next->getStudent()->id << endl;
    cout << fixed << setprecision(2) << "GPA: " << next->getStudent()->gpa << endl;
    printChain(next->getNext());
  }
}

void del(Node** table, int id) { // iterate through the hash table
  for (int i = 0; i < tsize; i++) {
    if (table[i] != NULL) {
      table[i] = delChain(table[i], table[i], id);
    }
  }
}

Node* delChain(Node* head, Node* current, int id) { // delete a student from the linked list based off an id inputted
  if (current->getStudent()->id == id && length == 1) { // deleting the only student in the list
    head->~Node();
    return NULL;
  }

  if (current->getStudent()->id == id) { // if the head is the one to delete
    current = current->getNext();
    head->~Node();
    return current;
  }
  else {
    if (current->getNext() != NULL) { // not last in the list
      if (current->getNext()->getStudent()->id == id) { // currents next is the one to delete
	Node* newNext = current->getNext()->getNext(); // stores the new next node
	current->getNext()->~Node(); // delete the node to be deleted
	current->setNext(newNext); // fix the linked list order
	return head;
      }
      else { // keep going through the linked list until you find it
	delChain(head, current->getNext(), id); // recursion
      }
    }
  }
  //cout << "Invalid ID" << endl;
  return head;
}
