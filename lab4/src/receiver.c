#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include "common.h"
//#include "point.h"
#include <unistd.h>
#include <sys/time.h>

#define _XOPEN_SOURCE 600

int main(int argc, char *argv[])
{
	mqd_t qdes;
	char qname[] = "/mailbox1_srajguru";
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;
	
	//convert character arguments N,B to integers
	int N = atoi(argv[1]);
	int B = atoi(argv[2]);

	if ( argc !=3 ) {
		printf("Usage: produce <N> <B>\n");
		exit(1);
	}

	//qname = argv[1];

	attr.mq_maxmsg  = B;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;	/* a blocking queue  */

	qdes  = mq_open(qname, O_RDONLY, mode, &attr);
	if (qdes == -1 ) {
		perror("mq_open()");
		exit(1);
	}


	int count = 0;
	while (count<N) {
		int number;
		/* only block for a limited time if the queue is empty */
		if ((mq_receive(qdes, (char *) &number, sizeof(int), 0)) == -1)
 {
			perror("mq_receive() failed");
			
		} else {
			printf("%d is consumed\n", number);
           		count++;
		}
	};

	if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}

	return 0;
}

