/**
 * @file window.cc
 * @author Carlos Mazcunan <mazcunyanbla@esat-alumni.com>
 * @brief This file implements functions for the HUD and the ImGui menus.
 * @version 0.1
 * @date 2023-10-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <include.h>
#include <database.h>    
#include <ImGuiWindow.h>


ImVec2 Vec2toImVec2(Vec2 vector){
    ImVec2 im_vector;
    im_vector.x = vector.x;
    im_vector.y = vector.y;
    return im_vector;
}

void ConsoleMessage(int type, int i)
{
    /* time_t current_hour;
    time(&current_hour);

    struct tm *infHour;
    infHour = localtime(&current_hour);
    char buffer[80];
    strftime(buffer, 80, "[%H:%M:%S]", infHour);
    ImGui::Text(buffer); */

    if (settings.console_msg[i].string != NULL)
    {
            float r = (float)(rand() % 100)/100;
            float g = (float)(rand() % 100) / 100;
            float b = (float)(rand() % 100) / 100;
        switch (type)
        {
        //error messages type (red)
        case 0:
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), settings.console_msg[i].string);
            break;
        //connect/disconnect db message type (white)
        case 1:
            

            ImGui::Text(settings.console_msg[i].string);
            break;
        //show table message (green)
        case 2:
            
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), settings.console_msg[i].string);
            break;
        case 3:
            ImGui::TextColored(ImVec4(r, g, b, 1.0f), settings.console_msg[i].string);
            break;
        default:
            break;
        }
    }
    if(settings.n_msg == 99)
    {
        settings.n_msg = 0;
    }
}

void DataBaseSelectedWindow()
{    

    ImGui::SetNextWindowSize(ImVec2(Vec2toImVec2(settings.windowSettings[0].size)));
    ImGui::SetNextWindowPos(ImVec2(Vec2toImVec2(settings.windowSettings[0].position)));
    ImGui::Begin("Select DataBase:", nullptr ,
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);

    ImGui::Combo("", &settings.current_database, settings.db_names, settings.db_database_counter);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    if(ImGui::Button("Connect")){
        char db_path[100] = "../../data/databases/";

        ResetTable();
        settings.db_table_count = 0;

        strcat(db_path, settings.db_names[settings.current_database]);
        // printf("%s\n", db_path);
        ConnectToDB(db_path, &settings.db_current, &settings.db_result_code);
        RunQuery("SELECT name FROM sqlite_master WHERE type = 'table' AND name NOT LIKE 'sqlite%'",
                 settings.db_current, GetTablesFromDB);

        settings.db_connected = false;

        /* printf("Connected to database %s\n", settings.db_names[settings.current_database]); */
        // For show console message
        time_t current_hour;
        time(&current_hour);

        struct tm *infHour;
        infHour = localtime(&current_hour);
        char buffer_time[80];
        strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

        settings.console_msg[settings.n_msg].type = 1;
        int msg_size = strlen("Connected to database ") + strlen(settings.db_names[settings.current_database]) + strlen(buffer_time);
        settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

        strcat(settings.console_msg[settings.n_msg].string, "\n");
        strcat(settings.console_msg[settings.n_msg].string, buffer_time);
        strcat(settings.console_msg[settings.n_msg].string, "Connected to database ");
        strcat(settings.console_msg[settings.n_msg].string, settings.db_names[settings.current_database]);
        settings.n_msg++;
        settings.visual_connected = true;

    }
    ImGui::SameLine();
    
    if(ImGui::Button("Disconnect")){
        ResetTable();
        settings.db_table_count = 0;

        sqlite3_close(settings.db_current);
        settings.db_connected = false;

        // printf("Disconected from database %s\n", settings.db_names[settings.current_database]);
        /* printf("Disconected from database %s\n", settings.db_names[settings.current_database]); */

        //For show console message
        time_t current_hour;
        time(&current_hour);

        struct tm *infHour;
        infHour = localtime(&current_hour);
        char buffer_time[80];
        strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

        settings.console_msg[settings.n_msg].type = 1;
        int msg_size = strlen("Disconected from database ") + strlen(settings.db_names[settings.current_database])  + strlen(buffer_time);
        settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

        strcat(settings.console_msg[settings.n_msg].string, "\n");
        strcat(settings.console_msg[settings.n_msg].string, buffer_time);
        strcat(settings.console_msg[settings.n_msg].string, "Disconected from database ");
        strcat(settings.console_msg[settings.n_msg].string, settings.db_names[settings.current_database]);
        settings.n_msg++;
        settings.visual_connected = false;

    }
    
    

    ImGui::End();
}

