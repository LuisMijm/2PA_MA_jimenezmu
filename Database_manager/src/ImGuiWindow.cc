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

void ConsoleMessage(int type, int i)
{
    if (settings.console_msg[i] != NULL)
    {
        switch (type)
        {
        //error messages type (red)
        case 0:
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), settings.console_msg[i]);
            break;
        //console message type (white)
        case 1:
            ImGui::Text(settings.console_msg[i]);
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

    ImGui::SetNextWindowSize(ImVec2(200, 110));
    ImGui::SetNextWindowPos(ImVec2(20, 20));
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
        char db_path[100] = "../../data/internaldb/";

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
        settings.type_msg = 1;
        int msg_size = strlen("Connected to database ") + strlen(settings.db_names[settings.current_database]);
        settings.console_msg[settings.n_msg] = (char *)calloc(msg_size, sizeof(char));
        strcat(settings.console_msg[settings.n_msg], "\nConnected to database ");
        strcat(settings.console_msg[settings.n_msg], settings.db_names[settings.current_database]);
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
        settings.type_msg = 1;
        int msg_size = strlen("Disconected from database ") + strlen(settings.db_names[settings.current_database]);
        settings.console_msg[settings.n_msg] = (char *)calloc(msg_size, sizeof(char));
        strcat(settings.console_msg[settings.n_msg], "\nDisconected from database ");
        strcat(settings.console_msg[settings.n_msg], settings.db_names[settings.current_database]);
        settings.n_msg++;
    }

    ImGui::End();
}

void TableSelectedWindow(int selected_database/* , char *current_database_name */)
{
    ImGui::SetNextWindowSize(ImVec2(200, 110));
    ImGui::SetNextWindowPos(ImVec2(20, 150));
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
    ImGui::SetNextWindowSize(ImVec2(540, 240));
    ImGui::SetNextWindowPos(ImVec2(240, 20));
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
        // Freeze the first column
        ImGui::TableSetupScrollFreeze(1, 1);

        for (int i = 0; i < settings.db_Cols; i++)
        {
            ImGui::TableSetupColumn(settings.db_ColNames[i], ImGuiTableColumnFlags_None);
        }
        
        // Define column headers
       

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
    ImGui::SetNextWindowSize(ImVec2(540, 150));
    ImGui::SetNextWindowPos(ImVec2(240, 280));
    ImGui::Begin("Console", nullptr , 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoScrollbar);
    
    if (ImGui::BeginChild("Subventana", ImVec2(520, 110), true))
    {
        for(int i = settings.n_msg; i > 0; i--){
            ConsoleMessage(settings.type_msg, i);
            
        }
        ImGui::EndChild();
    }

    if (ImGui::Button("Clear"))
    {
        //solucionar liberar de memoria
        for(int i = 0; i < settings.n_msg; i++){
            /* free(settings.console_msg[i]); */
            settings.console_msg[i] = nullptr;
        }
        settings.n_msg = 0;   
    }
    ImGui::End();
}

void QuerieWindow()
{
    ImGui::SetNextWindowSize(ImVec2(200, 150));
    ImGui::SetNextWindowPos(ImVec2(20, 280));
    ImGui::Begin("Queries:", nullptr , 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);
    ImGui::InputTextMultiline("##source", settings.querie_text, IM_ARRAYSIZE(settings.querie_text),
                              ImVec2(-1.0f, ImGui::GetTextLineHeight() * 7),
                              ImGuiInputTextFlags_AllowTabInput);
    if(ImGui::Button("Submit"))
    {
        // FreeTable(settings.db_Cols, settings.db_Rows);
        // settings.db_Cols = settings.db_Rows = 0;
        ResetTable();
        // settings.db_table_count = 0;
        settings.db_result_code = RunQuery(settings.querie_text, settings.db_current, GetDataFromDB);
        if (0 == settings.db_result_code)
        {
            settings.db_connected = true;

        }else
        {
            ResetTable();
            settings.db_connected = false;
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
    TableSelectedWindow(settings.current_database/* , "DataBase" */);
    if(settings.db_connected){
        CurrentTableWindow(0, 0,"table", settings.db_Rows, settings.db_Cols);
    }
    ConsoleWindow();
    QuerieWindow();
}