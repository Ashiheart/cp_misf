#include <stdio.h>
#include <stdbool.h>

#include "function-timer.h"
#include "task.h"
#include "priority_list.h"


static bool is_scheduling_complete(int pe_num, struct Task *assign[pe_num], struct priority_list *head);

static struct Task *assign_task(struct priority_list *head);

static bool is_task_standby(const struct Task *task);

static void show_processor_state(int time, int pe_num, struct Task *assign[pe_num]);

static void simulate_scheduling_processor(struct priority_list *head, int pe_num);


void cp_misf_prioritylist(FILE *fp, int pe_num)
{
    int task_len = 0;

    struct Task *task;

    function_timer(make_task(fp, &task_len, &task), "input");

    struct priority_list head = (struct priority_list) { .value = NULL, .next = NULL };

    function_timer(make_priority_list(task_len, task, &head), "insert_sort");

    //show_critical_path(&head);

    //show_plist(&head);

    //show_task(task_len, task);

    function_timer(simulate_scheduling_processor(&head, pe_num), "scheduling");

    task_destructor(task_len, task);

    plist_destructor(&head);
}


void simulate_scheduling_processor(struct priority_list *head, int pe_num)
{
    struct Task *assign[pe_num]; for(int i = 0; i < pe_num; i++) { assign[i] = NULL; }

    bool HasCompletedTasks = true;

    printf("time #"); for(int i = 0; i < pe_num; i++) printf("%5d", i);
    printf("\n------"); for(int i = 0; i < pe_num; i++) printf("-----"); printf("\n");

    for(int t = 1; !is_scheduling_complete(pe_num, assign, head); t++) {
        // 割当
        for(int i = 0; i < pe_num; i++) {
            if(HasCompletedTasks && assign[i] == NULL) {
                assign[i] = assign_task(head);
                if(assign[i] == NULL) {
                    HasCompletedTasks = false;
                    break;
                }
            }
        }
        //描画
        show_processor_state(t, pe_num, assign);
        //更新
        for(int i = 0; i < pe_num; i++) {
            if(assign[i] != NULL) {
                assign[i]->progress++;
                if(assign[i]->progress == assign[i]->processing_time) {
                    assign[i] = NULL;
                    HasCompletedTasks = true;
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
        if(task->predecessor[i]->processing_time - task->predecessor[i]->progress > 0) return false;
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

void show_processor_state(int time, int pe_num, struct Task *assign[pe_num]) 
{
    printf("%4d #", time);
    for(int i = 0; i < pe_num; i++) {
        if(assign[i] != NULL) {
            if(assign[i]->progress == 0) {
                printf("%5d", assign[i]->id);
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

