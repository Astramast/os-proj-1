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
	//int pipes[8];
	int pid = 1;
	for (int i=0; i<4; i++){
		pid = fork();
		if (pid !=0){
			sons[i]=pid;
		}
		else{
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
