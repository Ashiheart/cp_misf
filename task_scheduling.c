#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "function-timer.h"
#include "task.h"
#include "priority_list.h"


static bool is_scheduling_complete(int pe_num, struct Task *assign[pe_num], struct priority_list *head);

static struct Task *assign_task           (struct priority_list *head);

static struct Task *assign_task_from_array(int n, struct Task task[n], int *plist_index);


static bool is_task_standby(const struct Task *task);

static void show_processor_state(int time, int pe_num, struct Task *assign[pe_num]);

static void simulate_scheduling_processor(struct priority_list *head, int pe_num);

static void simulate_scheduling_processor_taskarray(int n, struct Task task[n], int *plist_index, int pe_num);

static int cmp(const void *lhs, const void *rhs);

static int cmp_r(const void *lhs, const void *rhs, void *task);

void cp_misf_prioritylist(FILE *fp, int pe_num)
{
    int task_len = 0;

    struct Task *task;

    function_timer(make_task(fp, &task_len, &task), "input");

    struct priority_list head = (struct priority_list) { .value = NULL, .next = NULL };

    function_timer(make_priority_list(task_len, task, &head), "insert_sort");

    function_timer(simulate_scheduling_processor(&head, pe_num), "scheduling");

    show_plist(&head);

    show_task(task_len, task);


    task_destructor(task_len, task);

    plist_destructor(&head);
}

void cp_misf_taskarray(FILE *fp, int pe_num)
{
    int task_len = 0;

    struct Task *task;

    function_timer(make_task(fp, &task_len, &task), "input");

    for(int i = 1; i < task_len - 1; i++) { task[i].progress = -1; }

    int id_plist[task_len - 2]; for(int i = 0; i < task_len - 2; i++) { id_plist[i] = i + 1; }

    function_timer(qsort_r(id_plist, task_len - 2, sizeof(int), cmp_r, task), "quick_sort");

    function_timer(simulate_scheduling_processor_taskarray(task_len, task, id_plist, pe_num), "scheduling");

    show_task(task_len, task);
    
    task_destructor(task_len, task);
}

void cp_misf(FILE *stg, int pe_num)
{
    function_timer(cp_misf_taskarray(stg, pe_num), "cp/misf by taskarray");

    //function_timer(cp_misf_prioritylist(stg, pe_num), "cp/misf by prioritylist");
}



void simulate_scheduling_processor_taskarray(int n, struct Task task[n], int *plist_index, int pe_num)
{
    struct Task *assign[pe_num]; for(int i = 0; i < pe_num; i++) { assign[i] = NULL; }

    printf("time #"); for(int i = 0; i < pe_num; i++) printf("%5d", i);
    printf("\n------"); for(int i = 0; i < pe_num; i++) printf("-----"); printf("\n");

    int complete_task_counter = 0;

    bool has_complete_tasks = true;

    for(int t = 1; complete_task_counter != n - 2; t++) {
        for(int i = 0; i < pe_num; i++) {
            if(has_complete_tasks && assign[i] == NULL) {
                assign[i] = assign_task_from_array(n, task, plist_index);
                if(assign[i] == NULL) {
                    has_complete_tasks = false;
                    break;
                }
            }
        }
        show_processor_state(t, pe_num, assign);
        for(int i = 0; i < pe_num; i++) {
            if(assign[i] != NULL) {
                assign[i]->progress++;
                if(assign[i]->progress == assign[i]->processing_time) {
                    assign[i] = NULL;
                    complete_task_counter++;
                    has_complete_tasks = true;
                }
            }
        }
    }

}


void simulate_scheduling_processor(struct priority_list *head, int pe_num)
{
    struct Task *assign[pe_num]; for(int i = 0; i < pe_num; i++) { assign[i] = NULL; }

    bool has_complete_tasks = true;

    printf("time #"); for(int i = 0; i < pe_num; i++) printf("%5d", i);
    printf("\n------"); for(int i = 0; i < pe_num; i++) printf("-----"); printf("\n");

    for(int t = 1; !is_scheduling_complete(pe_num, assign, head); t++) {
        for(int i = 0; i < pe_num; i++) {
            if(has_complete_tasks && assign[i] == NULL) {
                assign[i] = assign_task(head);
                if(assign[i] == NULL) {
                    has_complete_tasks = false;
                    break;
                }
            }
        }
        show_processor_state(t, pe_num, assign);
        for(int i = 0; i < pe_num; i++) {
            if(assign[i] != NULL) {
                assign[i]->progress++;
                if(assign[i]->progress == assign[i]->processing_time) {
                    assign[i] = NULL;
                    has_complete_tasks = true;
                }
            }
        }
    }
}

bool is_scheduling_complete(int pe_num, struct Task *assign[pe_num], struct priority_list *head)
{
    if(head->next != NULL) {
        return false;
    }

    for(int i = 0; i < pe_num; i++) {
        if(assign[i] != NULL) {
            return false;
        }
    }

    return true;
}

bool is_task_standby(const struct Task *task)
{
    for(int i = 0; i < task->predecessor_size; i++) {
        //printf("\t%d %d\n", task->predecessor[i]->processing_time, task->predecessor[i]->progress);
        if(task->predecessor[i]->processing_time != task->predecessor[i]->progress) return false;
    }

    return true;
}

struct Task *assign_task(struct priority_list *head)
{  
    struct Task *assign = NULL;
    for(struct priority_list *ptr = head; ptr->next != NULL; ptr = ptr->next) {
        if(is_task_standby(ptr->next->value)) {
            assign = ptr->next->value;
            ptr->next = ptr->next->next;
            return assign;
        }
    }
    return NULL;
}

struct Task *assign_task_from_array(int n, struct Task task[n], int *plist_index)
{
    for(int i = 0; i < n - 2; i++) {
        if(is_task_standby(task + plist_index[i]) && task[plist_index[i]].progress == -1) {
            task[plist_index[i]].progress = 0;
            return &task[plist_index[i]];
        }
    }

    return  NULL;
}

void show_processor_state(int time, int pe_num, struct Task *assign[pe_num]) 
{
    printf("%4d #", time);
    for(int i = 0; i < pe_num; i++) {
        if(assign[i] != NULL) {
            if(assign[i]->progress == 0) { printf("%5d", assign[i]->id);
            } else if(assign[i]->processing_time - assign[i]->progress == 1) {
                printf("%5s", "v");
            } else {
                printf("%5s", "|");
            }
        } else {
            printf("%5s", ".");
        }
    }
    puts("");
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

static int cmp_r(const void *lhs, const void *rhs, void *task)
{
    return cmp(&((struct Task *)task)[*((int*)lhs)], &((struct Task *)task)[*((int*)rhs)]);
}
