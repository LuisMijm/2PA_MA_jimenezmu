
#include "config.h"

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