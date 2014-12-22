We have decided to declare a box with one block of memory in size 12 bytes. A brief description of each task has been provided in lab3_test_spec.txt.

This is how we have ensured that the five test cases have been met:-

1. A task periodically prints task status of each task in the system.

Task 5 prints the task status of each status in the system at a period of os_dly_wait(1).

2. A task can allocate a fixed size of memory.

Task 3, Task3h, and Task3hh all (try to) allocate a block of memory in the testpool.

3. A task will get blocked if there is no memory available when os_mem_alloc() is called.

On running the program, after Task 3 is first run, you will notice that when Task 3h and Task 3hh are run, their statuses are shown as WAIT_MEM and are blocked.

4. A blocked on memory task will be resumed once enough memory is available in the system.
AND
5. Create a testing scenario that multiple tasks with different priorities are all blocked waiting for memory. When memory becomes available, test whether it is the highest priority task that waits the longest that gets the memory first.

Subsequently when Task 4 is run and the block of memory is freed up, notice that of the three tasks Task3 (Priority: 1), Task3h (Priority: 2), Task 3hh (Priority: 3), it is the task with the highest priority, Task 3hh that is unblocked and gets the memory, as can be observed with its WAIT_DLY task state.