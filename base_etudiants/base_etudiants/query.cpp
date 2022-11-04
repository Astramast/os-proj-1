#include "query.h"
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstring>
#include "db.h"
using std::stoul;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::strcpy;
using std::invalid_argument;

void insert(student_t* student, database_t *data_base){
	for(unsigned long int i=0;i<data_base->lsize;i++ ){
		if(data_base->data[i].id==student->id){
			throw invalid_argument("The id where you want to set a new student already exist.");
		}
	}
	db_add(data_base,*student); 
	cout<<"student inserted with success"<<endl;
}

bool data_analyse(string field){

	bool is_field_found=false;
	string id="id";
	string fname="fname";
	string lname="lname";
	string section="section";
	string birthdate="birthdate";
	vector<string>student_data={id,fname,lname,section,birthdate};//creation of a list to store the data

	unsigned short int vector_index=0;
	while(is_field_found == false and vector_index<student_data.size()){
		if(field == student_data[vector_index]){
			is_field_found=true;
		}
		vector_index++;
	}
	return is_field_found;
}

vector<student_t*> select(string field , string value, database_t* data_base, query_result_t *query){

	vector<student_t*>sort_student_list;
	if(data_analyse(field)){
		for(unsigned long int i=0;i<data_base->lsize;i++){

			if(field =="id"){
				if(data_base->data[i].id == stoul(value)){
					sort_student_list.push_back(&data_base->data[i]);
					query_result_add(query, data_base->data[i]);
				}
			}

			else if(field =="fname"){
				if(data_base->data[i].fname == value){
					sort_student_list.push_back(&data_base->data[i]);
					query_result_add(query, data_base->data[i]);       
				}
			}

			else if(field =="lname"){
				if(data_base->data[i].lname == value){
					sort_student_list.push_back(&data_base->data[i]);
					query_result_add(query, data_base->data[i]);       
				}
			}

			else if(field =="section"){
				if(data_base->data[i].section == value){
					sort_student_list.push_back(&data_base->data[i]);
					query_result_add(query, data_base->data[i]);       
				}
			}

			else if(field =="birthdate"){
				char* student_bd_temp=nullptr;
				tm* student_tm = &data_base->data[i].birthdate;
				strftime(student_bd_temp, 10, "%d/%m/%Y", student_tm);
				if(student_bd_temp == value){
					sort_student_list.push_back(&data_base->data[i]);
					query_result_add(query, data_base->data[i]);       
				}
			}
		}
	}
	else{
		throw invalid_argument("The data that you want to select doesn't exist.");
	}
	return sort_student_list;
}

void delete_function(string field,string value , database_t* data_base, query_result_t *query){

	if(data_analyse(field)){
		for(unsigned long int i=0;i<data_base->lsize;i++){
			bool delete_student = false;

			if(field =="id"){
				if(data_base->data[i].id == stoul(value)){
					delete_student = true;
					query_result_add(query, data_base->data[i]);
				}
			}

			else if(field =="fname"){
				if(data_base->data[i].fname == value){
					delete_student = true;
					query_result_add(query, data_base->data[i]);
				}
			}

			else if(field =="lname"){
				if(data_base->data[i].lname == value){
					delete_student = true;
					query_result_add(query, data_base->data[i]);
				}
			}

			else if(field =="section"){
				if(data_base->data[i].section == value){
					delete_student = true;
					query_result_add(query, data_base->data[i]);
				}
			}

			else if(field =="birthdate"){
				char* student_bd_temp=nullptr;
				tm* student_tm = &data_base->data[i].birthdate;
				strftime(student_bd_temp, 10, "%d/%m/%Y", student_tm);
				if(student_bd_temp == value){
					delete_student = true;
					query_result_add(query, data_base->data[i]);
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

void update(string filter_field ,string value ,string modified_field ,char* new_value,database_t* data_base, query_result_t *query){

	vector<student_t*> temporary_student_list=select(filter_field,value,data_base, query);
	if(data_analyse(modified_field)){
		if(modified_field =="id"){
			for (size_t i=0; i<temporary_student_list.size(); i++){
				temporary_student_list[i]->id= stoul(new_value);
				query_result_add(query, *temporary_student_list[i]);
			}
		}

		else if(modified_field =="fname"){
			for(size_t i=0;i<temporary_student_list.size();i++){
				strcpy(temporary_student_list[i]->fname,new_value);
				query_result_add(query, *temporary_student_list[i]);
			}
		}

		else if(modified_field =="lname"){
			for(size_t i=0;i<temporary_student_list.size();i++){
				strcpy(temporary_student_list[i]->lname,new_value);
				query_result_add(query, *temporary_student_list[i]);
			}
		}

		else if(modified_field =="section"){
			for(size_t i=0;i<temporary_student_list.size();i++){
				strcpy(temporary_student_list[i]->section,new_value);
				query_result_add(query, *temporary_student_list[i]);
			}
		}

		else if(modified_field =="birthdate"){
			for(size_t i=0;i<temporary_student_list.size();i++){
			//convert a Date string into the struct tm
				strptime(new_value,"%d/%m/%Y",&temporary_student_list[i]->birthdate);
				query_result_add(query, *temporary_student_list[i]);
			}
		}
	}
	else{
		throw invalid_argument("The data that you want to change doesn't exist.");
	}
}

void query_result_init(query_result_t* result, const char* query) {
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	result->start_ns = now.tv_nsec + 1e9 * now.tv_sec;
	result->status = QUERY_SUCCESS;
	snprintf(result->query, 256, "%s", query);
	result->lsize = 0;
	result->psize = sizeof(student_t)*100;
	result->students = (student_t*) malloc(sizeof(student_t)*100);
}

void query_result_add(query_result_t* result, student_t s){
	if (result->lsize*sizeof(student_t) == result->psize){query_result_extend_memory(result);}
	result->students[result->lsize] = s;
	result->lsize++;
}

void query_result_extend_memory(query_result_t *res){
	student_t* temp = NULL;
	temp = new student_t[10*(res->psize)];
	memcpy(temp, res->students, sizeof(student_t)*res->lsize);
	free(res->students);
	res->students = temp;
	res->psize = 10*res->psize;
}

void query_result_log(query_result_t *query, const char* file_path){
	FILE *file = fopen(file_path, "w");
	if (file != NULL){
		fputs("Etudiants concernés", file);
		for (size_t i=0; i<query->lsize; i++){
			char temp[1000];
			student_to_str(temp, &query->students[i]);
			fputs(temp, file);
		}
		char temp[356];
		snprintf(temp, 356, "Query status : %i\n", query->status);
		fputs(temp, file);
		snprintf(temp, 356, "Query request : %s\n", query->query);
		fputs(temp, file);
		snprintf(temp, 356, "Query start time : %li\n", query->start_ns);
		fputs(temp, file);
		snprintf(temp, 356, "Query end time : %li\n", query->end_ns);
		fputs(temp, file);
		fclose(file);
	}
	else{perror(file_path);}
}
