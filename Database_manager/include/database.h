/**
 * @file database.h
 * @author Luis Miguel Jiménez
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _DATABASE_H__
#define _DATABASE_H__ 1

/**
 * @brief callback function to retrieve data from a database
 * 
 * @param not_used 
 * @param argc 
 * @param argv 
 * @param azcolname 
 * @return int 
 */
int GetDataFromDB(void *not_used, int argc, char **argv, char **azcolname);

/**
 * @brief function to free memory of the pointer that holds the table data
 * 
 * @param cols number of columns
 * @param rows number of rows
 */
void FreeTable(int cols, int rows);


#endif