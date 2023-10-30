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
    if (settings.console_msg[i].string != NULL)
    {
        switch (type)
        {
        //error messages type (red)
        case 0:
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), settings.console_msg[i].string);
            break;
        //console message type (white)
        case 1:
            ImGui::Text(settings.console_msg[i].string);
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
        settings.console_msg[settings.n_msg].type = 1;
        int msg_size = strlen("Connected to database ") + strlen(settings.db_names[settings.current_database]);
        settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));
        strcat(settings.console_msg[settings.n_msg].string, "\nConnected to database ");
        strcat(settings.console_msg[settings.n_msg].string, settings.db_names[settings.current_database]);
        settings.n_msg++;

    }
    ImGui::SameLine();
    
    if(ImGui::Button("Disconnect")){
        ResetTable();
        settings.db_table_count = 0;

        sqlite3_close(settings.db_current);
        settings.db_connected = false;

        printf("Disconected from database %s\n", settings.db_names[settings.current_database]);
        /* printf("Disconected from database %s\n", settings.db_names[settings.current_database]); */

        //For show console message
        settings.console_msg[settings.n_msg].type = 1;
        int msg_size = strlen("Disconected from database ") + strlen(settings.db_names[settings.current_database]);
        settings.console_msg[settings.n_msg].string = (char *)calloc(msg_size, sizeof(char));
        strcat(settings.console_msg[settings.n_msg].string, "\nDisconected from database ");
        strcat(settings.console_msg[settings.n_msg].string, settings.db_names[settings.current_database]);
        settings.n_msg++;
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

        ResetTable();

        strcat(db_table_query, settings.db_table_names[settings.current_table]);
        settings.db_result_code = RunQuery(db_table_query, settings.db_current, GetDataFromDB);
        settings.db_connected = true;
    }
    ImGui::SameLine();
    

    ImGui::End();
}


void CurrentTableWindow(int selected_table, int database_selected,
                        char current_table_name[], int n_db_rows = 0, 
                        int n_db_cols = 0)
{
    ImGui::SetNextWindowSize(ImVec2(Vec2toImVec2(settings.windowSettings[2].size)));
    ImGui::SetNextWindowPos(ImVec2(Vec2toImVec2(settings.windowSettings[2].position)));
    ImGui::Begin(current_table_name, nullptr , 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse );

    if (ImGui::BeginTable("HorizontalTable", settings.db_Cols, 
                            ImGuiTableFlags_ScrollX | 
                            ImGuiTableFlags_ScrollY | 
                            ImGuiTableFlags_RowBg | 
                            ImGuiTableFlags_BordersOuter | 
                            ImGuiTableFlags_BordersV))
    {
        // ImGui::TableSetupScrollFreeze(1, 1);

        for (int i = 0; i < settings.db_Cols; i++)
        {
            ImGui::TableSetupColumn(settings.db_ColNames[i], ImGuiTableColumnFlags_None);
        }
       
       
        // End the header row
        ImGui::TableHeadersRow();

        // Insert data
        for (int row = 0; row < settings.db_Rows; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < settings.db_Cols; col++)
            {
                ImGui::TableSetColumnIndex(col);
                ImGui::Text(settings.db_table_info[row][col]);
                
                // switch ()
                // {
                // case /* constant-expression */:
                //     /* code */
                //     break;
                
                // default:
                //     break;
                // }
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
    
    if (ImGui::BeginChild("Subventana", ImVec2(1100, 135), true))
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

void QuerieWindow()
{
    ImGui::SetNextWindowSize(ImVec2(Vec2toImVec2(settings.windowSettings[4].size)));
    ImGui::SetNextWindowPos(ImVec2(Vec2toImVec2(settings.windowSettings[4].position)));
    ImGui::Begin("Queries:", nullptr , 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);
    ImGui::InputTextMultiline("##source", settings.querie_text, IM_ARRAYSIZE(settings.querie_text),
                              ImVec2(-1.0f, ImGui::GetTextLineHeight() * 8.5),
                              ImGuiInputTextFlags_AllowTabInput);
    if(ImGui::Button("Submit"))
    {

        ResetTable();

        settings.db_result_code = RunQuery(settings.querie_text, settings.db_current, GetDataFromDB);
        if (0 == settings.db_result_code)
        {
            settings.db_connected = true;

        }else
        {
            ResetTable();
            settings.db_connected = false;
        }
        if (settings.db_Cols <= 0 || settings.db_Cols > 64)
        {
            // AddErrorMsg("Unable to acces table: Only 1-64 columns allowed");
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