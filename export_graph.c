#include "task.h"
#include <stdio.h>

int main(void)
{
    int n = 0;

    struct Task *task = NULL;

    make_task(stdin, &n, &task);

    export_task_graph(n, task, "graph.gv");

    task_destructor(n, task);
}
