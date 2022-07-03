#define _GNU_SOURCE
#define TIMER_VALUE_ONLY
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "task.h"
#include "priority_list.h"

#include "function-timer.h"

struct task_distributor {

    int size;

    void *plist;

    struct Task *(*task_assign)(int, void*);

};

struct {

    int pe_num;

    int time;

    struct Task *task;

} assign;

static void task_scheduling(struct task_distributor *td, int pe_num);

static bool task_assign_processors(int pe_num, struct Task *assign[pe_num], struct task_distributor *td);

static int update_processors(int pe_num, struct Task *assign[pe_num]);

static struct Task *assign_task           (int n, void *vhead);

static struct Task *assign_task_from_array(int n, void *vlist);


static bool is_task_standby(const struct Task *task);

static void show_processor_state(int time, int pe_num, struct Task *assign[pe_num]);

static int cmp(const void *lhs, const void *rhs);

void show_scheduling(void)
{
    int pe_num = 0;
    printf("time #"); for(int i = 0; i < pe_num; i++) printf("%5d", i);
    printf("\n------"); for(int i = 0; i < pe_num; i++) printf("-----"); printf("\n");

}


void cp_misf_prioritylist(int task_len, struct Task *task, int pe_num)
{
    set_critical_path(task_len, task);

    struct priority_list head = (struct priority_list) { .value = NULL, .next = NULL };

    function_timer(plist_make(task_len, task, &head), "make_plist");

    struct task_distributor td = { 
        .size = task_len - 2,
        .plist = &head,
        .task_assign = assign_task,
    };

    task_scheduling(&td, pe_num);

    //function_timer(simulate_scheduling_processor(&head, pe_num), "scheduling");

    plist_destructor(&head);
}

void cp_misf_taskarray(int task_len, struct Task *task, int pe_num)
{
    set_critical_path(task_len, task);

    for(int i = 1; i < task_len - 1; i++) { task[i].progress = -1; }

    struct Task *plist[task_len - 2]; 

    for(int i = 0; i < task_len - 2; i++) { plist[i] = &task[i + 1]; }

    function_timer(qsort(plist, task_len - 2, sizeof(struct Task*), cmp), "quick_sort");

    struct task_distributor td = { 
        .size = task_len - 2,
        .plist = &plist,
        .task_assign = assign_task_from_array,
    };

    task_scheduling(&td, pe_num);
}

void task_scheduling(struct task_distributor *td, int pe_num)
{
    struct Task *assign[pe_num]; for(int i = 0; i < pe_num; i++) { assign[i] = NULL; }

    int complete_tasks = 0;
    int now_complete_tasks = 0;
    bool has_plist_update = true;

    for(int t = 1; complete_tasks != td->size; t++) {
        if(has_plist_update) {
            has_plist_update = task_assign_processors(pe_num, assign, td);
        }
        show_processor_state(t, pe_num, assign);
        now_complete_tasks = update_processors(pe_num, assign);
        if(now_complete_tasks) {
            complete_tasks += now_complete_tasks;
            has_plist_update = true;
        }
    }
}

bool task_assign_processors(int n, struct Task *assign[n], struct task_distributor *td)
{
    for(int i = 0; i < n; i++) {
        if(assign[i] == NULL) {
            assign[i] = (*td->task_assign)(td->size, td->plist);
            if(assign[i] == NULL) {
                return false;
            }
        }
    }

    return true;
}

int update_processors(int pe_num, struct Task *assign[pe_num])
{
    int complete_tasks = 0;

    for(int i = 0; i < pe_num; i++) {
        if(assign[i] != NULL) {
            assign[i]->progress++;
            if(assign[i]->progress == assign[i]->processing_time) {
                assign[i] = NULL;
                complete_tasks++;
            }
        }
    }

    return complete_tasks;
}

bool is_task_standby(const struct Task *task)
{
    for(int i = 0; i < task->predecessor_size; i++) {
        if(task->predecessor[i]->processing_time != task->predecessor[i]->progress) {
            return false;
        }
    }

    return true;
}

struct Task *assign_task(int n, void *vhead)
{  
    struct priority_list *head = (struct priority_list*)(vhead);

    for(struct priority_list *ptr = head; ptr->next != NULL; ptr = ptr->next) {
        if(is_task_standby(ptr->next->value)) {
            return plist_pop(ptr);
        }
    }
    return NULL;
}

struct Task *assign_task_from_array(int n, void *vlist)
{
    struct Task **plist = (struct Task**)(vlist);

    for(int i = 0; i < n; i++) {
        if(is_task_standby(plist[i]) && plist[i]->progress == -1) {
            plist[i]->progress = 0;
            return plist[i];
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
    struct Task *tlhs = *((struct Task**)lhs);
    struct Task *trhs = *((struct Task**)rhs);

    int val = trhs->cp_len - tlhs->cp_len;

    //misf
    if(val != 0) {
        return val;
    } else {
        return trhs->successors - tlhs->successors;
    }
}

int cmp_r(const void *lhs, const void *rhs, void *task)
{
    // cmp order by task->cp_len and task->successors
    return cmp(&((struct Task *)task)[*((int*)lhs)], &((struct Task *)task)[*((int*)rhs)]);
}
