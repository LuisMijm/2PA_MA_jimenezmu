/**
 * @file database.h
 * @author Luis Miguel Jiménez
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _DATABASE_H__
#define _DATABASE_H__ 1

int GetDataFromDB(void *not_used, int argc, char **argv, char **azcolname);

void FreeTable(int cols, int rows);


#endif