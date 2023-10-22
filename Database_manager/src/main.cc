

#include "include.h"

int esat::main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));

    InitSettings();

    ConnectToDB("./data/internal_db/internaldb.db", &settings.db_internal, &settings.db_result_code);
    settings.db_result_code = RunQuery("SELECT * FROM settings", settings.db_internal, SetSettings);

    esat::WindowInit(settings.screen_window_width, settings.screen_window_height);

    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape) && !settings.exit_program)
    {
        settings.fps_last_time = esat::Time();
        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);

        

        do
        {
            settings.fps_current_time = esat::Time();
        } while ((settings.fps_current_time - settings.fps_last_time) <= 1000.0 / settings.fps_fps);
        esat::WindowFrame();
    }

    sqlite3_close(settings.db_internal);

    esat::WindowDestroy();
    return 0;
}