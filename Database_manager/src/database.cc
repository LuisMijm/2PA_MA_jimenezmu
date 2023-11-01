
#include <database.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <esat_extra/sqlite3.h>

#include <config.h>


int ResetTable()
{
    FreeTable(settings.db_Cols, settings.db_Rows, settings.db_table_info);
    FreeTable(settings.db_Cols, settings.db_Rows, settings.db_table_info_back);
    settings.db_Cols = 0;
    settings.db_Rows = 0;

    return 0;
}

void AssesData(char* data, char* type)
{
    int dot_count = 0;
    
    if (strstr(type, "TEXT") != NULL)
    {
        
    }
    else if (strstr(type, "INTEGER") != NULL)
    {
        
    }
    else if (strstr(type, "REAL") != NULL)
    {
        
    }
    else if (strstr(type, "NUMERIC") != NULL)
    {
        
    }
}

int GetTablesFromDB(void *not_used, int argc, char **argv, char **azcolname)
{
    settings.db_table_count++;
    settings.db_table_names = (char**)realloc(settings.db_table_names, 
                               settings.db_table_count * sizeof(char*));
                               
    settings.db_table_names[settings.db_table_count - 1] = (char*)calloc(kStringSize, sizeof(char));

    sscanf(argv[0], "%s", settings.db_table_names[settings.db_table_count - 1]);

    return 0;
}

int GetTableTypes(void *used, int argc, char **argv, char **azcolname)
{
    int* count = (int*)used;
    
    sscanf(argv[2], "%s", settings.db_ColTypes[count[0]]);
    printf("\ncol %d type: %s - argv2: %s", count[0], settings.db_ColTypes[*count], argv[2]);
    count[0]++;

    return 0;
}

int GetDataFromDB(void *not_used, int argc, char **argv, char **azcolname)
{
    not_used = 0;
    int count = 0;
    char pragma_query[300] = "PRAGMA table_info(";

    if (argc > 0 && argc <= 64)
    {
        if (settings.db_Rows == 0)
        {
            settings.db_Cols = argc;
            settings.db_ColNames = (char **)realloc(settings.db_ColNames, argc * sizeof(char *));
            settings.db_ColTypes = (char **)realloc(settings.db_ColTypes, argc * sizeof(char *));

            for (int i = 0; i < argc; i++)
            {
                settings.db_ColNames[i] = (char *)calloc(kStringSize, sizeof(char));

                sscanf(azcolname[i], "%s", settings.db_ColNames[i]);

                settings.db_ColTypes[i] = (char *)calloc(kStringSize, sizeof(char));

            }

            strcat(pragma_query, settings.db_table_names[settings.current_table]);
            strcat(pragma_query, ");");
            // printf("pragma query: %s", pragma_query);
            RunQuery(pragma_query, settings.db_current, GetTableTypes, &count);
        }

        settings.db_Rows++;

        // Row memory reserve
        settings.db_table_info = (char ***)realloc(settings.db_table_info, settings.db_Rows * sizeof(char **));
        settings.db_table_info_back = (char ***)realloc(settings.db_table_info_back, settings.db_Rows * sizeof(char **));

        // Col memory reserve
        settings.db_table_info[settings.db_Rows - 1] = (char **)calloc(argc, sizeof(char *));
        settings.db_table_info_back[settings.db_Rows - 1] = (char **)calloc(argc, sizeof(char *));

        // Strings memory reserve
        for (int i = 0; i < argc; i++)
        {
            if (NULL != argv[i])
            {
                settings.db_table_info[settings.db_Rows - 1][i] = (char *)calloc(strlen(argv[i]) + kStringSize, sizeof(char));
                sscanf(argv[i], "%[^\n]", settings.db_table_info[settings.db_Rows - 1][i]);

                settings.db_table_info_back[settings.db_Rows - 1][i] = (char *)calloc(strlen(argv[i]) + kStringSize, sizeof(char));
                sscanf(argv[i], "%[^\n]", settings.db_table_info_back[settings.db_Rows - 1][i]);
            }
            else
            {
                settings.db_table_info[settings.db_Rows - 1][i] = (char *)calloc(kStringSize, sizeof(char));
                strcpy(settings.db_table_info[settings.db_Rows - 1][i], "\0");

                settings.db_table_info_back[settings.db_Rows - 1][i] = (char *)calloc(kStringSize, sizeof(char));
                strcpy(settings.db_table_info_back[settings.db_Rows - 1][i], "\0");
            }
        }
    }else
    {
        AddErrorMsg("Asertion failed: columns_count > 0 && coulumns_count <= 64 && Only 1..64 \"columns allowed!\"");
    }

    return 0;
}

// void InsertDataLine(int cols)
// {
//     char* insertString = (char*)calloc(cols * kStringSize, sizeof(char));

//     strcpy(insertString, "INSERT INTO (");

//     for (int i = 0; i < cols; i++)
//     {
//         // strcpy(insertString, settings.db_ColNames[i]);
//         if (i < cols - 1)
//             strcpy(insertString, ", ");
//     }
//     strcpy(insertString, ")\n");
//     strcpy(insertString, "VALUES (");

