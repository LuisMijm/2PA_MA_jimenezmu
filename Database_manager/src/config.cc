
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

#include <time.h>

Settings settings;

enum WindowSetting
{
    kWindowSetting_DataBaseSelected,
    kWindowSetting_TableSelected,
    kWindowSetting_TableWindow,
    kWindowSetting_ConsoleWindow,
    kWindowSetting_QueryWindow,
    kWindowSetting_SettingsWindow
};


void InitSettings()
{

    settings.windowSettings[kWindowSetting_DataBaseSelected].size = {380.0f, 180.0f};
    settings.windowSettings[kWindowSetting_DataBaseSelected].position = {20.0f, 20.0f};

    settings.windowSettings[kWindowSetting_TableSelected].size = {380.0f, 180.0f};
    settings.windowSettings[kWindowSetting_TableSelected].position = {20.0f, 220.0f};

    settings.windowSettings[kWindowSetting_TableWindow].size = {780.0f, 380.0f};
    settings.windowSettings[kWindowSetting_TableWindow].position = {420.0f, 20.0f};

    settings.windowSettings[kWindowSetting_ConsoleWindow].size = {1160.0f, 170.0f};
    settings.windowSettings[kWindowSetting_ConsoleWindow].position = {20.0f, 420.0f};

   settings.windowSettings[kWindowSetting_QueryWindow].size =  {1160.0f, 170.0f};
   settings.windowSettings[kWindowSetting_QueryWindow].position = {20.0f, 610.0f};

    
    settings.screen_window_width = 1200;
    settings.screen_window_height = 800;

    settings.fps_current_time = 0.0f;
    settings.fps_last_time = 0.0f;
    settings.fps_fps = 60;

    settings.exit_program = false;
    settings.freeze_update = false;

    settings.current_database = 0;
    settings.current_table = 0;

    settings.db_connected = false;

    settings.visual_connected = false;

    settings.querie_text[0] = '\0';
    settings.db_database_counter = 0;
    settings.db_table_count = 0;

    
    
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
        // printf("Database open\n");
        
        return 0;
    }
}

void AddErrorMsg(char *err_msg)
{

    time_t current_hour;
    time(&current_hour);

    struct tm *infHour;
    infHour = localtime(&current_hour);
    char buffer_time[80];
    strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

    int string_size = 0;
    int err_size = strlen(err_msg) + strlen(buffer_time);

    if(NULL == settings.console_msg[settings.n_msg].string )
    {
        string_size = kStringSize;
    }else {
        string_size = strlen(settings.console_msg[settings.n_msg].string );
    }


    settings.console_msg[settings.n_msg].string = (char *)calloc((err_size + string_size + 3) , sizeof(char));
                                                        

    settings.console_msg[settings.n_msg].type= 0;
    strcat(settings.console_msg[settings.n_msg].string , "\n");
    strcat(settings.console_msg[settings.n_msg].string, buffer_time);

    strcat(settings.console_msg[settings.n_msg].string , err_msg);
    settings.n_msg++;
    
}

int RunQuery(char *query, sqlite3 *db, int callback(void *, int, char **, char **), int* count)
{
    int result_code;
    char *err_msg = (char *)calloc('\0', 100 * sizeof(char));

    result_code = sqlite3_exec(db, query, callback, (void*)count, &err_msg);

    // printf("\nerror: %s\n", err_msg);

    if (err_msg != nullptr)
    {
        AddErrorMsg(err_msg);
    }

    free(err_msg);

    return result_code;
}


int RunQuery(char *query, sqlite3 *db, int callback(void *, int, char **, char **))
{
    int result_code;
    char* err_msg = (char*)calloc('\0', 100 * sizeof(char));

    result_code = sqlite3_exec(db, query,  callback, 0, &err_msg);

    // printf("\nerror: %s\n", err_msg);

    if (err_msg != nullptr)
    {
        AddErrorMsg(err_msg);
    }

    free(err_msg);

    return result_code;
}


int SetSettings(void* not_used, int argc, char** argv, char** azcolname)
{
    not_used = 0;

    if (!strcmp(argv[1], "screen_window_height"))
    {
        sscanf(argv[2], "%d", &settings.screen_window_height);
    }
    else if (!strcmp(argv[1], "screen_window_width"))
    {
        sscanf(argv[2], "%d", &settings.screen_window_width);
    }
    else if(!strcmp(argv[1], "fps_fps"))
    {
        sscanf(argv[2], "%c", &settings.fps_fps);
    }
    else if(!strcmp(argv[1], "window_selectdb_size_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_DataBaseSelected].size.x);
    }
    else if (!strcmp(argv[1], "window_selectdb_size_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_DataBaseSelected].size.y);
    }
    else if (!strcmp(argv[1], "window_selectdb_pos_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_DataBaseSelected].position.x);
    }
    else if (!strcmp(argv[1], "window_selectdb_pos_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_DataBaseSelected].position.y);
    }
    else if(!strcmp(argv[1], "window_selectTable_size_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_TableSelected].size.x);
    }
    else if (!strcmp(argv[1], "window_selectTable_size_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_TableSelected].size.y);
    }
    else if (!strcmp(argv[1], "window_selectTable_pos_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_TableSelected].position.x);
    }
    else if (!strcmp(argv[1], "window_selectTable_pos_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_TableSelected].position.y);
    }
    else if(!strcmp(argv[1], "window_table_size_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_TableWindow].size.x);
    }
    else if (!strcmp(argv[1], "window_table_size_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_TableWindow].size.y);
    }
    else if (!strcmp(argv[1], "window_table_pos_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_TableWindow].position.x);
    }
    else if (!strcmp(argv[1], "window_table_pos_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_TableWindow].position.y);
    }
    else if(!strcmp(argv[1], "window_console_size_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_ConsoleWindow].size.x);
    }
    else if (!strcmp(argv[1], "window_console_size_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_ConsoleWindow].size.y);
    }
    else if (!strcmp(argv[1], "window_console_pos_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_ConsoleWindow].position.x);
    }
    else if (!strcmp(argv[1], "window_console_pos_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_ConsoleWindow].position.y);
    }
    else if(!strcmp(argv[1], "window_query_size_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_QueryWindow].size.x);
    }
    else if (!strcmp(argv[1], "window_query_size_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_QueryWindow].size.y);
    }
    else if (!strcmp(argv[1], "window_query_pos_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_QueryWindow].position.x);
    }
    else if (!strcmp(argv[1], "window_query_pos_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_QueryWindow].position.y);
    }
    else if(!strcmp(argv[1], "window_settings_size_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_SettingsWindow].size.x);
    }
    else if (!strcmp(argv[1], "window_settings_size_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_SettingsWindow].size.y);
    }
    else if (!strcmp(argv[1], "window_settings_pos_x"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_SettingsWindow].position.x);
    }
    else if (!strcmp(argv[1], "window_settings_pos_y"))
    {
        sscanf(argv[2], "%f", &settings.windowSettings[kWindowSetting_SettingsWindow].position.y);
    }
    // else if(!strcmp(argv[1], ""))
    // {
    //     sscanf(argv[2], "%", &settings.);
    // }

    return 0;
}

void DataBaseNames(char* db_path)
{
    DIR *directory;
    struct dirent *entry;
    directory = opendir("../../data/databases/");
    settings.db_database_counter = 0; 

    if (directory != NULL)
    {
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
        }
        closedir(directory);
    }else
    {
        printf("Can't open folder to collect db names\n");
    }
}




