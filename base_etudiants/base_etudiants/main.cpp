#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "utils.h"

bool FLAG = true;

void sigint_handler(int received){
	if (received == SIGINT){
		printf("Process shutting down");
		FLAG = false;
	}
}


int main(int argc, char const *argv[]) {
	printf("STARTING\n");
    const char *db_path = argv[1];
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
	for (int i=0; i<8; i++){
		pipe(pipes[i]);
	}
	int pid;
	for (int i=0; i<4; i++){
		pid = fork();
		if (pid !=0){
			sons[i]=pid;
			close(pipes[2*i][0];
			close(pipes[2*i+1][1];
		}
		else{
			close(pipes[2*i][1]);
			close(pipes[2*i+1][0]);
			int my_read = pipes[2*i][0];
			int my_write = pipes[2*i+1][1];
			i=4;
		}
	}
	if (pid!=0){//father
		signal(SIGINT, sigint_handler);
		char user_query[256];
		while (FLAG && ((fscanf(stdin, "%s", user_query))==1)){
			printf("%s\n", user_query);
		}
			

		//Ici procédure de fin de programme (à compléter)
		//
		//
		db_save(&db, db_path);
    	printf("Bye bye!\n");
		}
	}
	else{//son
		printf("Im son ");
	}
	printf("%i\n", pid);
    return 0;
}
