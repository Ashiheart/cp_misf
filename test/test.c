#include "task_scheduling.h"
#include "task.h"
#include "priority_list.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int pe = argc == 2 ? (int)strtol(argv[1], NULL, 10) : 2;

    int task_len = 0;

    struct Task *task = NULL;

    task_make(stdin, &task_len, &task);

    task_export_graph(task_len, task, "graph.gv");

    struct priority_list head = (struct priority_list) { .value = NULL, .next = NULL };

    cp_misf_prioritylist(task_len, task, pe);

    plist_make(task_len, task, &head);

    plist_show(&head);

    plist_destructor(&head);

    return 0;
}