void TableSelectedWindow(int selected_database)
{
    ImGui::SetNextWindowSize(ImVec2(Vec2toImVec2(settings.windowSettings[1].size)));
    ImGui::SetNextWindowPos(ImVec2(Vec2toImVec2(settings.windowSettings[1].position)));
    ImGui::Begin("Select Table:", nullptr ,
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);
    

    ImGui::Combo(" ", &settings.current_table, settings.db_table_names, settings.db_table_count);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();
    if(ImGui::Button("OK") && settings.db_table_count >= 0){
            
        char db_table_query[300] = "SELECT * FROM ";
        settings.edit_table = true;
        ResetTable();

        strcat(db_table_query, settings.db_table_names[settings.current_table]);
        settings.db_result_code = RunQuery(db_table_query, settings.db_current, GetDataFromDB);
        settings.db_connected = true;

        time_t current_hour;
        time(&current_hour);

        struct tm *infHour;
        infHour = localtime(&current_hour);
        char buffer_time[80];
        strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

        settings.console_msg[settings.n_msg].type = 1;
        int msg_size = strlen("\nShowing table ") + strlen(settings.db_table_names[settings.current_table]) + strlen(buffer_time);
        settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

        strcat(settings.console_msg[settings.n_msg].string, "\n");
        strcat(settings.console_msg[settings.n_msg].string, buffer_time);
        strcat(settings.console_msg[settings.n_msg].string, "Showing table ");
        strcat(settings.console_msg[settings.n_msg].string, settings.db_table_names[settings.current_table]);
        
        settings.n_msg++;
    }
    ImGui::SameLine();
    

    ImGui::End();
}


