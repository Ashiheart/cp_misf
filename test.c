#include "task.h"

int main(void)
{
    int task_len;

    struct Task *task;

    make_task(stdin, &task_len, &task);

    struct Task tmp;

    printf("%d %p\n", task[5].id, task[5].predecessor);

    printf("%d %p\n", task[6].id, task[6].predecessor);

    tmp = task[5];

    task[5] = task[6];

    task[6] = tmp;

    struct Task *p;

    /*
    p = *task[5].predecessor;

    *task[5].predecessor = p;

    p = *task[6].predecessor;
    */

    printf("%d %p\n", task[5].id, task[5].predecessor);

    printf("%d %p\n", task[6].id, task[6].predecessor);


    /*
    for(int i = 0; i < task_len; i++) {
        for(int j = i + 1; j < task_len; j++) {
            if(task[i].cp_len > task[j].cp_len) {
                tmp = task[i];
                task[i] = task[j];
                task[j] = tmp;
            }
        }
    }
    */

    show_task(task_len, task);

    task_destructor(task_len, task);

    return 0;
}
