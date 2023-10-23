
/**
 * @file config.h
 * @author Luis Miguel Jiménez
<<<<<<< Updated upstream
 * @brief Header file for the program configuration procedures and data
=======
 * @brief Header file for the configuration procedures and data
>>>>>>> Stashed changes
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
    // int db_Current_Col_Rows[2];
    sqlite3 *db_current;
    char** db_names;
    char** db_ColNames;
    char*** db_table_info;
};

extern Settings settings;

void InitSettings();

int ConnectToDB(char *db_path, sqlite3 **db, int *result_code);
int RunQuery(char *query, sqlite3 *db, int callback(void *, int, char **, char **));
int SetSettings(void *not_used, int argc, char **argv, char **azcolname);
void DataBaseNames(char *db_path);

#endif