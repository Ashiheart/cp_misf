#define TIMER_VALUE_ONLY
#include "task_scheduling.h"
#include "function-timer.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int pe = argc == 2 ? (int)strtol(argv[1], NULL, 10) : 2;

    int task_len = 0;

    struct Task *task = NULL;

    task_make(stdin, &task_len, &task);

    function_timer(
            set_critical_path(task_len, task);
            cp_misf_taskarray(task_len, task, pe),
            "cp/misf");

    task_destructor(task_len, task);

    return 0;
}
