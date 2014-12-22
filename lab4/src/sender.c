#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include "common.h"
//#include "point.h"
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

/* Spawn a child process running a new program.  PROGRAM is the name
   of the program to run; the path will be searched for this program.
   ARG_LIST is a NULL-terminated list of character strings to be
   passed as the program's argument list.  Returns the process id of
   the spawned process.  */
int spawn (char* program, char** arg_list)
{
	pid_t child_pid;

	/* Duplicate this process.  */
	child_pid = fork ();
	if (child_pid != 0)
		/* This is the parent process.  */
		return child_pid;
	else {
		/* Now execute PROGRAM, searching for it in the path.  */
		execvp (program, arg_list);
		/* The execvp function returns only if an error occurs.  */
		fprintf (stderr, "an error occurred in execvp\n");
		abort ();
	}
}

int main(int argc, char *argv[])
{

	struct timeval tv;
	double t1;
	double t2;
	double t3;

	int child_status;	

	//Converting the character arguments for N, B to an integer
	int N = atoi(argv[1]);
	int B = atoi(argv[2]);

	mqd_t qdes;
	//char  quit = '\0';

	/* queue name must start with '/'. man mq_overview */
	char  qname[] = "/mailbox1_srajguru";	
	//char *qname = NULL;

	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;

	if ( argc !=3 ) {
		printf("Usage: produce <N> <B>\n");
		exit(1);
	}

	//qname = argv[1];
	
	//Attributes of the message queue - max size is B, size of message is the size of an integer, etc.
	attr.mq_maxmsg  = B;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;		/* a blocking queue  */

	qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
	if (qdes == -1 ) {
		perror("mq_open() failed");
		exit(1);
	}

	char* arg_list[] = {
		"./receiver.out",     /* argv[0], the name of the child program, ie. the consumer.  */
		argv[1], 
		argv[2],
		NULL      /* The argument list must end with a NULL.  */
	};

	srand(time(0));

	//Time before the fork
	gettimeofday(&tv, NULL);
	t1 = tv.tv_sec + tv.tv_usec/1000000.0;;

	spawn("./receiver.out", arg_list);

	//Time before the first integer is produced
	gettimeofday(&tv, NULL);
	t2 = tv.tv_sec + tv.tv_usec/1000000.0;

	int count = 0;
	while(count<N)
	{	
		int number;
		
		//Generate a random integer between 1-100	
		number = rand() % 100;
		
	if (mq_send(qdes, (char *)&number, sizeof(int), 0) == -1) {
			perror("mq_send() failed");
		}

		count++;
	};

	//Close message queue after all integers have been produced
	if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}


	/* Wait for the child process, ie. consumer to complete. */
	wait (&child_status);

	if (WIFEXITED (child_status))
	{		
		//Time after the last integer is consumed
		gettimeofday(&tv, NULL);
		t3 = tv.tv_sec + tv.tv_usec/1000000.0;

		if (mq_unlink(qname) != 0) {
			perror("mq_unlink() failed");
			exit(3);
		}

	}
	//	else
	//{
	//printf("The child process exited abnormally\n");
	//}

	//	if (mq_close(qdes) == -1) {
	//		perror("mq_close() failed");
	//		exit(2);
	//	}

	printf("Time to initialize system: %f seconds\n", (t2-t1));
	printf("Time to transmit data: %f seconds\n", (t3-t2));

	return 0;
}

