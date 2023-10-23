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

#include "../include/database.h"    
#include "../include/ImGuiWindow.h"    


void DataBaseSelectedWindow()
{
    ImGui::SetNextWindowSize(ImVec2(200, 110));
    ImGui::SetNextWindowPos(ImVec2(20, 20));
    ImGui::Begin("Select DataBase:", nullptr ,
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);
    ImGui::Combo("", &settings.current_database, 
                "InternalDB\0MyDB\0");
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();
    if(ImGui::Button("Connect")){
        char db_path[30] = "./data/databases/";
        strcat(db_path, settings.db_names[settings.current_database]);
        ConnectToDB(db_path, &settings.db_current, &settings.db_result_code);
    }
    ImGui::SameLine();
    if(ImGui::Button("Disconnect")){
        // sqlite3_close(*settings.db_current);
    }

    ImGui::End();
}

void TableSelectedWindow(int selected_database, char current_database_name[])
{
    ImGui::SetNextWindowSize(ImVec2(200, 110));
    ImGui::SetNextWindowPos(ImVec2(20, 150));
    ImGui::Begin("Select Table:", nullptr ,
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);
    
    switch (selected_database)
    {
    case 0:
            ImGui::Combo("", &settings.current_table,
                        "Settings Table\0\0");
        break;
    case 1:
        ImGui::Combo("", &settings.current_table,
                        "Table1\0Table2\0Table3\0\0");
        break;
    default:
        break;
    }
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();
    if(ImGui::Button("OK")){
        char db_table_query[30] = "SELECT * FROM ";
        strcat(db_table_query, settings.db_table_names[settings.current_table]);
        settings.db_result_code = RunQuery(db_table_query, settings.db_current, GetDataFromDB);
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
    if (ImGui::BeginTable("Tabla1", 3)) {
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("DataType");
        ImGui::TableSetupColumn("Value");
        for (int row = 0; row < n_db_rows; row++) {
            ImGui::TableNextRow();
            for (int col = 0; col < n_db_cols; col++) {
                ImGui::TableSetColumnIndex(col);
                ImGui::Text("Fila %d, Col %d", row, col);
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
                    | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
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
        FreeTable(settings.db_Cols, settings.db_Rows);
        settings.db_Cols = settings.db_Rows = 0;
        settings.db_result_code = RunQuery(settings.querie_text, settings.db_current, GetDataFromDB);
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
    TableSelectedWindow(settings.current_database, "DataBase");
    CurrentTableWindow(0, 0,"table", settings.db_Rows, settings.db_Cols);
    ConsoleWindow();
    QuerieWindow();
}