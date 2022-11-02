#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "utils.h"

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
	//TODO
	if (pid!=0){
    	db_save(&db, db_path);
    	printf("Bye bye!\n");
		printf("Sons : ");
		for (int i=0; i<4; i++){
			printf("%i, ", sons[i]);
		}
		printf("\n");
	}
	else{
		printf("Im son ");
	}
	printf("%i\n", pid);
    return 0;
}
