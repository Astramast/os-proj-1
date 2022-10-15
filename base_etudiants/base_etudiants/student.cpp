#include "student.h"
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
using namespace std;

void student_to_str(char* buffer, student_t* student) {
  /**
   * function: store in the buffer a human readable version of the student information
   * parameter: buffer: variable to store dynamically the content of the student, student: all the informations 
   *                                                                                           about a student in the data base
   * 
   */
  unsigned long len=sizeof(student_t);
  buffer= new char[len];
  buffer="id: "+student->id+ " First Name: "+student->fname+ " Last Name: "+ student->lname +" Section: "+ student->section+" Birthdate: "+
                                     student->birthdate.tm_mday+"/" +student->birthdate.tm_mon+"/"+student->birthdate.tm_year;
  printf("%s\n", buffer);
  //sprintf: function to add multiples things in a variable as strings       
}

int student_equals(student_t* student_1, student_t* student_2) {
/**
 * function: return 1 if both students are the same else return 0
 * paramater: student 1 and student 2, students in the data base
 * 
 */
  if(student_1->id == student_2->id){
    if(strcmp(student_1->fname,student_2->fname)==0){
      if(strcmp(student_1->lname,student_2->lname)==0){
        if(strcmp(student_1->section, student_2->section)==0){
          if(student_1->birthdate.tm_mday==student_2->birthdate.tm_mday){
            if(student_1->birthdate.tm_mon == student_2->birthdate.tm_mon){
              if(student_1->birthdate.tm_year == student_2->birthdate.tm_year){
                return 1;
              }
            }
          }
        }
      }
    }
  }
  return 0;
}


int main(){

  student_t etudiant1={5,"Tibo","malaga","info"};
  etudiant1.birthdate.tm_mday=2;
  etudiant1.birthdate.tm_mon=4;
  etudiant1.birthdate.tm_year=2001;

  student_t etudiant2={5,"Tibo","malaga","info"};
  etudiant2.birthdate.tm_mday= 2;
  etudiant2.birthdate.tm_mon= 4;
  etudiant2.birthdate.tm_year= 2001;

  student_to_str("", &etudiant1);
  cout<<student_equals(&etudiant1,&etudiant2)<<endl;
  return 0;
}
