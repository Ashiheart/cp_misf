#pragma once

#include "task.h"

struct priority_list
{
    struct Task*            value;

    struct priority_list*   next;
};

void make_priority_list(int n, struct Task task[n], struct priority_list *head);

void plist_destructor(struct priority_list *head);

void show_plist(struct priority_list *head);

void show_critical_path(struct priority_list *head);

