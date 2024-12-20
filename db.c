#include "db.h"

// "private" static variables for sorting stuff
static int m_sort_idx = 0;
static bool m_sort_int = false;
static bool m_sort_asc = false;

// defining the prototype here can technically make it private
int compareRow(const void *a, const void *b);

// allocating new table
db_Table *db_newTable(int col_count)
{
    db_Table *new_table = (db_Table *)malloc(sizeof(db_Table));
    new_table->col_count = col_count;
    new_table->row_count = 0;
    new_table->rows = NULL;
    new_table->col_names = (char **)malloc(sizeof(char *) * col_count);
    for (int i = 0; i < col_count; i++)
        new_table->col_names[i] = (char *)malloc(sizeof(char) * 255);
    return new_table;
}

// allocating new row
db_Row *db_newRow(int col_count)
{
    db_Row *new_row = (db_Row *)malloc(sizeof(db_Row));
    new_row->col_count = col_count;
    new_row->id = 0;
    new_row->is_null = false;
    new_row->elements = (char **)malloc(sizeof(char *) * col_count);
    for (int i = 0; i < col_count; i++)
        new_row->elements[i] = (char *)malloc(sizeof(char) * 255);
    return new_row;
}

// parse new table from file
db_Table *db_fileToTable(char *path, int col_count)
{
    db_Table *new_table = db_newTable(col_count);

    FILE *fptr = fopen(path, "r");
    if (!fptr)
        return NULL;

    char *buff = (char *)malloc(sizeof(char) * 255);

    int i = 0;
    while (fgets(buff, 255, fptr))
    {
        // get the table header first
        if (i == 0)
        {
            // always strdup whenever strtok-ing
            char *token = strtok(strdup(buff), "|");
            if (!token)
                break;

            for (int i = 0; i < new_table->col_count; i++)
            {
                token = strtok(NULL, "|");
                new_table->col_names[i] = token;
            }
        }
        // get the table data
        else
        {
            db_insertRow(new_table, *db_strToRow(buff, new_table->col_count));
        }
        i++;
    }

    fclose(fptr);
    return new_table;
}

// converts char* into a db_Row* with col_count
db_Row *db_strToRow(char *str, int col_count)
{
    db_Row *new_row = db_newRow(col_count);
    new_row->id = atoi(strtok(strdup(str), "|"));

    for (int i = 0; i < col_count; i++)
    {
        new_row->elements[i] = strtok(NULL, "|");
    }

    return new_row;
}

// insert a row into a table
void db_insertRow(db_Table *table, db_Row row)
{
    table->row_count += 1;
    table->rows = (db_Row *)realloc(table->rows, sizeof(db_Row) * table->row_count);
    table->rows[table->row_count - 1] = row;
}

// print the table
void db_printTable(db_Table table)
{
    int *col_length = (int *)malloc(sizeof(int) * (table.col_count + 1));
    int total_table_length = 0;
    col_length[0] = util_strLength(util_intToStr(db_getHighestId(table)));
    if (col_length[0] <= 1)
        col_length[0] = 2;
    total_table_length += col_length[0];
    for (int i = 1; i < table.col_count + 1; i++)
    {
        col_length[i] = db_getHighestColumnLength(table, i - 1);
        total_table_length += col_length[i];
    }

    // bars
    for (int i = 0; i < table.col_count + 1; i++)
    {
        printf("+");
        for (int j = 0; j < col_length[i] + 2; j++)
            printf("-");
    }
    printf("+\n");

    printf("| %-*s |", col_length[0], "id");
    for (int i = 0; i < table.col_count; i++)
        printf(" %-*s |", col_length[i + 1], table.col_names[i]);

    // bars
    printf("\n");
    for (int i = 0; i < table.col_count + 1; i++)
    {
        printf("+");
        for (int j = 0; j < col_length[i] + 2; j++)
            printf("-");
    }
    printf("+");

    for (int i = 0; i < table.row_count; i++)
    {
        if (!table.rows[i].is_null)
        {
            printf("\n| %-*d |", col_length[0], table.rows[i].id);
            for (int j = 0; j < table.col_count; j++)
                printf(" %-*s |", col_length[j + 1], table.rows[i].elements[j]);
        }
    }

    printf("\n");
    for (int i = 0; i < table.col_count + 1; i++)
    {
        printf("+");
        for (int j = 0; j < col_length[i] + 2; j++)
            printf("-");
    }
    printf("+\n");
}

