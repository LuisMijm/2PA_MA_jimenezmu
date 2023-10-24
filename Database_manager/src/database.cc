
#include <database.h>

#include <stdio.h>
#include <stdlib.h>
#include <esat_extra/sqlite3.h>

#include <config.h>

// typedef char* String;

/**
 * @brief 
 * 
 * @return int 
 */
int ResetData()
{
    FreeTable(settings.db_Cols, settings.db_Rows);
    settings.db_Cols = 0;
    settings.db_Rows = 0;

    return 0;
}

/**
 * @brief Get the Data From Data Base object
 * 
 * @param not_used 
 * @param argc 
 * @param argv 
 * @param azcolname 
 * @return int 
 */
int GetDataFromDB(void *not_used, int argc, char **argv, char **azcolname)
{
    not_used = 0;

    if (settings.db_Rows == 0)
    {
        settings.db_Cols = argc;
        settings.db_ColNames = (char**)realloc(settings.db_ColNames, argc * sizeof(char*));

        for (int i = 0; i < argc; i++)
        {
            settings.db_ColNames[i] = (char *)realloc(settings.db_ColNames[i], kStringSize * sizeof(char));

            sscanf(azcolname[i], "%s", settings.db_ColNames[i]);
        }        
    }

    settings.db_Rows++;

    // Row memory reserve
    settings.db_table_info = (char ***)realloc(settings.db_table_info, settings.db_Rows * sizeof(char **));

    // Col memory reserve
    settings.db_table_info[settings.db_Rows - 1] = (char **)realloc(settings.db_table_info[settings.db_Rows - 1], argc * sizeof(char *));

    // Strings memory reserve
    for (int i = 0; i < argc; i++)
    {
        settings.db_table_info[settings.db_Rows - 1][i] = (char *)realloc(settings.db_table_info[settings.db_Rows - 1][i], kStringSize * sizeof(char));

        sscanf(argv[i], "%s", settings.db_table_info[settings.db_Rows - 1][i]);
    }

    return 0;
}

/**
 * @brief 
 * 
 * @param cols 
 * @param rows 
 */
void FreeTable(int cols, int rows) {

    for(int i = 0; i < cols; i++)
    {
        for(int j = 0; j < rows; j++){
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
