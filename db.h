/*
 *  db.h
 *
 *  database-like structure with table and row
 *  simple yet efficient
 *
 *  Author: Muhammad Daffa Rahman
 */

// menghindari duplikat
#ifndef _DB_H_
#define _DB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

typedef struct
{
    int id;
    int col_count;
    char **elements;
    bool is_null;
} db_Row;

typedef struct
{
    int row_count;
    int col_count;
    char **col_names;
    db_Row *rows;
} db_Table;

// init empty
db_Table *db_newTable(int col_count);
db_Row *db_newRow(int col_count);

// init with data
db_Table *db_fileToTable(char *path, int col_count);
db_Row *db_strToRow(char *str, int col_count);

// inserting
void db_insertRow(db_Table *table, db_Row row);

// printing and saving
void db_printTable(db_Table table);
void db_saveTable(db_Table table, char *path);

// getting value
db_Row *db_selectFirstRowWhere(db_Table table, int col_ref, char *val_ref);
db_Row *db_selectRowWhereId(db_Table table, int ref_id);

int db_getColIdx(db_Table table, char *col_name);
int db_getHighestId(db_Table table);
int db_getHowManyWhere(db_Table table, int col_ref, char *val_ref);
int db_getHighestColumnLength(db_Table table, int col_ref);

// sorting rows
db_Row *db_getSortedRows(db_Table table, int base_idx, bool alpha, bool asc);

#endif