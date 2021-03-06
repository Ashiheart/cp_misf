#define _GNU_SOURCE
#include <stdlib.h>

#include "priority_list.h"
#include "task.h"

static int cmp(const void *lhs, const void *rhs);

static int cmp_r(const void *lhs, const void *rhs, void *task);

static void plist_show_cpath_recursive(struct priority_list *tar);


void plist_make(int n, struct Task task[n], struct priority_list *head)
{
    int plist_size = n - 2;

    int id_plist[plist_size];

    for(int i = 0; i < plist_size; i++) { id_plist[i] = i + 1; }

    qsort_r(id_plist, plist_size, sizeof(int), cmp_r, task);

    struct priority_list *tar = NULL;

    struct priority_list *tail = head;

    for(int i = 0; i < plist_size; i++, tail = tail->next) {

        tar  = (struct priority_list*)malloc(sizeof(struct priority_list));

        *tar = (struct priority_list) { .value = &task[id_plist[i]], .next = NULL };

        tail->next = tar;
    }
}

void plist_insertion_sort(int n, struct Task task[n], struct priority_list *head)
{
    struct priority_list *tmp = NULL;
    struct priority_list *tar = NULL;

    // i = 0, (n - 1) is dummy tasks.
    for(int i = n - 2; i > 0; i--) {

        tar  = (struct priority_list*)malloc(sizeof(struct priority_list));

        *tar = (struct priority_list) { .value = &task[i], .next = NULL };

        for(struct priority_list *ptr = head; ptr != NULL; ptr = ptr->next) {
            if(ptr->next == NULL || cmp(ptr->next->value, tar->value) >= 0) {
                tmp = ptr->next;
                ptr->next = tar;
                tar->next = tmp;
                break;
            }
        }
    }

}

void plist_destructor(struct priority_list *head)
{
    struct priority_list *next;
    for(struct priority_list *ptr = head->next; ptr != NULL; ptr = next) {
        next = ptr->next;
        free(ptr);
    }
}

struct Task *plist_pop(struct priority_list *prev)
{
    struct priority_list *tar = prev->next;
    struct Task *value = tar->value;

    prev->next = prev->next->next;
    free(tar);
    return value;
}

void plist_show(struct priority_list *head)
{
    for(struct priority_list *ptr = head->next; ptr != NULL; ptr = ptr->next) {
        printf("id:%3d, pt:%3d, cp_len:%3d, after task:%3d\n", 
                ptr->value->id, ptr->value->processing_time, ptr->value->cp_len, ptr->value->successors);
    }
}

void plist_show_cpath(struct priority_list *head)
{
    puts("id(critical_path, processing_time)");
    plist_show_cpath_recursive(head->next);
    puts("");
}

void plist_show_cpath_recursive(struct priority_list *tar)
{
    if(tar == NULL) return;

    printf("%d(c%d,p%d)", tar->value->id, tar->value->cp_len, tar->value->processing_time);

    if(tar->value->cp_len - tar->value->processing_time == 0) {
        return;
    }

    printf(" -> ");

    for(struct priority_list *ptr = tar; ptr != NULL; ptr = ptr->next) {
        if(ptr->value->cp_len == tar->value->cp_len - tar->value->processing_time) {
            plist_show_cpath_recursive(ptr);
            return ;
        }
    }
}

int cmp(const void *lhs, const void *rhs)
{
    int val = ((struct Task*)rhs)->cp_len - ((struct Task*)lhs)->cp_len;

    //misf
    if(val != 0) {
        return val;
    } else {
        return ((struct Task*)rhs)->successors - ((struct Task*)lhs)->successors;
    }
}

int cmp_r(const void *lhs, const void *rhs, void *task)
{
    // cmp order by task->cp_len and task->successors
    return cmp(&((struct Task *)task)[*((int*)lhs)], &((struct Task *)task)[*((int*)rhs)]);
}

