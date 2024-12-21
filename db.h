/*
 *  db.h
 *
 *  database-like structure with table and row
 *  simple yet efficient
 *
 *  Author: Muhammad Daffa Rahman
 */

#ifndef _DB_H_
#define _DB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

// row...
typedef struct
{
    int id;
    int col_count;
    char **elements;
    bool is_null;
} db_Row;

// we call this revolutionary (?), because we only need 2 structs here, db_Row and this
// and we use these 2 for all of tables
// every tables has rows, and it has a size info so you know how many stuff you got
typedef struct
{
    int row_count;
    int col_count;
    char **col_names;
    db_Row *rows;
} db_Table;

// create new table with fixed column size
db_Table *db_newTable(int col_count);
// create new row with fixed column size
db_Row *db_newRow(int col_count);

// basically import table from a file
db_Table *db_fileToTable(char *path, int col_count);
// because the format of each line is a string, we use this to convert each line to row
db_Row *db_strToRow(char *str, int col_count);

// insert a row into table
void db_insertRow(db_Table *table, db_Row row);

// print table with SQL like look
void db_printTable(db_Table table);
// save the table to the file path
void db_saveTable(db_Table table, char *path);

// selecting the first row or basically first occurences where....
db_Row *db_selectFirstRowWhere(db_Table table, int col_ref, char *val_ref);
// select the row by id
db_Row *db_selectRowWhereId(db_Table table, int ref_id);

// get the index of a column, please guys always use this
int db_getColIdx(db_Table table, char *col_name);
// get the highest id in the table, use this to do "autoincrement"
int db_getHighestId(db_Table table);
// get how many occurences where....
int db_getHowManyWhere(db_Table table, int col_ref, char *val_ref);
// get the highest strlen() of a value on a column, used on table printing to make it more pretty
int db_getHighestColumnLength(db_Table table, int col_ref);

// returns sorted rows based on the base_idx (column index), alphabetically? ascending?
db_Row *db_getSortedRows(db_Table table, int base_idx, bool alpha, bool asc);

#endif