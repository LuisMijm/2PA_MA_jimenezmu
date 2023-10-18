
#ifndef _CONFIG_H__
#define _CONFIG_H__ 1

#include <esat_extra/sqlite3.h>


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
    sqlite3 *db;
};

extern Settings settings;

void InitSettings();

#endif