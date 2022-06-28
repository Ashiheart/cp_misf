CFLAGS = -Wall -O3
.PHONY: build clean

build: array.out plist.out

array.out: array.c task_scheduling.o task.o priority_list.o function-timer.h
	gcc array.c task_scheduling.o task.o priority_list.o -o array.out $(CFLAGS)

plist.out: plist.c task_scheduling.o task.o priority_list.o function-timer.h
	gcc plist.c task_scheduling.o task.o priority_list.o -o plist.out $(CFLAGS) 

test.out: test.c task_scheduling.o task.o priority_list.o
	gcc test.c task_scheduling.o task.o priority_list.o -o test.out $(CFLAGS) 

task_scheduling.o: task_scheduling.c function-timer.h
	gcc -c task_scheduling.c $(CFLAGS)

task.o: task.c function-timer.h
	gcc -c task.c $(CFLAGS)

priority_list.o: priority_list.c function-timer.h
	gcc -c priority_list.c $(CFLAGS)

clean:; rm -rf *.o *.out dump*
