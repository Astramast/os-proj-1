#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include <signal.h>
#include <cstring>
#include "query.h"
#include "parsing.h"
#include "utils.h"
#include <sys/wait.h>
#include <sys/mman.h>

bool END=false;
bool USR1=false;

void sigint_handler(int received){
	if (received == SIGINT){
		END=true;
	}
}

void sigusr1_handler(int received){
	if (received == SIGUSR1){
		USR1=true;
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
	
	size_t* shared_lsize = (size_t*) mmap(NULL, sizeof(size_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*shared_lsize = db.lsize;

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
	int my_read;
	int my_write;
	for (int i=0; i<8; i++){
		if (pipe(pipes[i])!=0){
			perror("Error during pipe(): ");
			exit(2);
		}
	}

	int pid;
	for (int i=0; i<4; i++){
		pid = fork();
		if (pid < 0){
			perror("Error during fork(): ");
			exit(1);
		}
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
	signal(SIGINT, sigint_handler);
	if (pid!=0){//father
		bool is_transaction_on = false;
		signal(SIGUSR1, sigusr1_handler);
		char user_query[256];
		while (fgets(user_query, 256, stdin)){
			if (END){
				break;
			}

			if (USR1){
				db_save(&db, db_path);
			}

			printf("%s", user_query);
			if(strcmp(user_query,"transaction")==0){
				printf("UDA");
				is_transaction_on = true;
			}

			query_result_t query;
			query_result_init(&query, user_query);

			if (is_transaction_on){
				for (int i=0; i<4; i++){
					safe_write(pipes[2*i][1], &query, sizeof(query_result_t));
				}
				for (int i=0; i<4; i++){
					int temp;
					read(pipes[2*i+1][0], &temp, sizeof(int));
				}
				is_transaction_on = false;
			}
			else{
				int query_number = identify_query(query);
				if (query_number != -1){
					safe_write(pipes[2*query_number][1], &query, sizeof(query_result_t));
					sleep(1);
				}

				else{
					printf("E: Wrong query. Use insert, select, delete, update\n");
				}
			}
		}
		//End of program procedure
		for (int i=0; i<4; i++){
			kill(sons[i], SIGINT);
			int temp=EOF;
			write(pipes[2*i][1], &temp, sizeof(int));
			close(pipes[2*i][1]);
			close(pipes[2*i+1][0]);
			int state;
			wait(&state);
		}
		db.lsize = *shared_lsize;
		db_save(&db, db_path);
    	printf("Bye bye!\n");
	}

	else{//son
		while (!END){
			query_result_t query;
			safe_read(my_read, &query, sizeof(query_result_t));
			if (!END){
				if (strcmp(query.query, "transaction")==0){
					int temp;
					safe_write(my_write, &temp, sizeof(int));
				}
				else{
					db.lsize = *shared_lsize;
					printf("Running query %s\n", query.query);
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
					printf("%i\n", query_number);
			
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
						printf("Entered if\n");
						if (parse_selectors(query_parsing, field, value)){
							printf("Entered if parse\n");
							select(field, value, &db, &query);
							printf("Out of select\n");
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
					if (!everything_fine){printf("Wrong query argument given. Failed.\n");}
					query.query[strcspn(query.query, "\n")]=0;
					printf("HOP\n");
					struct timespec now;
					clock_gettime(CLOCK_REALTIME, &now);
					query.end_ns = now.tv_nsec + 1e9 * now.tv_sec;
					log_query(&query);
					*shared_lsize = db.lsize;
					sleep(1);
				}
			}
		}
		close(my_read);
		close(my_write);
	}
    return 0;
}
