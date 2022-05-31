#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>

#define MEASURE_MODE

#ifdef MEASURE_MODE
#define timer_print(str, start, end) \
    fprintf(stderr, "%d, %s: %s:\texe time %f[ms]\n", \
            __LINE__, __func__, str, \
            ((end.tv_sec - start.tv_sec) * 1000. + (end.tv_usec - start.tv_usec) * 0.001))

#define function_timer(method, message) { \
    struct timeval start, end; \
    gettimeofday(&start, NULL); \
    method; \
    gettimeofday(&end, NULL); \
    timer_print(message, start, end); }
#else
#define function_timer(method, message) method
#endif

struct Task
{
    int     id;
    int     processing_time;

    int     cp_len;
    int     successors;

    int     progress;

    int             predecessor_size;
    struct Task**   predecessor;
};

struct priority_list
{
    struct Task*            value;

    struct priority_list*   next;
};

void cp_misf_prioritylist(FILE *fp, int pe_num);

/* .stgファイルをtaskに読み込み */
void make_task(FILE *fp, int *n, struct Task **task);

void end_task(int n, struct Task task[n]);

void make_priority_list(int n, struct Task task[n], struct priority_list *head);

void simulate_scheduling_processor(struct priority_list *head, int pe_num);

void simulate_scheduling_processor_array(int n, struct Task task[n], int pe_num);

/* タスクグラフを画像に変換 */
void export_task_graph(int n, struct Task task[n], char *export_file_name);

void show_task(int n, struct Task task[n]);

void show_plist(struct priority_list *head);

void show_critical_path(struct priority_list *head);



void set_critical_path(int n, struct Task task[n]);

static int pop_strtoi(char **str);

static bool is_scheduling_complete(int pe_num, struct Task *assign[pe_num], struct priority_list *head);

static struct Task *assign_task(struct priority_list *head);

static bool is_task_standby(const struct Task *task);

static int cmp(const void *lhs, const void *rhs);

static void show_critical_path_recursive(struct priority_list *tar);

static void show_processor_state(int time, int pe_num, struct Task *assign[pe_num]);

void task_destructor(int n, struct Task task[n]);

void plist_destructor(struct priority_list *head);


int main(int argc, char *argv[])
{
    int pe = argc == 2 ? (int)strtol(argv[1], NULL, 10) : 2;

    cp_misf_prioritylist(stdin, pe);

    //function_timer(qsort(task, task_len, sizeof(struct Task), cmp), "quick_sort");

    return 0;
}

void cp_misf_prioritylist(FILE *fp, int pe_num)
{
    int task_len = 0;

    struct Task *task;

    function_timer(make_task(fp, &task_len, &task), "input");

    function_timer(set_critical_path(task_len, task), "cpath");


    struct priority_list head = (struct priority_list) { .value = NULL, .next = NULL };

    function_timer(make_priority_list(task_len, task, &head), "insert_sort");

    //show_critical_path(&head);

    //show_plist(&head);

    //show_task(task_len, task);

    function_timer(simulate_scheduling_processor(&head, pe_num), "scheduling");


    task_destructor(task_len, task);

    plist_destructor(&head);
}

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

void plist_destructor(struct priority_list *head)
{
    struct priority_list *next;
    for(struct priority_list *ptr = head->next; ptr != NULL; ptr = next)
    {
        next = ptr->next;
        free(ptr);
    }
}

// ex) dot -Tpng graph.gv -o graph.png
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

int cmp(const void *lhs, const void *rhs)
{
    int val = ((struct Task*)rhs)->cp_len - ((struct Task*)lhs)->cp_len;

    //misf
    if(val != 0) {
        return val;
    } else {
        return ((struct Task*)rhs)->successors - ((struct Task*)lhs)->successors;
    }
}

void make_priority_list(int n, struct Task task[n], struct priority_list *head)
{
    struct priority_list *tmp = NULL;
    struct priority_list *tar = NULL;

    // i = 0, (n - 1) is dummy tasks.
    for(int i = n - 2; i > 0; i--) {

        tar  = (struct priority_list*)malloc(sizeof(struct priority_list));

        *tar = (struct priority_list) { .value = &task[i], .next = NULL };

        for(struct priority_list *ptr = head; ptr != NULL; ptr = ptr->next) {
            if(ptr->next == NULL || cmp(ptr->next->value, tar->value) >= 0) {
                tmp = ptr->next;
                ptr->next = tar;
                tar->next = tmp;
                break;
            }
        }
    }

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

void show_plist(struct priority_list *head)
{
    for(struct priority_list *ptr = head->next; ptr != NULL; ptr = ptr->next) {
        printf("id:%3d, pt:%3d, cp_len:%3d, after task:%3d\n", 
                ptr->value->id, ptr->value->processing_time, ptr->value->cp_len, ptr->value->successors);
    }
}

void show_critical_path(struct priority_list *head)
{
    puts("id(critical_path, processing_time)");
    show_critical_path_recursive(head->next);
    puts("");
}

void show_critical_path_recursive(struct priority_list *tar)
{
    if(tar == NULL) return;

    printf("%d(c%d,p%d)", tar->value->id, tar->value->cp_len, tar->value->processing_time);

    if(tar->value->cp_len - tar->value->processing_time == 0) {
        return;
    }

    printf(" -> ");

    for(struct priority_list *ptr = tar; ptr != NULL; ptr = ptr->next) {
        if(ptr->value->cp_len == tar->value->cp_len - tar->value->processing_time) {
            show_critical_path_recursive(ptr);
            return ;
        }
    }
}
