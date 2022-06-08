#include "task.h"
#include "priority_list.h"
#include <stdio.h>

int main(void)
{
    int n = 0;

    struct Task *task = NULL;

    task_make(stdin, &n, &task);

    //task_export_graph(n, task, "graph.gv");

    struct priority_list head = (struct priority_list) { .value = NULL, .next = NULL };

    plist_make(n, task, &head);

    head->value = (struct priority_list*)malloc(sizeof(struct priority_list*)*n);

    plist_show(&head);

    plist_destructor(&head);

    task_destructor(n, task);
}
