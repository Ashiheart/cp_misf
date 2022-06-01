#include "task_scheduling.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int pe = argc == 2 ? (int)strtol(argv[1], NULL, 10) : 2;

    cp_misf(stdin, pe);

    return 0;
}
