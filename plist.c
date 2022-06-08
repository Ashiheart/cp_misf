#include "task_scheduling.h"
#include <stdlib.h>

#include "function-timer.h"

int main(int argc, char *argv[])
{
    int pe = argc == 2 ? (int)strtol(argv[1], NULL, 10) : 2;

    int task_len = 0;

    struct Task *task = NULL;

    task_make(stdin, &task_len, &task);

    function_timer(cp_misf_prioritylist(task_len, task, pe), "cp/misf");

    fprintf(stderr, "\n");

    return 0;
}
