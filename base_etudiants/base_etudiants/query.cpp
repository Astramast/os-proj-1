#include "query.h"
#include <time.h>
#include <iostream>
#include <vector>
#include <map>
#include <any>
#include "db.h"
#include "db.cpp"
using namespace std;

void insert(student_t* student, database_t* data_base){

  for(unsigned long int i=0;i<data_base->lsize;i++ ){
    if(data_base->data[i].id==student->id){
        throw invalid_argument("The id where you want to set a new student already exist.");
    }
  }
  db_add(data_base,*student); 
  cout<<"student inserted with success"<<endl;
}
bool data_analyse(string data_filter,string filter_asked){

    bool is_data_find=false;
    string id="id";
    string fname="fname";
    string lname="lname";
    string section="section";
    string birthdate="birthdate";
    vector<string>student_data={id,fname,lname,section,birthdate};//creation of a list to store the data

	unsigned short int vector_index=0;
    while(is_data_find == false and vector_index<student_data.size()){
      if(data_filter == student_data[vector_index]){
        is_data_find=true;
      }
      vector_index++;
    }
    return is_data_find;
}

vector<student_t*> select(string data_type,string filter_asked, database_t* data_base){

  vector<student_t*>sort_student_list;
  //we get the info of the data_research function
  if(data_analyse(data_type, filter_asked)){
    for(unsigned long int i=0;i<data_base->lsize;i++){
      
	  if(data_type=="id"){
        if(data_base->data[i].id == stoul(filter_asked)){//probleme de conversion 
          sort_student_list.push_back(&data_base->data[i]);
        }
      }  

      else if(data_type=="fname"){
        if(data_base->data[i].fname == filter_asked){
          sort_student_list.push_back(&data_base->data[i]);
        }
      }
      
      else if(data_type=="lname"){
        if(data_base->data[i].lname == filter_asked){
          sort_student_list.push_back(&data_base->data[i]);
        }
      }

      else if(data_type=="section"){
        if(data_base->data[i].section == filter_asked){
          sort_student_list.push_back(&data_base->data[i]);
        }
      }

	  else if(data_type=="birthdate"){
      char* student_bd_temp;
      tm* student_tm = &data_base->data[i].birthdate;
      strftime(student_bd_temp, 10, "%d/%m/%Y", student_tm);
      	if(student_bd_temp == filter_asked){
          sort_student_list.push_back(&data_base->data[i]);
        }
      }

    }
  }
  else{
    throw invalid_argument("The data that you want to select doesn't exist.");
  }
  return sort_student_list;
}

void delete_function(string data_type,string filter_asked,database_t* data_base){

  bool is_data_valid=data_analyse(data_type,filter_asked);
  if(is_data_valid){
    for(unsigned long int i=0;i<data_base->lsize;i++){
		  bool delete_student = false;
      if(data_type=="id"){
        if(data_base->data[i].id == stoul(filter_asked)){
			delete_student = true;
        }
      }  

      else if(data_type=="fname"){
        if(data_base->data[i].fname == filter_asked){
			    delete_student = true;
        }
      }
      
      else if(data_type=="lname"){
        if(data_base->data[i].lname == filter_asked){
			    delete_student = true;
        }
      }

      else if(data_type=="section"){
        if(data_base->data[i].section == filter_asked){
			    delete_student = true;
        }
      }

      else if(data_type=="birthdate"){

        char* student_bd_temp;
        tm* student_tm = &data_base->data[i].birthdate;
        strftime(student_bd_temp, 10, "%d/%m/%Y", student_tm);

        if(student_bd_temp == filter_asked){
			    delete_student = true;
        }
      }

	  if (delete_student){
      db_remove(data_base, i); //ici lsize-- est execute
      i--; //!!! On fait ça car le student indice i va etre remplace par le suivant et on doit aussi le controler!!!
	  }
    }
  }
  else{
    throw invalid_argument("The data that you want to delete doesn't exist.");
  }
}

void update(string data_type,string filter_asked,string new_data_type,string new_filter,database_t* data_base){


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
