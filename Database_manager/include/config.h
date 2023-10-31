
/**
 * @file config.h
 * @author Luis Miguel Jiménez
 * @brief Header file for the program configuration procedures and data
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _CONFIG_H__
#define _CONFIG_H__ 1

#include <esat_extra/sqlite3.h>

int const kStringSize = 100;

/**
 * @struct Vec2
 * @brief Represents a two-dimensional vector.
 *
 * The `Vec2` struct defines a two-dimensional vector with `x` and `y` components.
 */
struct Vec2{
    float x, y;
};

/**
 * @struct WindowSettings
 * @brief Represents settings for the ImGui window.
 *
 * The `WindowSettings` struct holds configuration options for the ImGui 
 * graphical window, including the size and position of the window.
 */
struct WindowSettings{
    Vec2 size, position;
};

struct ConsoleMessage{
    char* string;
    int type;
};

/**
 * @brief struct to save all the data that the program needs
 * 
 */
struct Settings{

    // Window
    int screen_window_width;
    int screen_window_height;
    
    // Time
    double fps_current_time;
    double fps_last_time;
    unsigned char fps_fps;

    // Flow Control
    bool exit_program;
    bool freeze_update;

    // internal database
    int db_result_code;
    sqlite3 *db_internal;

    // Data base info
    int db_Rows;
    int db_Cols;
    int db_database_counter;
    int db_table_count;
    bool db_connected;
    sqlite3 *db_current;
    char** db_names;
    char** db_table_names;
    char** db_ColNames;
    char** db_ColTypes;
    char*** db_table_info;
    char*** db_table_info_back;
    char* all_db_names;

    // selected database & table
    int current_database;
    int current_table;


    // ImGui windows settings
    WindowSettings windowSettings[5];

    // Input text Window Querie
    char querie_text[512];

    //Message Console
    ConsoleMessage console_msg[100];
    int type_msg;
    int n_msg;
};

extern Settings settings;


/**
 * @brief Procedure to initialize the global variables of the program 
 * 
 */
void InitSettings();

/**
 * @brief Connects to an SQLite database.
 *
 * This function establishes a connection to an SQLite database located at the 
 * specified path.
 *
 * @param db_path The path to the SQLite database to connect to.
 * @param db A pointer to an SQLite database handle.
 * @param result_code A pointer to store the result code of the connection attempt.
 * @return Returns 0 if the connection is successful, 1 on failure.
 */
int ConnectToDB(char *db_path, sqlite3 **db, int *result_code);

/**
 * @brief Adds an error message to a collection of messages.
 *
 * This function is used to add an error message to a collection of messages.
 * It allocates memory for the error message and appends it to the existing messages.
 *
 * @param err_msg The error message to add to the collection.
 */

void AddErrorMsg(char *err_msg);

/**
 * @brief Executes an SQL query on an SQLite database.
 *
 * This function executes the specified SQL query on the provided SQLite database
 * and invokes a callback function for result processing. It also handles any potential
 * error messages and stores them in the collection of messages.
 *
 * @param query The SQL query to execute.
 * @param db A pointer to the SQLite database.
 * @param callback A callback function for processing query results.
 * @return Returns the result code of the query execution.
 */
int RunQuery(char *query, sqlite3 *db, int callback(void *, int, char **, char **));

/**
 * @brief Sets application settings based on database query results.
 *
 * This function is used to set application settings based on data retrieved from a
 * database query. It parses the query results and updates the settings accordingly.
 *
 * @param not_used An unused pointer (can be NULL).
 * @param argc The number of columns in the query result.
 * @param argv An array of strings containing the query result data.
 * @param azcolname An array of strings containing column names (not used here).
 * @return Returns 0 to indicate a successful update of settings.
 */
int SetSettings(void *not_used, int argc, char **argv, char **azcolname);

/**
 * @brief Collects names of database files in a specified directory.
 *
 * This function scans a directory for database files with the ".db" extension and collects
 * their names, storing them in the `settings.db_names` array. The total count of collected
 * database names is stored in `settings.db_database_counter`.
 *
 * @param db_path The path to the directory containing the database files.
 */
void DataBaseNames(char *db_path);

#endif