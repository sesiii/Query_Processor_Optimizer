#include "stats.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#define MAX_TABLES 10
#define MAX_COLUMNS_PER_TABLE 10

TableStats *tables[MAX_TABLES];
int table_count = 0;

// Helper function to create a new column stat
static ColumnStats* create_column_stat(const char *table, const char *column, 
                                     int distinct, int min, int max, double sel) {
    ColumnStats *stat = malloc(sizeof(ColumnStats));
    stat->table = strdup(table);
    stat->column = strdup(column);
    stat->distinct_values = distinct;
    stat->min_value = min;
    stat->max_value = max;
    stat->selectivity = sel;
    return stat;
}

// Initialize hardcoded statistics for testing
void init_stats() {
    printf("Initializing statistics with hardcoded values\n");

    // Employee table statistics
    TableStats *employees = malloc(sizeof(TableStats));
    employees->name = strdup("employees");
    employees->row_count = 10000;
    employees->column_count = 4;
    employees->size_in_bytes = employees->row_count * 40;
    employees->columns = malloc(employees->column_count * sizeof(ColumnStats *));

    employees->columns[0] = create_column_stat("employees", "emp_id", 10000, 1, 10000, 0.0001);
    employees->columns[1] = create_column_stat("employees", "name", 9500, 0, 0, 0.00011);
    employees->columns[2] = create_column_stat("employees", "dept_id", 20, 1, 20, 0.05);
    employees->columns[3] = create_column_stat("employees", "salary", 1000, 30000, 150000, 0.001);
    tables[table_count++] = employees;

    // Departments table statistics
    TableStats *departments = malloc(sizeof(TableStats));
    departments->name = strdup("departments");
    departments->row_count = 20;
    departments->column_count = 3;
    departments->size_in_bytes = departments->row_count * 50;
    departments->columns = malloc(departments->column_count * sizeof(ColumnStats *));

    departments->columns[0] = create_column_stat("departments", "dept_id", 20, 1, 20, 0.05);
    departments->columns[1] = create_column_stat("departments", "dept_name", 20, 0, 0, 0.05);
    departments->columns[2] = create_column_stat("departments", "location", 10, 0, 0, 0.1);
    tables[table_count++] = departments;

    // Salaries table statistics
    TableStats *salaries = malloc(sizeof(TableStats));
    salaries->name = strdup("salaries");
    salaries->row_count = 10000;
    salaries->column_count = 4;
    salaries->size_in_bytes = salaries->row_count * 25;
    salaries->columns = malloc(salaries->column_count * sizeof(ColumnStats *));

    salaries->columns[0] = create_column_stat("salaries", "emp_id", 10000, 1, 10000, 0.0001);
    salaries->columns[1] = create_column_stat("salaries", "salary", 1000, 30000, 150000, 0.001);
    salaries->columns[2] = create_column_stat("salaries", "year", 10, 2013, 2023, 0.1);
    salaries->columns[3] = create_column_stat("salaries", "bonus", 500, 0, 50000, 0.002);
    tables[table_count++] = salaries;

    // Projects table statistics
    TableStats *projects = malloc(sizeof(TableStats));
    projects->name = strdup("projects");
    projects->row_count = 5000;
    projects->column_count = 4;
    projects->size_in_bytes = projects->row_count * 60;
    projects->columns = malloc(projects->column_count * sizeof(ColumnStats *));

    projects->columns[0] = create_column_stat("projects", "project_id", 5000, 1, 5000, 0.0002);
    projects->columns[1] = create_column_stat("projects", "project_name", 2500, 0, 0, 0.0004);
    projects->columns[2] = create_column_stat("projects", "dept_id", 20, 1, 20, 0.05);
    projects->columns[3] = create_column_stat("projects", "budget", 1000, 10000, 1000000, 0.001);
    tables[table_count++] = projects;

    printf("Statistics initialized for %d tables\n", table_count);
}

void free_stats() {
    for (int i = 0; i < table_count; i++) {
        for (int j = 0; j < tables[i]->column_count; j++) {
            free(tables[i]->columns[j]->table);
            free(tables[i]->columns[j]->column);
            free(tables[i]->columns[j]);
        }
        free(tables[i]->columns);
        free(tables[i]->name);
        free(tables[i]);
    }
    table_count = 0;
}

TableStats *get_table_stats(const char *table_name) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(tables[i]->name, table_name) == 0) {
            return tables[i];
        }
    }
    printf("Warning: No statistics found for table %s\n", table_name);
    return NULL;
}

ColumnStats *get_column_stats(const char *table_name, const char *column_name) {
    TableStats *table = get_table_stats(table_name);
    if (!table) return NULL;

    for (int i = 0; i < table->column_count; i++) {
        if (strcmp(table->columns[i]->column, column_name) == 0) {
            return table->columns[i];
        }
    }

    printf("Warning: No statistics found for column %s.%s\n", table_name, column_name);
    return NULL;
}

double calculate_join_selectivity(const char *table1, const char *column1, 
                                const char *table2, const char *column2) {
    ColumnStats *stats1 = get_column_stats(table1, column1);
    ColumnStats *stats2 = get_column_stats(table2, column2);

    if (!stats1 || !stats2) {
        TableStats *t1 = get_table_stats(table1);
        TableStats *t2 = get_table_stats(table2);
        double default_sel = 1.0 / fmax(t1 ? t1->row_count : 1000, t2 ? t2->row_count : 1000);
        printf("No stats for %s.%s or %s.%s, using default selectivity: %f\n",
              table1, column1, table2, column2, default_sel);
        return default_sel;
    }

    // For primary key - foreign key joins, selectivity is 1/max(distinct_values)
    return 1.0 / fmax(stats1->distinct_values, stats2->distinct_values);
}

double calculate_condition_selectivity(const char *table, const char *column,
                                     const char *op, int value) {
    ColumnStats *stats = get_column_stats(table, column);
    if (!stats) return 1.0;

    // Handle string comparisons (for columns like dept_name)
    if (stats->min_value == 0 && stats->max_value == 0) {
        return 1.0 / stats->distinct_values;
    }

    double range = stats->max_value - stats->min_value;
    if (range <= 0) return 0.5;

    if (strcmp(op, "=") == 0) {
        return 1.0 / stats->distinct_values;
    }
    else if (strcmp(op, "<") == 0 || strcmp(op, "<=") == 0) {
        double fraction = (value - stats->min_value) / range;
        return fraction > 1.0 ? 1.0 : (fraction < 0.0 ? 0.0 : fraction);
    }
    else if (strcmp(op, ">") == 0 || strcmp(op, ">=") == 0) {
        double fraction = (stats->max_value - value) / range;
        return fraction > 1.0 ? 1.0 : (fraction < 0.0 ? 0.0 : fraction);
    }

    return 0.5;
}

int estimate_result_size(const char *table_name, double selectivity) {
    TableStats *stats = get_table_stats(table_name);
    if (!stats) return 0;
    return (int)(stats->row_count * selectivity);
}