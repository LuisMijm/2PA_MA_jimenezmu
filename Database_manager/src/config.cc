
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

#include <dirent.h>

Settings settings;


void InitSettings()
{
    
    settings.screen_window_width = 800;
    settings.screen_window_height = 800;

    settings.fps_current_time = 0.0f;
    settings.fps_last_time = 0.0f;
    settings.fps_fps = 60;

    settings.exit_program = false;
    settings.freeze_update = false;

    settings.current_database = 0;
    settings.current_table = 0;

    settings.db_connected = false;

    

    settings.querie_text[0] = '\0';
    settings.db_database_counter = 0;
    settings.db_table_count = 0;

    
    settings.type_msg = 1;
    settings.n_msg = 0;

    
    
    settings.db_table_names = (char **)realloc(settings.db_table_names, 1 * sizeof(char *));
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

void AddErrorMsg(char *err_msg)
{
    int string_size = 0;
    int err_size = strlen(err_msg);

    if(NULL == settings.console_msg[settings.n_msg])
    {
        string_size = kStringSize;
    }else {
        string_size = strlen(settings.console_msg[settings.n_msg]);
    }


    // if(settings.console_msg != nullptr){
    //     free(settings.console_msg);
    //     settings.console_msg = nullptr;
    // }

    settings.console_msg[settings.n_msg] = (char *)calloc((err_size + string_size + 3) , sizeof(char));
                                                        

    settings.type_msg = 0;
    strcat(settings.console_msg[settings.n_msg], "\n");
    strcat(settings.console_msg[settings.n_msg], err_msg);
    settings.n_msg++;
    
}

int RunQuery(char *query, sqlite3 *db, int callback(void *, int, char **, char **))
{
    int result_code;
    char* err_msg = (char*)calloc('\0', 100 * sizeof(char));


    result_code = sqlite3_exec(db, query,  callback, 0, &err_msg);
    
    printf("\n");
    printf("error: %s", err_msg);
    printf("\n");
    
    if (err_msg != nullptr)
    {
        AddErrorMsg(err_msg);
    }

    /* free(err_msg); */

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

void DataBaseNames(char* db_path)
{
    DIR *directory;
    struct dirent *entry;
    directory = opendir("../../data/internaldb/");
    settings.db_database_counter = 0; 

    //if ((directory = opendir(db_path)) != NULL)
    if (directory != NULL)
    {
        // printf("collecting names\n");

        while ((entry = readdir(directory)) != NULL)
        {
            if (entry->d_type == DT_REG && strstr(entry->d_name, ".db") != NULL)
            {
                settings.db_database_counter++;
                if ((settings.db_names = (char**)realloc(settings.db_names, settings.db_database_counter * sizeof(char*))) == nullptr)
                {
                    printf("Can't realocate memory\n");
                }
                settings.db_names[settings.db_database_counter - 1] = (char *)calloc(kStringSize, sizeof(char));
                strcpy(settings.db_names[settings.db_database_counter - 1], entry->d_name);
            }
            // printf("entry d_name: %s\n", entry->d_name);
            // printf("name: %s\n", settings.db_names[settings.db_database_counter - 1]);
        }
        closedir(directory);
    }else
    {
        printf("Can't open folder to collect db names\n");
    }

    // for(int i = 0; i < 1; i++){
        // settings.all_db_names = (char*)realloc(settings.all_db_names, (kStringSize * (i + 1)) * sizeof(char));
        // strcat(settings.all_db_names, settings.db_names[i]);
        // strcat(settings.all_db_names, "\0");
        // strcat(settings.all_db_names, settings.db_names[i+1]);
    // }
    // strcat(settings.all_db_names, "\0\0");

    // printf("%s\n", settings.all_db_names);
}