//     for (int i = 0; i < cols; i++)
//     {
//         strcpy(insertString, "\'");
//         // strcpy(insertString, settings.db_table_info[row][i]);
//         if (i < cols - 1)
//             strcpy(insertString, "\', ");
//     }
//     strcpy(insertString, ")\n");

//     RunQuery(insertString, settings.db_current, GetDataFromDB);

// }

void CheckString(char *query, char *value)
{
    char v[2];
    for (int i = 0; i < (signed int)strlen(value); i++)
    {
        if (value[i] == 39)
        {
            strcat(query, "'");
        }
        v[0] = value[i];
        v[1] = '\0';
        strcat(query, v);
    }
}

void DeleteDataCol(int row, int cols)
{
    char deleteString[4096] = "\0";

    strcat(deleteString, "DELETE FROM ");
    strcat(deleteString, settings.db_table_names[settings.current_table]);
    strcat(deleteString, "\nWHERE ");

    for (int i = 0; i < cols; i++)
    {
        strcat(deleteString, settings.db_ColNames[i]);

        if (settings.db_table_info_back[row][i][0] == '\0')
        {
            strcat(deleteString, " IS NULL OR ");
            strcat(deleteString, settings.db_ColNames[i]);
            strcat(deleteString, " = ''");
        }
        else
        {
            strcat(deleteString, " = '");
            CheckString(deleteString, settings.db_table_info_back[row][i]);
            strcat(deleteString, "'");
        }

        if (i < cols - 1)
        {
            strcat(deleteString, " AND ");
        }
    }
    strcat(deleteString, ";");

    printf("\n query: %s", deleteString);

    RunQuery(deleteString, settings.db_current, GetDataFromDB);
}

void UpdateData(int row, int cols)
{
    char updateString[4096] = "\0";

    strcat(updateString, "UPDATE ");
    strcat(updateString, settings.db_table_names[settings.current_table]);
    strcat(updateString, "\n SET ");

    for (int i = 0; i < cols; i++)
    {
        strcat(updateString, settings.db_ColNames[i]);
        strcat(updateString, " = ");

        if (settings.db_table_info[row][i] != NULL)
        {
            strcat(updateString, "'");
            CheckString(updateString, settings.db_table_info[row][i]);
            strcat(updateString, "'");
        }else
        {
            strcat(updateString, "'NULL'");
        }

        if (i < cols - 1)
        {
            strcat(updateString, ", ");
        }
    }
    strcat(updateString, "\nWHERE ");

    for (int i = 0; i < cols; i++)
    {
        strcat(updateString, settings.db_ColNames[i]);

        if (settings.db_table_info_back[row][i][0] == '\0')
        {
            strcat(updateString, " IS NULL OR ");
            strcat(updateString, settings.db_ColNames[i]);
            strcat(updateString, " = ''");
        }
        else
        {
            strcat(updateString, " = '");
            CheckString(updateString, settings.db_table_info_back[row][i]);
            strcat(updateString, "'");
        }

        if (i < cols - 1)
        {
            strcat(updateString, " AND ");
        }
    }

    strcat(updateString, ";");

    RunQuery(updateString, settings.db_current, GetDataFromDB);
}

void FreeTable(int cols, int rows, char*** table) {
    if (table != nullptr)
    {
        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                free(table[j][i]);
                table[j][i] = nullptr;
            }
        }

        for (int i = 0; i < rows; i++)
        {
            free(table[i]);
            table[i] = nullptr;
        }

        for (int i = 0; i < cols; i++)
        {
            if (settings.db_ColNames != nullptr)
            {
                if (settings.db_ColNames[i] != nullptr)
                {
                    free(settings.db_ColNames[i]);
                    settings.db_ColNames[i] = nullptr;
                }
            }

            if (settings.db_ColTypes != nullptr)
            {   
                if (settings.db_ColTypes[i] != nullptr)
                {
                    free(settings.db_ColTypes[i]);
                    settings.db_ColTypes[i];
                }
            }
        }

        free(settings.db_ColNames);
        settings.db_ColNames = nullptr;

        free(settings.db_ColTypes);
        settings.db_ColTypes = nullptr;
        // free(table);
        // table = nullptr;
    }
}

void GeneralFree() 
{
    FreeTable(settings.db_Cols, settings.db_Rows, settings.db_table_info);
    FreeTable(settings.db_Cols, settings.db_Rows, settings.db_table_info_back);

    if (settings.db_current != nullptr)
    {
        free(settings.db_current);
        settings.db_current = nullptr;
    }
    if (settings.db_internal != nullptr)
    {
        free(settings.db_internal);
        settings.db_internal = nullptr;
    }

    for(int i = 0; i < settings.db_Cols; i++) {
        free(settings.db_table_names[i]);
        settings.db_table_names[i] = nullptr;
    }
    
    free(settings.db_table_names);
    settings.db_table_names = nullptr;
}