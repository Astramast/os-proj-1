#include "query.h"
#include <time.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

void insert(student_t* student, database_t* data_base){
    for(unsigned long int i=0;i<data_base->data->id;i++ ){
      if(data_base->data[i].id==student->id){
          throw invalid_argument("The id where you want to set a new student already exist.");
      }
    }
    db_add(data_base,*student); 
    cout<<"student inserted with succes"<<endl;
}
bool data_research_validity(string data_type,student_t* student){
    bool data_find=false;
    string id=to_string(student->id);
    string fname=student->fname;
    string lname=student->lname;
    string section=student->section;
    string birthday_day=to_string(student->birthdate.tm_mday);
    string birthday_month=to_string(student->birthdate.tm_mon);
    string birthday_year=to_string(student->birthdate.tm_year);

    vector<string>student_data={id,fname,lname,section,birthday_day,birthday_month,birthday_year};

    while(unsigned short int vector_index=0<student_data.size() or data_find == true){
      if(data_type == student_data[vector_index]){
        data_find=true;
      }
    }
    return data_find;
}

void select(string data_type, student_t* student,database_t* data_base){
  if(data_research_validity(data_type,student)==true){
    for(unsigned long int i=0;i<data_base->psize){
      
    }
  }
}

void query_result_init(query_result_t* result, const char* query) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  result->start_ns = now.tv_nsec + 1e9 * now.tv_sec;
  result->status = QUERY_SUCCESS;
  // Votre code ici
}
