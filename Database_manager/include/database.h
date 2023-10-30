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
 * @brief Resets and frees memory associated with a database table.
 *
 * This function resets and frees memory associated with a database table, including
 * column names and data rows. It is used to clear the existing table data and prepare
 * for further data retrieval or processing.
 *
 * @return Returns 0 to indicate a successful reset.
 */
int ResetTable();

/**
 * @brief Retrieves tables from the database and stores them in a data structure.
 *
 * This function is used to obtain table names from a database and store them
 * in a data structure. The table names are extracted from the `argv` array and
 * stored in the `settings.db_table_names` structure.
 *
 * @param not_used An unused pointer (can be NULL).
 * @param argc The number of arguments in the `argv` array.
 * @param argv An array of strings containing the names of database tables.
 * @param azcolname An array of strings containing column names (not used here).
 * @return Returns 0 if the operation is successful.
 */
int GetTablesFromDB(void *not_used, int argc, char **argv, char **azcolname);

/**
 * @brief Retrieves data from a database query and stores it in data structures.
 *
 * This function is used to retrieve data from a database query and store it in
 * data structures. It handles both column names and the actual data rows. Column
 * names are stored in the `settings.db_ColNames` array, and data rows are stored in
 * the `settings.db_table_info` structure.
 *
 * @param not_used An unused pointer (can be NULL).
 * @param argc The number of columns in the query result.
 * @param argv An array of strings containing the data for each column.
 * @param azcolname An array of strings containing the column names.
 * @return Returns 0 if the operation is successful.
 */
int GetDataFromDB(void *not_used, int argc, char **argv, char **azcolname);

/**
*/
// void InsertDataLine(int cols);

/**
*/
void UpdateData(int row, int cols);

/**
 * @brief Frees memory associated with database table data.
 *
 * This function is responsible for freeing memory associated with database table data,
 * including column names and data rows. It should be used to clean up allocated memory
 * after data processing is complete.
 *
 * @param cols The number of columns in the table.
 * @param rows The number of rows in the table.
 */
void FreeTable(int cols, int rows);


#endif