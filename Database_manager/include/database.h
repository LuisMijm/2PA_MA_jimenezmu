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
 * @brief
 * This function assesses the given data and its type to determine how to process it. The function examines the data type provided and performs specific actions based on that type. (NOT implemented)
 *
 * @param data
 * @param type
 */
void AssesData(char *data, char *type);

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
 * @brief Callback function to obtain column types from a database table.
 *
 * This callback function is used to obtain column types from a database table. It is typically called within a database query and processes the results to populate an array with column types.
 *
 * @param used A pointer to an integer used to track the count of processed columns.
 * @param argc The number of columns in the query result.
 * @param argv An array of strings representing the column data.
 * @param azcolname An array of column names (not used in this function).
 *
 * @return Always returns 0.
 *
 */
int GetTableTypes(void *not_used, int argc, char **argv, char **azcolname);


/**
*/
int GetQueryFromDB(void *not_used, int argc, char **argv, char **azcolname);

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
 * @brief Check and modify a string to ensure proper handling of single quotes.
 *
 * This function checks a given string and modifies it to ensure proper handling of single quotes (apostrophes). It replaces any single quotes with double single quotes to prevent SQL injection and related issues.
 *
 * @param query The query or string where single quotes need to be checked and modified.
 * @param value The value to be checked and processed for single quotes.
 *
 */
void CheckString(char *query, char *value);

/**
 * @brief Delete data from a database table based on specified conditions.
 *
 * This function constructs a SQL DELETE statement to remove data from a database table based on specified conditions. It creates the appropriate query string and then executes the query to delete data from the table.
 *
 * @param row The row index indicating which row of data to delete.
 * @param cols The number of columns in the table.
 *
 */
void DeleteDataCol(int row, int cols);

/**
 * @brief Update data in a database table based on specified conditions.
 *
 * This function constructs a SQL UPDATE statement to modify data in a database table based on specified conditions. It creates the appropriate query string and then executes the query to update the table with new data.
 *
 * @param row The row index indicating which row of data to update.
 * @param cols The number of columns in the table.
 *
 */
void UpdateData(int row, int cols);

/**
 * @brief Free memory allocated for a database table and related data.
 *
 * This function releases memory allocated for a database table and related data structures. It ensures that all dynamically allocated memory is properly deallocated to prevent memory leaks.
 *
 * @param cols The number of columns in the table.
 * @param rows The number of rows in the table.
 * @param table A pointer to a pointer to a two-dimensional array representing the table data.
 *
 */
void FreeTable(int cols, int rows, char ***table);

/**
 * @brief Free all allocated resources and clean up the database-related data.
 *
 * This function performs cleanup by freeing all allocated resources and memory used for managing database-related data, including table information, database connections, and related arrays. It ensures that all dynamically allocated memory is properly deallocated to prevent memory leaks.
 *
 */
void GeneralFree();


#endif