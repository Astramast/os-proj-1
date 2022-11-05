#include <stdio.h>
#include <unistd.h>
#include "db.h"
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

size_t safe_read(int fd, void* buffer, size_t nbytes) {
    ssize_t nbytes_read = read(fd, buffer, nbytes);
    if (nbytes_read < 0) {
        perror("read error: ");
        exit(1);
    }
    return (size_t)nbytes_read;
}

size_t safe_write(int fd, const void* buffer, size_t nbytes) {
    ssize_t bytes_written = write(fd, buffer, nbytes);
    if (bytes_written < 0) {
        perror("write: ");
        exit(1);
    }
    return (size_t)bytes_written;
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
				int query_number = identify_query(query);
				if (query_number != -1){
					safe_write(pipes[2*query_number][1], &query, sizeof(query_result_t));
					safe_read(pipes[2*query_number+1][0], &query, sizeof(query_result_t));
				}
				else{
					printf("E: Wrong query. Use insert, select, delete, update\n");
				}
		}
		//Ici procédure de fin de programme (à compléter)
		//creer le fichier si necessaire pour update et select jcrois
		//suivre l exemple du pdf
		//fermer les pipes a fermer
		db_save(&db, db_path);
    	printf("Bye bye!\n");
	}

	else{//son
		query_result_t query;
		safe_read(my_read, &query, sizeof(query_result_t));
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
		if (!everything_fine){printf("Wrong query argument given. Failed.\n");}
		//faire temps query end, query debut est deja setup ds query init
		safe_write(my_write, &query, sizeof(query_result_t));

	}

	//anti-crash : error: unused variable... lignes à supprimmer après implémentation
	printf("%i\n", sons[1]);
    return 0;
}
