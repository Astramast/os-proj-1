#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "utils.h"
#include <signal.h>
#include <cstring>
#include "query.h"
#include "parsing.h"

void sigint_handler(int received){
	if (received == SIGINT){
		printf("Process shutting down\n");
		exit(-12);
	}
}

int identify_query(query_result_t *query){
	char* temp;
	temp = query->query;
	char* query_first_word = strtok_r(NULL, " ", &temp);
	if (strcmp(query_first_word, "insert")==0){
		return 0;
	}
	if (strcmp(query_first_word, "select")==0){
		return 1;
	}
	if (strcmp(query_first_word, "delete")==0){
		return 2;
	}
	if (strcmp(query_first_word, "update")==0){
		return 3;
	}
	return -1;
}

int main(int argc, char const *argv[]) {
	printf("STARTING\n");
    const char *db_path = argv[1];
	if (strcmp(db_path, "")==0){
		printf("No file given. Usage : ./tinydb <db_file_path>");
		exit(-1);
	}
	printf("Entered path : %s\n", db_path);
	database_t db;
    db_init(&db);
    db_load(&db, db_path);
	int sons[4];
	int insert_pipe_father[2];
	int insert_pipe_son[2];
	int select_pipe_father[2];
	int select_pipe_son[2];
	int delete_pipe_father[2];
	int delete_pipe_son[2];
	int update_pipe_father[2];
	int update_pipe_son[2];
	int* pipes[8] = {insert_pipe_father, insert_pipe_son, select_pipe_father, select_pipe_son, delete_pipe_father, delete_pipe_son, update_pipe_father, update_pipe_son};
	int my_read, my_write;
	for (int i=0; i<8; i++){
		pipe(pipes[i]);
	}
	int pid;
	for (int i=0; i<4; i++){
		pid = fork();
		if (pid !=0){
			sons[i]=pid;
			close(pipes[2*i][0]);
			close(pipes[2*i+1][1]);
		}
		else{
			close(pipes[2*i][1]);
			close(pipes[2*i+1][0]);
			my_read = pipes[2*i][0];
			my_write = pipes[2*i+1][1];
			i=4;
		}
	}
	if (pid!=0){//father
		signal(SIGINT, sigint_handler);
		char user_query[256];
		while (fgets(user_query, 256, stdin)){
				query_result_t query;
				query_result_init(&query, user_query);
				int query_number = identify_query(&query);
				if (query_number != -1){
					write(pipes[2*query_number][1], &query, sizeof(query_result_t*));
				}
				else{
					printf("E: Wrong query. Use insert, select, delete, update\n");
				}
		}

		//Ici procédure de fin de programme (à compléter)

		db_save(&db, db_path);
    	printf("Bye bye!\n");
	}

	else{//son
		query_result_t *query_ptr = NULL;
		read(my_read, query_ptr, sizeof(query_result_t*));
		printf("%s",query_ptr->query);
		char* fname,*lname,*section,*field,*value,*field_to_update,*update_value;
		fname = lname = section = field = value = field_to_update = update_value=nullptr;

		unsigned* id=nullptr;
		struct tm* birthdate=nullptr;
		database_t data_base;

		switch (identify_query(query_ptr)){
			case 0:{
				parse_insert(query_ptr->query, fname, lname, id, section, birthdate);
				student_t student={*id,{*fname,*lname,*section}};
				student.birthdate=*birthdate;
				insert(&student,&data_base);
				printf(fname," ",lname," ",id," ",section," ",birthdate);
				break;
			}
			case 1:{
				parse_selectors(query_ptr->query, field, value);
				select(field, value, &data_base, query_ptr);
				break;
			}
			case 2:{
				parse_selectors(query_ptr->query, field, value);
				delete_function(field, value, &data_base, query_ptr);
				printf(field," ",value);
				break;
			}
			case 3:{
				parse_update(query_ptr->query, field, value, field_to_update, update_value);
				update(field, value, field_to_update, update_value, &data_base, query_ptr);
				printf(field," ",value," ",field_to_update," ",update_value);
				break;
			}
		}
		
		printf("%i%i\n", my_read, my_write);
		exit(0);//ATTENTION NE PAS SUPPRIMMER CETTE LIGNE TANT QUE LE PERE NE GERE PAS LA FIN DU PROGRAMME SINON T AURAS DES TINYDB QUI RUN SUR TON PC.

	}

	//anti-crash : error: unused variable... lignes à supprimmer après implémentation
	printf("%i\n", sons[1]);
    return 0;
}
