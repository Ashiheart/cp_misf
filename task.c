#include "task.h"
#include "function-timer.h"

#include <stdlib.h>
#include <stdio.h>

static void set_critical_path(int n, struct Task task[n]);

static int pop_strtoi(char **str);

void make_task(FILE *fp, int *n, struct Task **task)
{
    static const int Width = 12;

    if(fscanf(fp, "%d", n) != 1) { exit(-1); } while(fgetc(fp) != '\n'); *n += 2;

    *task = (struct Task*)malloc(sizeof(struct Task) * (*n));

    char str[Width * (*n + 2)];

    char *ptr = NULL;

    for(int i = 0; i < *n; i++) {

        if((ptr = fgets(str, sizeof(str), fp)) == NULL) exit(-1);

        (*task)[i] = (struct Task) {
            .id                 = pop_strtoi(&ptr),
            .processing_time    = pop_strtoi(&ptr),
            .predecessor_size   = pop_strtoi(&ptr),
            .successors         = 0,
            .cp_len             = 0,
            .progress           = 0,
        };

        (*task)[i].predecessor = (struct Task**)malloc(sizeof(struct Task*)*(*task)[i].predecessor_size);

        for(int j = 0; j < (*task)[i].predecessor_size; j++) {
            (*task)[i].predecessor[j]    = &(*task)[pop_strtoi(&ptr)];
        }
    }

    function_timer(set_critical_path(*n, *task), "cpath");
}

void task_destructor(int n, struct Task task[n])
{
    for(int i = 0; i < n; i++) { 
        free(task[i].predecessor); 
    }
    free(task);
}

void show_task(int n, struct Task task[n])
{
    for(int i = 0; i < n; i++) {
        printf("id:%3d, time:%3d: pred:%3d after:%3d", 
                task[i].id, task[i].processing_time, task[i].predecessor_size, task[i].successors);
        printf("\t");
        for(int j = 0; j < task[i].predecessor_size; j++) { printf("%d ", task[i].predecessor[j]->id);
        }
        printf("\n");
    }

}

void export_task_graph(int n, struct Task task[n], char *export_file_name)
{
    FILE *fp = fopen(export_file_name, "w");
    fprintf(fp, "digraph {\n");
    for(int i = 0; i < n; i++) {
        fprintf(fp, "%d [label = \"%d,%d\", shape = circle, overlap = false]\n", task[i].id, task[i].id, task[i].processing_time);
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < task[i].predecessor_size; j++) {
            fprintf(fp, "%d -> %d\n", task[i].predecessor[j]->id, task[i].id);
        }
    }
    fprintf(fp, "}\n");

    fclose(fp);
}

// 副作用あります
int pop_strtoi(char **str)
{
    int num = 0;
    char *endptr;

    num = (int)strtol(*str, &endptr, 10);
    *str = endptr;

    return num;
}

void set_critical_path(int n, struct Task task[n])
{
    int candidate_path_len = 0;
    for(int i = n - 1; i > 0; i--) {
        for(int j = 0; j < task[i].predecessor_size; j++) {
            task[i].predecessor[j]->successors++;
            candidate_path_len = task[i].predecessor[j]->processing_time + task[i].cp_len;
            if( task[i].predecessor[j]->cp_len <= candidate_path_len) {
                task[i].predecessor[j]->cp_len  = candidate_path_len;
            }
        }
    }
}

