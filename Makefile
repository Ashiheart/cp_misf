CFLAGS = -Wall -O3

main.out: main.c task_scheduling.o task.o priority_list.o
	gcc main.c task_scheduling.o task.o priority_list.o -o main.out -Wall -O3

task_scheduling.o: task_scheduling.c
	gcc -c task_scheduling.c -Wall $(CFLAGS)

task.o: task.c
	gcc -c task.c -Wall $(CFLAGS)

priority_list.o: priority_list.c task.o
	gcc -c priority_list.c task.o $(CFLAG)

clean:; rm -rf *.o
