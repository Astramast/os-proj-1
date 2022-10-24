#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "student.h"

void db_save(database_t *db, const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) {
        perror("Could not open the DB file");
        exit(1);
    }
    if (fwrite(db->data, sizeof(student_t), db->lsize, f) < 0) {
        perror("Could not write in the DB file");
        exit(1);
    }
    fclose(f);
}

void db_load(database_t *db, const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Could not open the DB file");
        exit(1);
    }
    student_t student;
    while (fread(&student, sizeof(student_t), 1, file)) {
        db_add(db, student);
    }
    fclose(file);
}

void db_init(database_t *db) {
	db->data = (student_t*) malloc(sizeof(student_t)*10000);
	if (db->data == NULL){
		printf("DB's size too large TwT'");
	}
	db->psize = 10000*sizeof(student_t);
	db->lsize = 0;
}

void db_add(database_t *db, student_t student) {
	if (db->lsize*sizeof(student_t) == db->psize){db_extend_memory(db);}
	db->data[db->lsize] = student;
	db->lsize++;
}

void db_extend_memory(database_t *db){
	student_t* temp = NULL;
	temp = new student_t[2*(db->psize)];
	memcpy(temp, db->data, sizeof(student_t)*db->lsize);
	free(db->data);
	db->data = temp;
	db->psize = 2*db->psize;
}

void db_remove(database_t* db, int indice){
	for (size_t i = indice+1; i<=db->lsize; i++){
		db->data[i-1] = db->data[i];
	}
	db->lsize--;
}
