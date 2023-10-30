
#include <database.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <esat_extra/sqlite3.h>

#include <config.h>

// typedef char* String;


int ResetTable()
{
    FreeTable(settings.db_Cols, settings.db_Rows);
    // settings.db_table_count = 0;
    settings.db_Cols = 0;
    settings.db_Rows = 0;

    return 0;
}


int GetTablesFromDB(void *not_used, int argc, char **argv, char **azcolname)
{
    printf("%s\n", argv[0]);
   

    settings.db_table_count++;
    settings.db_table_names = (char**)realloc(settings.db_table_names, 
                               settings.db_table_count * sizeof(char*));
                               
    settings.db_table_names[settings.db_table_count - 1] = (char*)calloc(kStringSize, sizeof(char));
    // settings.db_table_names[settings.db_table_count - 1] = argv[1];
    sscanf(argv[0], "%s", settings.db_table_names[settings.db_table_count - 1]);

    return 0;
}


int GetDataFromDB(void *not_used, int argc, char **argv, char **azcolname)
{
    not_used = 0;
    if (argc > 0 && argc <= 64)
    {
        if (settings.db_Rows == 0)
        {
            settings.db_Cols = argc;
            settings.db_ColNames = (char **)realloc(settings.db_ColNames, argc * sizeof(char *));

            for (int i = 0; i < argc; i++)
            {
                settings.db_ColNames[i] = (char *)calloc(kStringSize, sizeof(char));

                sscanf(azcolname[i], "%s", settings.db_ColNames[i]);
            }
        }

        settings.db_Rows++;

        // Row memory reserve
        settings.db_table_info = (char ***)realloc(settings.db_table_info, settings.db_Rows * sizeof(char **));

        // Col memory reserve
        settings.db_table_info[settings.db_Rows - 1] = (char **)calloc(argc, sizeof(char *));

        // Strings memory reserve
        for (int i = 0; i < argc; i++)
        {
            if (NULL != argv[i])
            {
                settings.db_table_info[settings.db_Rows - 1][i] = (char *)calloc(strlen(argv[i]) + kStringSize, sizeof(char));
                sscanf(argv[i], "%[^\n]", settings.db_table_info[settings.db_Rows - 1][i]);
            }
            else
            {
                settings.db_table_info[settings.db_Rows - 1][i] = (char *)calloc(kStringSize, sizeof(char));
                strcpy(settings.db_table_info[settings.db_Rows - 1][i], "NULL");
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

//     free(insertString);
// }

void UpdateData(int row, int cols)
{
    char* updateString = (char*)calloc((cols * kStringSize) + kStringSize, sizeof(char));

    strcat(updateString, "UPDATE ");
    strcat(updateString, settings.db_table_names[settings.current_table]);
    strcat(updateString, " SET");

    for (int i = 0; i < cols; i++)
    {
        strcat(updateString, settings.db_ColNames[i]);
        strcat(updateString, " = ");

        if (settings.db_table_info[row][i] != NULL)
        {
            strcat(updateString, settings.db_table_info[row][i]);
        }else
        {
            strcat(updateString, "NULL");
        }
        
        strcat(updateString, "\'");

        if (i < cols - 1)
        {
            strcat(updateString, "\' ");   
        }
    }
    strcat(updateString, "WHERE ");
    // strcat(updateString, "");

    // RunQuery(updateString, settings.db_current, GetDataFromDB);

    free(updateString);
}


void FreeTable(int cols, int rows) {

    for(int i = 0; i < cols; i++)
    {
        // printf("free col: %d\n", i);
        for(int j = 0; j < rows; j++){
            // printf("free row: %d\n", j);
            // printf("table info 1: %s\n", settings.db_table_info[j][i]);

            free(settings.db_table_info[j][i]);
            settings.db_table_info[j][i] = nullptr;
        }
    }
    
    for (int i = 0; i < rows; i++)
    {
        free(settings.db_table_info[i]);
        settings.db_table_info[i] = nullptr;
    }
    
    for(int i = 0; i < cols; i++){
        free(settings.db_ColNames[i]);
        settings.db_ColNames[i] = nullptr;
    }

    free(settings.db_ColNames);
    settings.db_ColNames = nullptr;
    free(settings.db_table_info);
    settings.db_table_info = nullptr;
}
