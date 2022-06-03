#include "task_scheduling.h"
#include <stdlib.h>

#include "function-timer.h"

int main(int argc, char *argv[])
{
    int pe = argc == 2 ? (int)strtol(argv[1], NULL, 10) : 2;

    function_timer(cp_misf_prioritylist(stdin, pe), "cp/misf");

    fprintf(stderr, "\n");

    return 0;
}