// save table to a file and overwrites existing file if already exist
void db_saveTable(db_Table table, char *path)
{
    FILE *fptr = fopen(path, "w");
    if (!fptr)
        return;

    fprintf(fptr, "|id|");
    for (int i = 0; i < table.col_count; i++)
        fprintf(fptr, "%s|", table.col_names[i]);

    for (int i = 0; i < table.row_count; i++)
        if (!table.rows[i].is_null)
        {
            fprintf(fptr, "\n|%d|", table.rows[i].id);
            for (int j = 0; j < table.col_count; j++)
                fprintf(fptr, "%s|", table.rows[i].elements[j]);
        }

    fclose(fptr);
}

// get row where column col_ref value is val_ref
db_Row *db_selectFirstRowWhere(db_Table table, int col_ref, char *val_ref)
{
    for (int i = 0; i < table.row_count; i++)
        if (!table.rows[i].is_null)
            if (strcmp(table.rows[i].elements[col_ref], val_ref) == 0)
                return &table.rows[i];

    return NULL;
}

db_Row *db_selectRowWhereId(db_Table table, int ref_id)
{
    for (int i = 0; i < table.row_count; i++)
        if (!table.rows[i].is_null)
            if (table.rows[i].id == ref_id)
                return &table.rows[i];

    return NULL;
}

// get the index from col_names
int db_getColIdx(db_Table table, char *col_name)
{
    for (int i = 0; i < table.col_count; i++)
        if (strcmp(table.col_names[i], col_name) == 0)
            return i;
    return -1;
}

// get the highest id from all of the non is_null rows
int db_getHighestId(db_Table table)
{
    int highest = 0;
    for (int i = 0; i < table.row_count; i++)
        if (!table.rows[i].is_null)
            if (table.rows[i].id > highest)
                highest = table.rows[i].id;
    return highest;
}

// get how many occurences row where column value is this
int db_getHowManyWhere(db_Table table, int col_ref, char *val_ref)
{
    int res = 0;
    for (int i = 0; i < table.row_count; i++)
    {
        if (!table.rows[i].is_null)
        {
            if (strcmp(table.rows[i].elements[col_ref], val_ref) == 0)
                res++;
        }
    }
    return res;
}

// get highest strlen of a column
int db_getHighestColumnLength(db_Table table, int col_ref)
{
    int res = 0;
    for (int i = 0; i < table.row_count; i++)
    {
        if (!table.rows[i].is_null)
        {
            if (util_strLength(table.rows[i].elements[col_ref]) > res)
                res = util_strLength(table.rows[i].elements[col_ref]);

            if (util_strLength(table.col_names[col_ref]) > res)
                res = util_strLength(table.col_names[i]);
        }
    }
    return res;
}

// returns sorted row
db_Row *db_getSortedRows(db_Table table, int base_idx, bool alpha, bool asc)
{
    db_Row *sorted_rows = (db_Row *)malloc(sizeof(db_Row) * table.row_count);
    for (int i = 0; i < table.row_count; i++)
        sorted_rows[i] = table.rows[i];

    m_sort_idx = base_idx;
    m_sort_int = !alpha;
    m_sort_asc = asc;

    qsort(sorted_rows, table.row_count, sizeof(db_Row), compareRow);

    return sorted_rows;
}

// basically comparing two rows, used with qsort function
int compareRow(const void *a, const void *b)
{
    db_Row *row_a = (db_Row *)a;
    db_Row *row_b = (db_Row *)b;

    // if the flag set to sort a number
    if (m_sort_int)
    {
        int val_a = atoi(row_a->elements[m_sort_idx]);
        int val_b = atoi(row_b->elements[m_sort_idx]);

        if (m_sort_asc)
            return val_a - val_b;
        else
            return val_b - val_a;
    }
    // sort a string
    else
    {
        if (m_sort_asc)
            return strcmp(row_a->elements[m_sort_idx], row_b->elements[m_sort_idx]);
        else
            return strcmp(row_b->elements[m_sort_idx], row_a->elements[m_sort_idx]);
    }
}