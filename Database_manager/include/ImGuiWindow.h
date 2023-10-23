/**
* @file ImGuiWindow.h
* @author  Carlos Mazcunan <mazcunyanbla@esat-alumni.com>
* @brief Window header
* @version 0.1
* @date 2023-1000-18
*
* @copyright Copyright (c) 2023
*/

#ifndef __IMGUIWINDOW_H__
#define __IMGUIWINDOW_H__ 1

#include <stdio.h>

#include "esat\window.h"
#include "esat\input.h"
#include "esat\draw.h"
#include "esat\time.h"
#include "esat_extra\imgui.h"

#include "config.h"

/**
 * @brief Window to select data base
 * 
 * @copyright Coyright (c) 2023
 * 
*/
void DataBaseSelectedWindow();

/**
 * @brief Window to select table
 * 
 * @copyright Coyright (c) 2023
 * 
*/
void TableSelectedWindow(int selected_database, char current_database_name[]);


/**
 * @brief Table Window
 * 
 * Window to see the values of the tables
 * 
 * @copyright Coyright (c) 2023
 * 
*/
void CurrentTableWindow(int selected_table, int database_selected,
                        char current_table_name[], int n_db_rows);

/**
 * @brief Console Window
 * 
 * Window with console that show error messages
 * 
 * @copyright Coyright (c) 2023
 * 
*/
void ConsoleWindow();

/**
 * @brief Window with text
 * 
 * Window with some information of the software
 * 
 * @copyright Coyright (c) 2023
 * 
*/

void QuerieWindow();

/**
 * @brief Compilation of all windows
 * 
 * @copyright Copyright (c) 2023
*/

void AllWindow();

#endif