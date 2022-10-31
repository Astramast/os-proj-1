#include <stdio.h>

#include "db.h"
#include "utils.h"

int main(int argc, char const *argv[]) {
	printf("STARTING\n");
    const char *db_path = argv[1];
	printf("Entered path : %s\n", db_path);
	database_t db;
    db_init(&db);
    db_load(&db, db_path);
    for (size_t i=0; i<db.lsize; i++){
		char b[1000];
		student_to_str(b, &db.data[i]);
		printf("%s\n", b);
	}
    db_save(&db, db_path);
    printf("Bye bye!\n");
    return 0;
}
