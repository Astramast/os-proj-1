#include "query.h"
#include <time.h>
#include <iostream>
#include <vector>
#include <map>
#include <any>
using namespace std;

void insert(student_t* student, database_t* data_base){
  /**
   * function: add a student in the data base if the id doesn't already exist
   */
   
    for(unsigned long int i=0;i<data_base->lsize;i++ ){
      if(data_base->data[i].id==student->id){
          throw invalid_argument("The id where you want to set a new student already exist.");
      }
    }
    db_add(data_base,*student); 
    cout<<"student inserted with success"<<endl;
}
tuple<bool,string>data_analyse(string data_filter,string filter_asked){
  /**
   * function:
   */

    bool is_data_find=false;
    unsigned short int vector_index=0;
    string id="id";
    string fname="fname";
    string lname="lname";
    string section="section";
    string birthdate="birthdate";
    vector<string>student_data={id,fname,lname,section,birthdate};//creation of a list to store the data

    while(vector_index<student_data.size() or is_data_find == true){
      if(data_filter == student_data[vector_index]){
        is_data_find=true;
      }
      vector_index++;
    }
    if(is_data_find ==false){
      return {is_data_find,""};
    }
    return {is_data_find,student_data[vector_index]};
}

vector<student_t*> select(string data_type,string filter_asked,student_t* student,database_t* data_base){
  /**
   * function: return a list of student sorted by the filter data_type that the user sent
   */

  vector<student_t*>sort_student_list;
  //we get the info of the data_research function
  bool is_data_valid=get<0>(data_analyse(data_type,filter_asked));
  string data_write=get<1>(data_analyse(data_type,filter_asked));
  if(is_data_valid==true){
    for(unsigned long int i=0;i<data_base->lsize;i++){
      if(data_base->data[i]->id)
        sort_student_list.push_back(student);
    }
    return sort_student_list;
  }
  else{
    throw invalid_argument("The data that you want to select doesn't exist.");
  }
}

void query_result_init(query_result_t* result, const char* query) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  result->start_ns = now.tv_nsec + 1e9 * now.tv_sec;
  result->status = QUERY_SUCCESS;
  // Votre code ici
  //constructeur(initialise les valeurs)
}

void query_result_add(query_result_t* result, student_t s){

}
