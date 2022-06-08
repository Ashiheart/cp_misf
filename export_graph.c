#include "task.h"
#include <stdio.h>

int main(void)
{
    int n = 0;

    struct Task *task = NULL;

    task_make(stdin, &n, &task);

    task_export_graph(n, task, "graph.gv");

    task_destructor(n, task);
}
