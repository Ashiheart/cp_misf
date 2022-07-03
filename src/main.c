#include <stdio.h>

typedef int* priority_array;

void f(int n, int *x) {
    for(int i = 0; i < n; i++) {
        printf("%d\n", x[i]);
    }
}

int main(void)
{
    int x[10]; for(int i = 0; i < 10; i++) { x[i] = i * i; }

    void *p = x;

    f(10, p);

    return 0;
}


