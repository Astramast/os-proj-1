#include "query.h"
#include <time.h>
#include <string.h>
#include <cstring>
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
tuple<bool,string> data_research(string data_type,student_t* student){
    bool data_find=false;
    unsigned short int vector_index=0;
    string id="id";
    string fname="fname";
    string lname="lname";
    string section="section";
    vector<string>student_data={id,fname,lname,section};

    while(vector_index<student_data.size() or data_find == true){
      if(data_type == student_data[vector_index]){
        data_find=true;
      }
      vector_index++;
    }
    return {data_find,student_data[vector_index]};
}

void select(string data_type, student_t* student,database_t* data_base){
  vector<student_t*>sort_student_list;
  bool is_data_valid=get<0>(data_research(data_type,student));
  if(is_data_valid==true){
    for(unsigned long int i=0;i<data_base->psize;i++){
      if(data_base->data[i])
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
