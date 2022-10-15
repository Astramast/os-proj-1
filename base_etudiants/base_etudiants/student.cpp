#include "student.h"
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
using namespace std;

void student_to_str(char* buffer, student_t* student) {
  unsigned long len=sizeof(student_t);
  buffer= new char[len];
  sprintf(buffer,"id: %u First name: %s Last name: %s section: %s Birthdate: %d /%d /%d ", student->id, student->fname, student->lname, student->section, 
                                     student->birthdate.tm_mday, student->birthdate.tm_mon, student->birthdate.tm_year);       
}

int student_equals(student_t* student_1, student_t* student_2) {
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
