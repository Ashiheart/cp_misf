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

void make_task(FILE *fp, int *n, struct Task **task);

void task_destructor(int n, struct Task task[n]);

void show_task(int n, struct Task task[n]);

// ex) dot -Tpng graph.gv -o graph.png
void export_task_graph(int n, struct Task task[n], char *export_file_name);


