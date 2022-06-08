#pragma once

#include "task.h"

struct priority_list
{
    struct Task*            value;

    struct priority_list*   next;
};

void plist_make(int n, struct Task task[n], struct priority_list *head);

void plist_destructor(struct priority_list *head);

struct Task *plist_pop(struct priority_list *ptr);

void plist_show(struct priority_list *head);

void plist_show_cpath(struct priority_list *head);