void CurrentTableWindow(int selected_table, int database_selected,
                        char current_table_name[], int n_db_rows = 0, 
                        int n_db_cols = 0)
{
    bool keep = true;
    ImGui::SetNextWindowSize(ImVec2(Vec2toImVec2(settings.windowSettings[2].size)));
    ImGui::SetNextWindowPos(ImVec2(Vec2toImVec2(settings.windowSettings[2].position)));
    ImGui::Begin(current_table_name, nullptr , 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse );

    if (ImGui::BeginTable("HorizontalTable", settings.db_Cols + 1, 
                            ImGuiTableFlags_ScrollX | 
                            ImGuiTableFlags_ScrollY | 
                            ImGuiTableFlags_RowBg | 
                            ImGuiTableFlags_BordersOuter | 
                            ImGuiTableFlags_BordersV))
    {
        // ImGui::TableSetupScrollFreeze(1, 1);
        const int max_size_value = 1024;

        for (int i = 0; i < settings.db_Cols; i++)
        {
            ImGui::TableSetupColumn(settings.db_ColNames[i], ImGuiTableColumnFlags_None);
        }
       
       
        // End the header row
        ImGui::TableHeadersRow();

        // Insert data
        for (int row = 0; row < settings.db_Rows && keep; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < settings.db_Cols && keep; col++)
            {
                ImGui::TableSetColumnIndex(col);
                // ImGui::Text(settings.db_table_info[row][col]);
                // ImGui::InputText("\0", settings.db_table_info[row][col], sizeof(settings.db_table_info[row][col]));

                // ImGui::InputText("\0", settings.db_table_info[row][col], max_size_value);
                ImGui::PushID((100 * row) + col);

                // printf("\nrow = %d, col = %d type: %s", row, col, settings.db_ColTypes[col]);

                if (strstr(settings.db_ColTypes[col], "TEXT") || 
                    strstr(settings.db_ColTypes[col], "VARCHAR") ||
                    strstr(settings.db_ColTypes[col], "CHAR") || 
                    strstr(settings.db_ColTypes[col], "DATE"))
                {
                    ImGui::InputText("\0", settings.db_table_info[row][col], max_size_value, ImGuiInputTextFlags_AutoSelectAll);
                }
                else if (strstr(settings.db_ColTypes[col], "INTEGER") || 
                         strstr(settings.db_ColTypes[col], "TINYINT"))
                {
                    ImGui::InputText("\0", settings.db_table_info[row][col], max_size_value, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_CharsDecimal);
                }
                else if(strstr(settings.db_ColTypes[col], "REAL"))
                {
                    ImGui::InputText("\0", settings.db_table_info[row][col], max_size_value, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank);
                }
                else if (strstr(settings.db_ColTypes[col], "NUMERIC"))
                {
                    ImGui::InputText("\0", settings.db_table_info[row][col], max_size_value, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsScientific);
                }else
                {
                    ImGui::InputText("\0", settings.db_table_info[row][col], max_size_value, ImGuiInputTextFlags_AutoSelectAll);
                }

                ImGui::PopID();

                ImGui::TableSetColumnIndex(settings.db_Cols);
                
                ImGui::PushID((((100 * row) + col) + 100000));
                if(settings.edit_table)
                {
                    if (ImGui::Button("Update"))
                    {
                        UpdateData(row, settings.db_Cols);

                        // printf("\nshowing table: %s\n", settings.db_table_info[row][col]);
                        // printf("back table: %s\n", settings.db_table_info_back[row][col]);

                        char db_table_query[300] = "SELECT * FROM ";

                        ResetTable();

                        strcat(db_table_query, settings.db_table_names[settings.current_table]);
                        settings.db_result_code = RunQuery(db_table_query, settings.db_current, GetDataFromDB);
                        settings.db_connected = true;

                        time_t current_hour;
                        time(&current_hour);

                        struct tm *infHour;
                        infHour = localtime(&current_hour);
                        char buffer_time[80];
                        strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

                        settings.console_msg[settings.n_msg].type = 1;
                        int msg_size = strlen("\nShowing table ") + strlen(settings.db_table_names[settings.current_table]) + strlen(buffer_time);
                        settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

                        strcat(settings.console_msg[settings.n_msg].string, "\n");
                        strcat(settings.console_msg[settings.n_msg].string, buffer_time);
                        strcat(settings.console_msg[settings.n_msg].string, "Showing table ");
                        strcat(settings.console_msg[settings.n_msg].string, settings.db_table_names[settings.current_table]);

                        settings.n_msg++;
                    }
                    ImGui::PopID();

                    ImGui::SameLine();

                    ImGui::PushID((((100 * row) + col) + 1000));
                    if (ImGui::Button("Remove"))
                    {
                        DeleteDataCol(row, settings.db_Cols);

                        char db_table_query[300] = "SELECT * FROM ";

                        ResetTable();

                        strcat(db_table_query, settings.db_table_names[settings.current_table]);
                        settings.db_result_code = RunQuery(db_table_query, settings.db_current, GetDataFromDB);
                        settings.db_connected = true;

                        time_t current_hour;
                        time(&current_hour);

                        struct tm *infHour;
                        infHour = localtime(&current_hour);
                        char buffer_time[80];
                        strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

                        settings.console_msg[settings.n_msg].type = 1;
                        int msg_size = strlen("\nShowing table ") + strlen(settings.db_table_names[settings.current_table]) + strlen(buffer_time);
                        settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

                        strcat(settings.console_msg[settings.n_msg].string, "\n");
                        strcat(settings.console_msg[settings.n_msg].string, buffer_time);
                        strcat(settings.console_msg[settings.n_msg].string, "Showing table ");
                        strcat(settings.console_msg[settings.n_msg].string, settings.db_table_names[settings.current_table]);

                        keep = false;
                    }
                }
                ImGui::PopID();
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

void ConsoleWindow()
{
    ImGui::SetNextWindowSize(ImVec2(Vec2toImVec2(settings.windowSettings[3].size)));
    ImGui::SetNextWindowPos(ImVec2(Vec2toImVec2(settings.windowSettings[3].position)));
    ImGui::Begin("Console", nullptr , 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoScrollbar);
    
    if (ImGui::BeginChild("Subventana", ImVec2(1100, 130), true))
    {
        for(int i = settings.n_msg; i >= 0; i--){

            ConsoleMessage(settings.console_msg[i].type, i);
            
        }
        ImGui::EndChild();
    }

    if (ImGui::Button("Clear"))
    {
        for(int i = 0; i < settings.n_msg; i++){
            settings.console_msg[i].string = nullptr;
        }
        settings.n_msg = 0;   
    }
    ImGui::End();
}

void SettingsWindow()
{
    ImGui::SetNextWindowSize(ImVec2(Vec2toImVec2(settings.windowSettings[5].size)));
    ImGui::SetNextWindowPos(ImVec2(Vec2toImVec2(settings.windowSettings[5].position)));
    ImGui::Begin("Settings", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);

    if(ImGui::BeginMenuBar())
    {
        
        if(ImGui::BeginMenu("Settings Info"))
        {
            if(ImGui::BeginMenu("Resolution")){
                ImGui::Text("%dx%d", settings.screen_window_width, settings.screen_window_height);
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Fps")){
                ImGui::Text("%d", settings.fps_fps);
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Number Data Bases")){
                ImGui::Text("%d", settings.db_database_counter);
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Number of table")){
                ImGui::Text("%d", settings.db_table_count);
                ImGui::EndMenu();
            }
            
            ImGui::EndMenu();
        } 
        ImGui::Text("|");
        if (ImGui::BeginMenu("Credits"))
        {
            ImGui::Text("@2023 ESAT Academic Project by");
            if (ImGui::BeginMenu("Lucas Calatayud"))
            {
                ImGui::Text("calatayudbri@esat-alumni.com");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Luis Miguel Jimenez"))
            {
                ImGui::Text("jimenezmu@esat-alumi.com");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Carlos Mazcunan"))
            {
                ImGui::Text("mazcunyanbla@esat-alumni.com");
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::Text("|");

        if (ImGui::BeginMenu("Testing"))
        {
            if(ImGui::Button("RED"))
            {
                time_t current_hour;
                time(&current_hour);

                struct tm *infHour;
                infHour = localtime(&current_hour);
                char buffer_time[80];
                strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

                settings.console_msg[settings.n_msg].type = 0;
                int msg_size = strlen("\nRed Test ") + strlen(buffer_time);
                settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

                strcat(settings.console_msg[settings.n_msg].string, "\n");
                strcat(settings.console_msg[settings.n_msg].string, buffer_time);
                strcat(settings.console_msg[settings.n_msg].string, "Red Test ");
                settings.n_msg++;
                
            }
            if(ImGui::Button("GREEN"))
            {
                time_t current_hour;
                time(&current_hour);

                struct tm *infHour;
                infHour = localtime(&current_hour);
                char buffer_time[80];
                strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

                settings.console_msg[settings.n_msg].type = 2;
                int msg_size = strlen("\nGreen Test ") + strlen(buffer_time);
                settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

                strcat(settings.console_msg[settings.n_msg].string, "\n");
                strcat(settings.console_msg[settings.n_msg].string, buffer_time);
                strcat(settings.console_msg[settings.n_msg].string, "Green Test ");
                settings.n_msg++;
            }
            if(ImGui::Button("WHITE"))
            {
                time_t current_hour;
                time(&current_hour);

                struct tm *infHour;
                infHour = localtime(&current_hour);
                char buffer_time[80];
                strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

                settings.console_msg[settings.n_msg].type = 1;
                int msg_size = strlen("\nWhite Test ") + strlen(buffer_time);
                settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

                strcat(settings.console_msg[settings.n_msg].string, "\n");
                strcat(settings.console_msg[settings.n_msg].string, buffer_time);
                strcat(settings.console_msg[settings.n_msg].string, "White Test ");
                settings.n_msg++;
            }
            if(ImGui::Button("RANDOM"))
            {
                time_t current_hour;
                time(&current_hour);

                struct tm *infHour;
                infHour = localtime(&current_hour);
                char buffer_time[80];
                strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

                settings.console_msg[settings.n_msg].type = 3;
                int msg_size = strlen("\nRandom Test ") + strlen(buffer_time);
                settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

                strcat(settings.console_msg[settings.n_msg].string, "\n");
                strcat(settings.console_msg[settings.n_msg].string, buffer_time);
                strcat(settings.console_msg[settings.n_msg].string, "Random Test ");
                settings.n_msg++;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Spacing();

    time_t current_hour;
    time(&current_hour);

    struct tm *infHour;
    infHour = localtime(&current_hour);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d [%H:%M:%S]", infHour);
    ImGui::Text(buffer);
    ImGui::SameLine();
    ImGui::Text("V-1.1.3");
    ImGui::SameLine();
    if(settings.visual_connected == true){
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Connected");
    }else if(settings.visual_connected == false){
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Disconnected");
    }


    ImGui::End();
}

void QuerieWindow()
{
    ImGui::SetNextWindowSize(ImVec2(Vec2toImVec2(settings.windowSettings[4].size)));
    ImGui::SetNextWindowPos(ImVec2(Vec2toImVec2(settings.windowSettings[4].position)));
    ImGui::Begin("Queries:", nullptr , 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);
    ImGui::InputTextMultiline("##source", settings.querie_text, IM_ARRAYSIZE(settings.querie_text),
                              ImVec2(-1.0f, ImGui::GetTextLineHeight() * 8),
                              ImGuiInputTextFlags_AllowTabInput);
    if(ImGui::Button("Submit"))
    {
        settings.edit_table = false;
        ResetTable();
        
        settings.db_result_code = RunQuery(settings.querie_text, settings.db_current, GetQueryFromDB);
        
        if (0 == settings.db_result_code && settings.querie_text[0] != '\0')
        {
            settings.db_connected = true;

            time_t current_hour;
            time(&current_hour);

            struct tm *infHour;
            infHour = localtime(&current_hour);
            char buffer_time[80];
            strftime(buffer_time, 80, "[%H:%M:%S]", infHour);

            settings.console_msg[settings.n_msg].type = 2;
            int msg_size = strlen("Query: ") + strlen(settings.querie_text) + strlen(buffer_time);
            settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));

            strcat(settings.console_msg[settings.n_msg].string, "\n");
            strcat(settings.console_msg[settings.n_msg].string, buffer_time);
            strcat(settings.console_msg[settings.n_msg].string, "Query: ");
            strcat(settings.console_msg[settings.n_msg].string, settings.querie_text);

            settings.n_msg++;
        }
        else
        {
           ResetTable();
            settings.db_connected = false;
        }
        if (settings.db_Cols <= 0 || settings.db_Cols > 64)
        {
            // AddErrorMsg("Unable to access table: Only 1-64 columns allowed");
        }
        if (settings.querie_text[0] == '\0')
        {
            AddErrorMsg("Empty query");
        }
    }
    ImGui::SameLine();

    
    if(ImGui::Button("Clear"))
    {
        settings.querie_text[0] = '\0';
    }
    ImGui::End();
}

void AllWindow(){
    DataBaseSelectedWindow();
    SettingsWindow();
    TableSelectedWindow(settings.current_database);
    if(settings.db_connected){
        if (settings.db_Cols > 0 && settings.db_Cols <= 64)
        {
            CurrentTableWindow(0, 0, settings.db_table_names[settings.current_table], settings.db_Rows, settings.db_Cols);
        }else{
            // AddErrorMsg("Asertion failed: columns_count > 0 && coulumns_count <= 64 && Only 1..64 \"columns allowed!\"");
        }
    }
    ConsoleWindow();
    QuerieWindow();
}