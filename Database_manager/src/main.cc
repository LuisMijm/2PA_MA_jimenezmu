

#include "include.h"

int esat::main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));


    esat::WindowInit(g_settings.screen_window_width, g_settings.screen_window_height);

    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape) && !g_settings.exit_game)
    {
        g_settings.fps_last_time = esat::Time();
        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);


        do
        {
            g_settings.fps_current_time = esat::Time();
        } while ((g_settings.fps_current_time - g_settings.fps_last_time) <= 1000.0 / g_settings.fps_fps);
        esat::WindowFrame();
    }


    esat::WindowDestroy();
    return 0;
}