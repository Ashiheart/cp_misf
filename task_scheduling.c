
static void update_processing_element(struct Task *assign, struct priority_list *head);

static int strtod_next(char **str);

static int is_task_standby(const struct Task *task);

static int cmp(const void *lhs, const void *rhs);


void read_task(int n, struct Task task[n], struct priority_list plist[n])
{
    char str[12 * (n + 2)];

    char *ptr = NULL;

    int predecessor_id;

    for(int i = 0; i < n; i++) {

        fgets(str, sizeof(str), stdin);

        ptr = str;

        task[i].id               = strtod_next(&ptr);
        task[i].processing_time  = strtod_next(&ptr);
        task[i].predecessor_size = strtod_next(&ptr);

        task[i].predecessor = (struct Task**)malloc(sizeof(struct Task*)*task[i].predecessor_size);

        for(int j = 0; j < task[i].predecessor_size; j++) {
            predecessor_id                   = strtod_next(&ptr);
            task[i].predecessor[j]           = &task[predecessor_id];
            task[predecessor_id].successors += 1;
        }
    }

}

void export_task_graph(int n, struct Task task[n])
{
    FILE *fp = fopen("graph.gv", "w");
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
    for(int i = n - 1; i >= 0; i--) {
        for(int j = 0; j < task[i].predecessor_size; j++) {
            candidate_path_len = task[i].predecessor[j]->processing_time + task[i].cp_len;
            if( task[i].predecessor[j]->cp_len <= candidate_path_len) {
                task[i].predecessor[j]->cp_len  = candidate_path_len;
            }
        }
    }
}

void show_scheduling_processor(struct priority_list *head, int pe_num)
{
    printf("time | PE0\tPE1\n");
    printf("-----------------------\n");

    struct Task *assign[pe_num];

    for(int t = 0; t < 10; t++) {
        for(int i = 0; i < pe_num; i++) {
            update_processing_element(assign[i], head);
        }
    }
}

int is_task_standby(const struct Task *task)
{
    for(int i = 0; i < task->predecessor_size; i++) {
        if(task->predecessor[i]->state != Complete) return 0;
    }

    return 1;
}

void update_processing_element(struct Task *assign, struct priority_list *head)
{
    if(assign == NULL || assign->state == Complete) {
        for(struct priority_list *ptr = head; ptr != NULL; ptr = ptr->next) {
            if(is_task_standby(ptr->value)) {
                assign = ptr->next->value;
            }
        }
    } else {
        assign->processing_time--;
        if(assign->processing_time <= 0) {
            assign->state = Complete;
        }
    }
}

int cmp(const void *lhs, const void *rhs)
{
    int val = ((struct Task*)rhs)->cp_len - ((struct Task*)lhs)->cp_len;

    if(val != 0) {
        return val;
    } else {
        return ((struct Task*)rhs)->successors - ((struct Task*)lhs)->successors;
    }
}

void make_priority_list(int n, struct priority_list plist[n], struct priority_list *head)
{
    struct priority_list *tmp = NULL;

    for(int i = n - 1; i >= 0; i--) {
        for(struct priority_list *ptr = head; ptr != NULL; ptr = ptr->next) {
            if(ptr->next == NULL || cmp(ptr->next->value, plist[i].value) >= 0) {
                tmp = ptr->next;
                ptr->next = &plist[i];
                plist[i].next = tmp;
                break;
            }
        }
    }

}

int strtod_next(char **str)
{
    int num = 0;
    char *endptr;

    num = (int)strtol(*str, &endptr, 10);
    *str = endptr;

    return num;
}

void show_task(int n, struct Task task[n])
{
    for(int i = 0; i < n; i++) {
        printf("id:%3d, time:%3d: pred:%3d\n", task[i].id, task[i].processing_time, task[i].predecessor_size);
        printf("\t");
        for(int j = 0; j < task[i].predecessor_size; j++) {
            printf("%d ", task[i].predecessor[j]->id);
        }
        printf("\n");
    }

}

void show_plist(struct priority_list *head)
{
    for(struct priority_list *ptr = head->next; ptr != NULL; ptr = ptr->next) {
        printf("id:%3d, cp_len:%3d, after task:%3d\n", ptr->value->id, ptr->value->cp_len, ptr->value->successors);
    }
}
