#include <stdio.h>
#include <stdlib.h>

struct t {

    int value;

    int **ary;
};

int main(void)
{
    struct t t1;

    t1.value = 2;
    t1.ary   = (int*)malloc(sizeof(int)*t1.value);
    t1.ary[0] = 1;
    t1.ary[1] = 2;

    struct t t2;

    t2.value = 5;
    t2.ary   = (int*)malloc(sizeof(int)*t2.value);
    t2.ary[0] = 9;
    t2.ary[1] = 10;
    t2.ary[2] = 11;
    t2.ary[3] = 12;
    t2.ary[4] = 13;

    printf("%d, %d %d %d\n", t1.value, t1.ary[0], t1.ary[1], t1.ary[2]);

    printf("%d, %d %d %d\n", t2.value, t2.ary[0], t2.ary[1], t2.ary[2]);

    struct t tmp;

    tmp = t1;
    t1 = t2;
    t2 = tmp;

    printf("%d, %d %d %d\n", t1.value, t1.ary[0], t1.ary[1], t1.ary[2]);

    printf("%d, %d %d %d\n", t2.value, t2.ary[0], t2.ary[1], t2.ary[2]);

    free(t1.ary);
    free(t2.ary);

    return 0;
}
