CC = gcc
OBJS1 = event_manager.o date.o event_manager_tests.o
OBJS2 = priority_queue.o pq_example_tests.o
EXEC1 = event_manager
EXEC2 = priority_queue
DEBUG_FLAG = -g -DNDEBUG
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror
LIB_PQ = -L. -lpriority_queue


$(EXEC1): $(OBJS1) 
	$(CC) $(DEBUG_FLAG) $(COMP_FLAG) $(OBJS1) -o $@ $(LIB_PQ)

$(EXEC2): $(OBJS2) 
	$(CC) $(DEBUG_FLAG) $(COMP_FLAG) $(OBJS2) -o $@

pq_example_tests.o: tests/pq_example_tests.c priority_queue.h tests/test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $/tests/pq_example_tests.c

event_manager_tests.o: tests/event_manager_tests.c event_manager.h tests/test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $/tests/event_manager_tests.c
	
event_manager.o: event_manager.c event_manager.h priority_queue.h date.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
	
priority_queue.o: priority_queue.c priority_queue.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

date.o: date.c date.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c


clean:
	rm -f $(OBJS1) $(EXEC1)
	rm -f $(OBJS2) $(EXEC2)