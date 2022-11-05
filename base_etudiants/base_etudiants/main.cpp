#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include <signal.h>
#include <cstring>
#include "query.h"
#include "parsing.h"
#include "utils.h"

bool END=false;

void sigint_handler(int received){
	if (received == SIGINT){
		END=true;
	}
}

int identify_query(query_result_t query){
	char* temp;
	temp = query.query;
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
	int insert_pipe[2];
	int select_pipe[2];
	int delete_pipe[2];
	int update_pipe[2];
	int* pipes[4] = {insert_pipe, select_pipe, delete_pipe, update_pipe,};
	int my_read;
	for (int i=0; i<4; i++){
		pipe(pipes[i]);
	}
	int pid;
	for (int i=0; i<4; i++){
		pid = fork();
		if (pid !=0){
			sons[i]=pid;
			close(pipes[i][0]);
		}
		else{
			close(pipes[i][1]);
			my_read = pipes[i][0];
			i=4;
		}
	}
	signal(SIGINT, sigint_handler);
	if (pid!=0){//father
		char user_query[256];
		while (fgets(user_query, 256, stdin)){
			if (END){
				break;
			}

			query_result_t query;
			query_result_init(&query, user_query);
			int query_number = identify_query(query);
			if (query_number != -1){
				printf("Query is %i, %s", pipes[query_number][1], query.query);
				safe_write(pipes[query_number][1], &query, sizeof(query_result_t));
				sleep(1);
			}
			else{
				printf("E: Wrong query. Use insert, select, delete, update\n");
			}
		}
		//End of program procedure
		for (int i=0; i<4; i++){
			kill(sons[i], SIGINT);
			int temp=EOF;
			write(pipes[i][1], &temp, sizeof(int));
			close(pipes[i][1]);
		}
		db_save(&db, db_path);
    	printf("Bye bye!\n");
	}

	else{//son
		while (!END){
			query_result_t query;
			safe_read(my_read, &query, sizeof(query_result_t));
			printf("Running query '%i,%s'\n", my_read, query.query);
			char query_parsing[256];
			strcpy(query_parsing, query.query);
			char fname[64];
			char lname[64];
			char section[64];
			char field[256];
			char value[256];
			char field_to_update[256];
			char update_value[256];
	
			unsigned id;
			struct tm birthdate;
			int query_number=identify_query(query);
			bool everything_fine=true;
	
			if (query_number==0){
				if (parse_insert(query_parsing, fname, lname, &id, section, &birthdate)){
					student_t student;
					student.id = id;
					strcpy(student.fname, fname);
					strcpy(student.lname, lname);
					strcpy(student.section, section);
					student.birthdate=birthdate;
					insert(&student, &db,&query);
				}
				else {everything_fine = false;}
			}
			else if (query_number==1){
				if (parse_selectors(query_parsing, field, value)){
					select(field, value, &db, &query);
				}
				else{everything_fine = false;}
			}
			else if (query_number==2){
				if (parse_selectors(query_parsing, field, value)){
					delete_function(field, value, &db, &query);
				}
				else{everything_fine = false;}
			}
			else if (query_number==3){
				if (parse_update(query_parsing, field, value, field_to_update, update_value)){
					update(field, value, field_to_update, update_value, &db, &query);
				}
				else{everything_fine = false;}
			}
			else{everything_fine=false;}
			if ((!everything_fine) && (!END)){printf("Wrong query argument given. Failed.\n");}
			query.query[strcspn(query.query, "\n")]=0;
			struct timespec now;
			clock_gettime(CLOCK_REALTIME, &now);
			query.end_ns = now.tv_nsec + 1e9 * now.tv_sec;
			log_query(&query);
			sleep(1);
		}
	}
    return 0;
}
