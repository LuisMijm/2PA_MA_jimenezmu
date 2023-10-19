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

#include "../include/window.h"    


void DataBaseSelectedWindow()
{
    ImGui::SetNextWindowSize(ImVec2(200, 60));
    ImGui::SetNextWindowPos(ImVec2(20, 120));
    ImGui::Begin("Select DataBase:", nullptr ,
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);
    ImGui::Combo("", &settings.current_database, 
                "DataBaseA\0DataBaseB\0DataBaseC\0DataBaeD\0DataBaseE\0\0");
    ImGui::End();
}

void TableSelectedWindow(int selected_database, char current_database_name[])
{
    ImGui::SetNextWindowSize(ImVec2(200, 60));
    ImGui::SetNextWindowPos(ImVec2(20, 200));
    ImGui::Begin("Select Table:", nullptr ,
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);
    ImGui::Combo("", &settings.current_table,
                 "TableA\0TableB\0TableC\0TableD\0TableE\0\0");

    ImGui::End();
}

void TextWindow()
{
    ImGui::SetNextWindowSize(ImVec2(200, 80));
    ImGui::SetNextWindowPos(ImVec2(20, 20));
    ImGui::Begin("TextWindow", nullptr , 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Data Base Manager");
    ImGui::Text("Academic Project");
    ImGui::End();
}

void CurrentTableWindow(char current_table_name[])
{
    ImGui::SetNextWindowSize(ImVec2(540, 240));
    ImGui::SetNextWindowPos(ImVec2(240, 20));
    ImGui::Begin(current_table_name, nullptr , 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse );
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

    }
    ImGui::SameLine();
    if(ImGui::Button("Clear"))
    {
        settings.querie_text[0] = '\0';
    }
    ImGui::End();
}

void AllWindow(){
    TextWindow();
    DataBaseSelectedWindow();
    TableSelectedWindow(0, "DataBase");
    CurrentTableWindow("table");
    ConsoleWindow();
    QuerieWindow();
}