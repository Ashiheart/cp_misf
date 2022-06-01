CFLAGS = -Wall -O3

main.out: main.c task_scheduling.o task.o priority_list.o function-timer.h
	gcc main.c task_scheduling.o task.o priority_list.o -o main.out -Wall -O3

task_scheduling.o: task_scheduling.c
	gcc -c task_scheduling.c $(CFLAGS)

task.o: task.c
	gcc -c task.c $(CFLAGS)

priority_list.o: priority_list.c
	gcc -c priority_list.c $(CFLAG)

clean:; rm -rf *.o
