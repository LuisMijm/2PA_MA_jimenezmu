
#include <database.h>

#include <stdio.h>
#include <stdlib.h>
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
    // printf("Rows: %d \n", settings.db_Rows);
    if (settings.db_Rows == 0)
    {
        settings.db_Cols = argc;
        settings.db_ColNames = (char**)realloc(settings.db_ColNames, argc * sizeof(char*));

        for (int i = 0; i < argc; i++)
        {
            settings.db_ColNames[i] = (char *)calloc(kStringSize,sizeof(char));

            sscanf(azcolname[i], "%s", settings.db_ColNames[i]);
        }        
    }

    settings.db_Rows++;

    // Row memory reserve
    settings.db_table_info = (char ***)realloc(settings.db_table_info, settings.db_Rows * sizeof(char **));

    // Col memory reserve
    settings.db_table_info[settings.db_Rows - 1] = (char **)calloc(argc,sizeof(char *));

    // Strings memory reserve
    for (int i = 0; i < argc; i++)
    {
        settings.db_table_info[settings.db_Rows - 1][i] = (char *)calloc(kStringSize, sizeof(char));

        sscanf(argv[i], "%s", settings.db_table_info[settings.db_Rows - 1][i]);
    }

    return 0;
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
