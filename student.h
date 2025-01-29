#include <iostream>
#include <cstring>

using namespace std;

// all the variables and functions that student has stored in it

class Student {
 public:
  char fname[20];
  char lname[20];
  int id;
  float gpa;

  Student();
  ~Student();
};
