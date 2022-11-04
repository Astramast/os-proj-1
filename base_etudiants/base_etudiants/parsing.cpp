#include "parsing.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "student.h"

bool parse_update(char* query, char* field_filter, char* value_filter, char* field_to_update, char* update_value) {
    printf("%s\n",query); 
    
    char* key_val_filter = strtok_r(NULL, "e", &query);  // key=val filter
    if (key_val_filter == NULL) {
        return false;
    }

    char* key_val_update = strtok_r(NULL, "\n", &query);  // key=val updated value
    if (key_val_update == NULL) {
        return false;
    }
    printf("%s,%s\n", key_val_filter,key_val_update);
    if (parse_selectors(key_val_filter, field_filter, value_filter) == 0) {
        return false;
    }
    printf("%s,%s\n", key_val_filter,key_val_update);
    if (parse_selectors(key_val_update, field_to_update, update_value) == 0) {
        return false;
    }
    printf("%s,%s,%s,%s.\n",field_filter,value_filter,field_to_update,update_value);
    return true;
}

bool parse_insert(char* query, char* fname, char* lname, unsigned* id, char* section, struct tm* birthdate) {
    printf("%s\n",query);
    char* token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
	
	token = strtok_r(NULL, " ", &query);
	if (token == NULL) {
		return false;
	}
    strcpy(fname, token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(lname, token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    *id = (unsigned)atol(token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(section, token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    if (strptime(token, "%d/%m/%Y", birthdate) == NULL) {//transforme un string en date
        return false;
    }
    printf("%s,%s,%u,%s\n",fname,lname,*id,section);
    return true;
}

bool parse_selectors(char* query, char* field, char* value) {
    printf("%s\n",query);
    char* token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }

    token = strtok_r(NULL, "=", &query);
	if (token == NULL) {
		return false;
	}

    strcpy(field, token);
    token = strtok_r(NULL, "=", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(value, token);
    return true;
}
