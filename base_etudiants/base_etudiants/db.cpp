#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include "student.h"


void *create_shared_memory(size_t size) {
	//size is the number of bytes allocate to the mmap
	const int protection = PROT_READ | PROT_WRITE;

	// The buffer will be shared (meaning other processes can access it), but
	// anonymous (meaning third-party processes cannot obtain an address for it),
	// so only this process and its children will be able to use it:
	const int visibility = MAP_SHARED | MAP_ANONYMOUS;

	return mmap(NULL, size, protection, visibility, -1, 0);
}

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
	printf("Entered db_load\n");
    FILE *file = fopen(path, "rb");//cause du bug
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
	printf("Entered db_init\n");
	db->data = (student_t*) create_shared_memory(sizeof(student_t)*100);
	if (db->data == NULL){
		perror("DB's size too large for memory-chan TwT'");
	}
}

void db_add(database_t *db, student_t student) {
	if (db->lsize*sizeof(student_t) == db->psize){db_extend_memory(db);}
	db->data[db->lsize] = student;
	db->lsize++;
}

void db_extend_memory(database_t *db){
	printf("test");
	student_t* old_value = db->data;
	size_t old_psize = db->psize;
	db->psize = db->psize * 2;
	db->data = (student_t*)mmap(NULL, 10*(db->psize), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	memcpy(db->data, old_value, old_psize*sizeof(student_t));
	munmap(old_value, sizeof(old_value)*old_psize);

}

void db_remove(database_t* db, int indice){
	for (size_t i = indice+1; i<=db->lsize; i++){
		db->data[i-1] = db->data[i];
	}
	db->lsize--;
}
