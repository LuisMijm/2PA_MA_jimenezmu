
/**
 * @file config.cc
 * @author Luis Miguel Jiménez
 * @brief File for program configuration procedures and data
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Settings settings;

void InitSettings()
{
    
    settings.screen_window_height = 100;
    settings.screen_window_width = 100;

    settings.fps_current_time = 0.0f;
    settings.fps_last_time = 0.0f;
    settings.fps_fps = 60;

    settings.exit_program = false;
    settings.freeze_update = false;

    settings.current_database = 0;
    settings.current_table = 0;

    settings.querie_text[0] = '\0';

}


int ConnectToDB(char* db_path, sqlite3** db, int* result_code)
{
    *result_code = sqlite3_open(db_path, db);

    if (*result_code != SQLITE_OK)
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return 1;
    }else{
        printf("Database open\n");
        return 0;
    }
}

// int RunSettingsQuery(char *query, sqlite3 *db)
// {
//     int result_code;
//     char* err_msg = 0;

//     result_code = sqlite3_exec(db, query, SetSettings, 0, &err_msg);

//     return result_code;
// }

int RunQuery(char *query, sqlite3 *db, int callback(void *, int, char **, char **))
{
    int result_code;
    char* err_msg = 0;

    // settings.db_Rows[0] = sqlite3_column_count();

    result_code = sqlite3_exec(db, query,  callback, 0, &err_msg);

    return result_code;
}

int SetSettings(void* not_used, int argc, char** argv, char** azcolname)
{
    not_used = 0;

    if (!strcmp(argv[1], "screen_window_height"))
    {
        sscanf(argv[2], "%d", &settings.screen_window_height);
        // printf("%d\n", settings.screen_window_height);
    }
    else if (!strcmp(argv[1], "screen_window_width"))
    {
        sscanf(argv[2], "%d", &settings.screen_window_width);
        // printf("%d\n", settings.screen_window_width);
    }

    return 0;
}