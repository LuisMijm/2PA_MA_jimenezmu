
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

int const kStringSize = 16;

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

    // selected database & table
    int current_database;
    int current_table;

    //Input text Window Querie
    char querie_text[512];


    // Data base info
    int db_Rows;
    int db_Cols;
    int db_database_counter;
    int db_table_count;

    // int db_Current_Col_Rows[2];
    sqlite3 *db_current;
    char** db_names;
    char** db_table_names;
    char** db_ColNames;
    char*** db_table_info;
    char* all_db_names;

    //Message Console
    char* console_msg[100];
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
 * @brief 
 * 
 */
void GetTablesFromDB();


/**
 * @brief function to connect to a database
 * 
 * @param db_path path of the file where the db is
 * @param db variable sqlite that holds the db in the program   
 * @param result_code error code resulting of making the query
 * @return int 
 */
int ConnectToDB(char *db_path, sqlite3 **db, int *result_code);



void AddErrorMsg(char *err_msg);

/**
 * @brief function that makes a query in the db selected using the callback given
 * 
 * @param query string cotaining the query itself
 * @param db database on wich we will make the query
 * @param callback function to execute the query 
 * @return int 
 */
int RunQuery(char *query, sqlite3 *db, int callback(void *, int, char **, char **));

/**
 * @brief Callback function to stablish the program settings from a database
 * 
 * @param not_used 
 * @param argc 
 * @param argv 
 * @param azcolname 
 * @return int 
 */
int SetSettings(void *not_used, int argc, char **argv, char **azcolname);

/**
 * @brief Procedure to get the names of the databases that we can select
 * 
 * @param db_path file path where the databases are
 */
void DataBaseNames(char *db_path);

#endif