#define TIMER_DEBUG
#include "function-timer.h"

#include <unistd.h>
#include <stdio.h>

int main(void)
{
    function_timer(
        for(int i = 0; i < 10; i++) {
            puts("hello world");
        }
        , "output string stream"
    );

    return 0;
}

