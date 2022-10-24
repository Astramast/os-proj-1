#ifndef _QUERY_H
#define _QUERY_H

#include "db.h"
#include <vector>
using std::string;
using std::vector;

typedef enum { QUERY_SUCCESS, QUERY_FAILURE, UNRECOGNISED_FIELD } QUERY_STATUS;

/**
 * A query_result_t describes the result of a query.
 */
typedef struct {
  student_t* students; /** A list of students **/
  size_t lsize;        /** Logical size **/
  size_t psize;        /** Physical size **/
  QUERY_STATUS status; /** The return status of the query **/
  char query[256];     /** The actual query that was submitted **/
  long start_ns;       /** The start of the query in nanoseconds **/
  long end_ns;         /** The end of the query in nanoseconds **/
} query_result_t;


void insert(student_t* student, database_t* data_base);
/**
 * @brief: add a student in the data base if the id doesn't already exist
 * 
 * @param student: a student
 * @param data_base: the data base
 */

bool data_analyse(string data_filter);
/**
 * @brief: check if the data are correctly written and if the data can be used. 
 * 
 * @param data_filter: the data field(example: fname or id)
 * @return bool: return if the data is correct or no
 */

vector<student_t*> select(string data_filter,string filter_asked, database_t* data_base);
/**
 * @brief: browse all the students of the data base and add them in a vector if they match with the filter asked.
 * @return: return a vector with all the students sorted by the filter. 
 */

void delete_function(string data_filter,string filter_asked,database_t* data_base);
/**
 * @brief: browse all the students of the data base and remove them if they match with the filter asked.
 */

void update(string data_filter,string filter_asked,string set_data,string set_new_info,database_t* data_base);
/**
 * @brief Update the data of all the students matching the filter, data_filter=filter_asked and replace the data of set_data by the new data
 * 
 * @param data_filter: the filter in terms of the data type(example: fname)
 * @param filter_asked: the filter in terms of the student information to process(example: Quentin)
 * @param set_data: the type of data that the user want to update(example: id).
 * @param set_new_info: The new information of the student for the set_data(example: 83)
 * 
 */
void query_result_init(query_result_t* result, const char* query);
/**
 * @brief: Initialise a query_result_t structure. 
 * @param
 *
 * 
 */

void query_result_add(query_result_t* result, student_t s);
/**
 * @brief: Add a student to a query result.
 * 
 */
#endif