CFLAGS = -Wall -O3

build: main.out array.out plist.out

main.out: main.c task_scheduling.o task.o priority_list.o function-timer.h
	gcc main.c task_scheduling.o task.o priority_list.o -o main.out -Wall -O3

array.out: array.c task_scheduling.o task.o priority_list.o function-timer.h
	gcc array.c task_scheduling.o task.o priority_list.o -o array.out -Wall -O3

plist.out: plist.c task_scheduling.o task.o priority_list.o function-timer.h
	gcc plist.c task_scheduling.o task.o priority_list.o -o plist.out -Wall -O3

task_scheduling.o: task_scheduling.c function-timer.h
	gcc -c task_scheduling.c $(CFLAGS)

task.o: task.c function-timer.h
	gcc -c task.c $(CFLAGS)

priority_list.o: priority_list.c function-timer.h
	gcc -c priority_list.c $(CFLAG)

clean:; rm -rf *.o *.out
