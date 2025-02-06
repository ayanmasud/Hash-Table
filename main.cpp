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
Node* del(Node* head, Node* current, int id);
void average(Node* next, float sum);

int length = 0; // keeps track of how many nodes are in the linked list (useful when adding the first node or removing the only node)
int tsize = 100; // size of the hash table
int main() {
  cout << "'ADD' to add a student" << endl; // information as to what you can do
  cout << "'PRINT' to print students" << endl;
  cout << "'DELETE' to delete a student" << endl;
  cout << "'AVERAGE' to find the average GPA" << endl;
  cout << "'QUIT' to leave" << endl;
  
  Node** table = new Node*[tsize];

  Student* student = new Student();
  Node* current = new Node(student);
  strcpy(student->fname, "asdf");
  student->id = 123;
  hashFunction(table, current, 0, tsize);

  Student* student2 = new Student();
  Node* current2 = new Node(student2);
  strcpy(student2->fname, "fdsa");
  student2->id = 123;
  hashFunction(table, current2, 0, tsize);

  Student* student3 = new Student();
  Node* current3 = new Node(student3);
  strcpy(student3->fname, "mnoc");
  student3->id = 123;
  hashFunction(table, current3, 0, tsize);

  Student* student4 = new Student();
  Node* current4 = new Node(student4);
  strcpy(student4->fname, "sfwe");
  student4->id = 123;
  hashFunction(table, current4, 0, tsize);

  Student* student5 = new Student();
  Node* current5 = new Node(student5);
  strcpy(student5->fname, "ewae");
  student5->id = 123;
  hashFunction(table, current5, 0, tsize);

  // https://www.geeksforgeeks.org/read-a-file-line-by-line-in-cpp/
  
  while (true)
  {
    char cmd[8];
    cin.getline(cmd, 8); // get the command from the player
    
    if (table[29]->getNext()->getNext() != NULL) {
      cout << "daire";
    }
    else {
      cout << "nun";
    }
    
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
      Student* student = new Student();
      Node* current = new Node(student);

      ifstream("firstnames.txt",ios::in);
      
    }
    else if (strcmp(cmd, "PRINT") == 0) { // print the students in the list
      print(table);
    }
    /*else if (strcmp(cmd, "DELETE") == 0) { // delete a student
      int iderase = -1;
      cout << "Enter the ID of the person to delete: ";
      cin >> iderase;

      head = del(head, head, iderase);
      length--;
    }
    else if (strcmp(cmd, "AVERAGE") == 0) { // calculate the average gpa
      average(head, 0);
    }
    else if (strcmp(cmd, "QUIT") == 0) { // quit
      cout << endl << "Cya!" << endl;
      break;
      }*/
    cout << endl;
  }
  
  return 0;
}

void hashFunction(Node** &table, Node* added, int shift, int &tsize) {
  int id = added->getStudent()->id;

  // sum of the digits in the student ID
  int sum = 0;
  int n = id;
  while (n != 0) {
    sum += n % 10;
    n /= 10;
  }

  // formula for key
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

void print(Node** table) { // print the linked list
  for (int i = 0; i < tsize; i++) {
    if (table[i] != NULL) {
      printChain(table[i]);
    }
  }
}

void printChain(Node* next) { // print the linked list
  if(next != NULL) {
    cout << next->getStudent()->fname << " " << next->getStudent()->lname << ", " << next->getStudent()->id << ", ";
    cout << fixed << setprecision(2) << next->getStudent()->gpa << endl;
    printChain(next->getNext());
  }
}

Node* del(Node* head, Node* current, int id) { // delete a student from the linked list based off an id inputted
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
	del(head, current->getNext(), id); // recursion
      }
    }
  }
  cout << "Invalid ID" << endl;
  return head;
}

void average(Node* next, float sum) { // calculates the average gpa among all the students in the list
  if (next != NULL) {
    sum+=next->getStudent()->gpa;
    average(next->getNext(), sum); // recursion
  }
  else {
    float avg = sum/length;
    cout << "GPA average of all students: ";
    cout << fixed << setprecision(2) << avg << endl;
  }
}
