#pragma once

#include <stdio.h>

struct Task
{
    int     id;
    int     processing_time;

    int     cp_len;
    int     successors;

    int     progress;

    int             predecessor_size;
    struct Task**   predecessor;
};

void task_make(FILE *fp, int *n, struct Task **task);

void task_destructor(int n, struct Task task[n]);

void task_show(int n, struct Task task[n]);

// ex) dot -Tpng graph.gv -o graph.png
void task_export_graph(int n, struct Task task[n], char *export_file_name);


