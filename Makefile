CFLAGS = -Wall -O3

build: main.out main_task.out main_plist.out

main.out: main.c task_scheduling.o task.o priority_list.o function-timer.h
	gcc main.c task_scheduling.o task.o priority_list.o -o main.out -Wall -O3

main_task.out: main_task.c task_scheduling.o task.o priority_list.o function-timer.h
	gcc main_task.c task_scheduling.o task.o priority_list.o -o main_task.out -Wall -O3

main_plist.out: main_plist.c task_scheduling.o task.o priority_list.o function-timer.h
	gcc main_plist.c task_scheduling.o task.o priority_list.o -o main_plist.out -Wall -O3

task_scheduling.o: task_scheduling.c function-timer.h
	gcc -c task_scheduling.c $(CFLAGS)

task.o: task.c function-timer.h
	gcc -c task.c $(CFLAGS)

priority_list.o: priority_list.c function-timer.h
	gcc -c priority_list.c $(CFLAG)

clean:; rm -rf *.o
