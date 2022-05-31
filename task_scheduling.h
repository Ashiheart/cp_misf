#pragma once 

struct timeval start, end;

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
void read_task(FILE *fp, int *n, struct Task *task);

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


