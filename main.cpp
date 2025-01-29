#include <iostream>
#include <cstring>
#include "node.h"
#include <vector>
#include <iomanip>

using namespace std;

void hashFunction(Node** &table, Node* added);
Node* add(Node* head, Node* current, Node* added);
void print(Node* next);
Node* del(Node* head, Node* current, int id);
void average(Node* next, float sum);

int length = 0; // keeps track of how many nodes are in the linked list (useful when adding the first node or removing the only node)
int main() {
  cout << "'ADD' to add a student" << endl; // information as to what you can do
  cout << "'PRINT' to print students" << endl;
  cout << "'DELETE' to delete a student" << endl;
  cout << "'AVERAGE' to find the average GPA" << endl;
  cout << "'QUIT' to leave" << endl;
  
  Node** table = new Node*[100];
  
  while (true)
  {
    char cmd[8];
    cin.getline(cmd, 8); // get the command from the player

    Student* student = new Student();
    Node* current = new Node(student);
    strcpy(student->fname, "asdf");
    student->id = 123;
    hashFunction(table, current);

    Student* student2 = new Student();
    Node* current2 = new Node(student2);
    strcpy(student->fname, "fdsa");
    student->id = 420;
    hashFunction(table, current);
    
    
    /*if (strcmp(cmd, "ADD") == 0) { // adds a student
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

      hashFunction(table, current);
      //head = add(head, head, current);
      //length++;
      }*/
    else if (strcmp(cmd, "PRINT") == 0) { // print the students in the list
      print(head);
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

void hashFunction(Node** &table, Node* added) {
  int n = added->getStudent()->id;
  int sum = 0;
  while (n != 0) {
    int last = n % 10;
    sum += last;
    n /= 10; // this works because its an int so it wont keep the decimal
  }
  cout << sum;

  int key = (n + sum) % 100;

  if (table[key] == NULL) { // first to this key
    table[key] = added;
  }
  else { // use linked list for this same location
    Node* temp = table[key];
    while (temp->getNext() != NULL) {
      temp = temp->getNext();
    }
    temp->setNext(added);
  }
  
  cout << table[sum]->getStudent()->fname;
}

Node* add(Node* head, Node* current, Node* added) { // add a node to the linked list
  if (length == 0) { // empty list so its sets the one added as the head
    return added;
  }
  
  if (added->getStudent()->id > current->getStudent()->id) { // if the new node is bigger than the current
    if (current->getNext() == NULL) {
      current->setNext(added);
      return head;
    }
    else if (added->getStudent()->id < current->getNext()->getStudent()->id) { // if the new node is less than than the currents next node. this is where the new node belongs
      added->setNext(current->getNext());
      current->setNext(added);
      return head;
    }
    else { // new node is greater than currents next so we need to go deeper into the linked list to find its spot
      add(head, current->getNext(), added); // recursion
    }
  } // new node is less than current so it becomes the new head
  else {
    added->setNext(head);
    return added;
  }
  return head;
}

void print(Node* next) { // print the linked list
  if(next != NULL) {
    cout << next->getStudent()->fname << " " << next->getStudent()->lname << ", " << next->getStudent()->id << ", ";
    cout << fixed << setprecision(2) << next->getStudent()->gpa << endl;
    print(next->getNext());
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
