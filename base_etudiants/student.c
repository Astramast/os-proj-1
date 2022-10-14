#include "student.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void student_to_str(char* buffer, student_t* student) {
  unsigned long len=sizeof(student_t);
  buffer= malloc(len);
  strcat(buffer,itoa(student->id));
  strcat(buffer,student->fname);
  strcat(buffer," ");
  strcat(buffer,student->lname);
  strcat(buffer," ");
  strcat(buffer,student->section);
  sprintf(buffer,"%d: ",student->birthdate.tm_mday);
  //strcat(buffer,student->birthdate.tm_mon);
  //strcat(buffer,student->birthdate.tm_year);
                                                  
  printf("\nlen: %lu",len);
  printf("\nbuffer: %s",buffer);
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
  return 0;
}