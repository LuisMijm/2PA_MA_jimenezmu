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
 * @brief Convert a custom Vec2 vector to an ImVec2 vector.
 *
 * This function takes a custom Vec2 vector and converts it to an ImVec2 vector, which is suitable for use with ImGui functions.
 *
 * @param vector The custom Vec2 vector to be converted.
 * @return An ImVec2 vector equivalent to the input custom Vec2 vector.
 *
 */
ImVec2 Vec2toImVec2(Vec2 vector);

/**
 * @brief Display a console message with color based on message type.
 *
 * This function displays a console message with a specified color based on the message type. It is used to show error messages in red and regular console messages in white using ImGui's text display functions.
 *
 * @param type The type of the console message (0 for error, 1 for regular message).
 * @param i The index of the message in the `settings.console_msg` array.
 *
 */

void ConsoleMessage(int type, int i);

/**
* @brief Open a window for selecting a database.
*
* This function creates a window using ImGui for selecting a database. It provides options to connect to or disconnect from a database.
*
*/
void DataBaseSelectedWindow();

/**
 * @brief Open a window for selecting a table within a database.
 * This function creates a window using ImGui for selecting a table within the currently connected database.
 *
 * @param selected_database The index of the currently selected database.
 *
 */
void TableSelectedWindow(int selected_database);

/**
 * @brief Display the contents of a selected table within the currently connected database.
 *
 * This function creates a window using ImGui to display the contents of a selected table within the currently connected database. It uses an ImGui table to present the data in rows and columns.
 *
 * @param selected_table The index of the currently selected table.
 * @param database_selected The index of the currently selected database.
 * @param current_table_name The name of the currently selected table.
 * @param n_db_rows The number of rows in the table (default is 0).
 * @param n_db_cols The number of columns in the table (default is 0).
 *
 */
void CurrentTableWindow(int selected_table, int database_selected,
                        char current_table_name[], int n_db_rows,
                        int n_db_cols);

/**
 * @brief Display a console window for viewing messages and clearing the message history.
 *
 * This function creates a console window using ImGui for viewing messages and provides a button to clear the message history. Messages are displayed with their respective colors based on their types.
 *
 */
void ConsoleWindow();

/**
 * @brief Display a settings window for viewing application information and performing tests.
 *
 * This function creates a settings window using ImGui for displaying various application information, including resolution, frame rate, database counts, and the application version. It also provides buttons for performing tests and viewing application credits. Additionally, it displays the current connection status.
 *
 */
void SettingsWindow();

/**
 * @brief Display a window for inputting and executing database queries.
 *
 * This function creates a window using ImGui for inputting database queries and provides options to submit the query and clear the input field. It also handles the execution of queries and updates the user interface accordingly.
 *
 */

void QuerieWindow();

/**
 * @brief Display the main application window containing various sub-windows.
 *
 * This function serves as the main application window that displays several sub-windows for different tasks. It includes windows for selecting a database, selecting a table, viewing table data, a console for messages, and executing database queries. The content of each sub-window is dependent on the current application state.
 *
 */

void AllWindow();

#endif