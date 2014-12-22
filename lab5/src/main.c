#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

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


        int N = atoi(argv[1]);
        int B = atoi(argv[2]);
        int P = atoi(argv[3]);
        int C = atoi(argv[4]);
        int M = N;

        //Array and counter to store all the process IDs
	   int pids[P+C];
	   int pidcount = 0;

       //Main message queue
        mqd_t qdes;

        char  qname[] = "/mailbox1_srajguru";

        mode_t mode = S_IRUSR | S_IWUSR;
        struct mq_attr attr;

        attr.mq_maxmsg  = B;
        attr.mq_msgsize = sizeof(int);
        attr.mq_flags   = 0;            /* a blocking queue  */

        qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
        if (qdes == -1 ) {
                perror("mq_open() failed");
                exit(1);
        }

//Producer counter, single message queue
        mqd_t prodmq;

        char  prodqname[] = "/prodmq";

        mode_t modepmq = S_IRUSR | S_IWUSR;
        struct mq_attr pmqattr;

        pmqattr.mq_maxmsg  = 1;
        pmqattr.mq_msgsize = sizeof(int);
        pmqattr.mq_flags   = 0;            /* a blocking queue  */

        prodmq  = mq_open(prodqname, O_RDWR | O_CREAT, modepmq, &pmqattr);
        if (prodmq == -1 ) {
                perror("mq_open() failed here");
                exit(1);
        }


        if (mq_send(prodmq, (char *)&N, sizeof(int), 0) == -1)
        	{
                        perror("mq_send() failed");
            }

//Consumer counter, single message queue
        mqd_t consmq;

        char  consqname[] = "/consmq";

        mode_t modecmq = S_IRUSR | S_IWUSR;
        struct mq_attr cmqattr;

        cmqattr.mq_maxmsg  = 1;
        cmqattr.mq_msgsize = sizeof(int);
        cmqattr.mq_flags   = 0;            /* a blocking queue  */

        consmq  = mq_open(consqname, O_RDWR | O_CREAT, modecmq, &cmqattr);
        if (consmq == -1 ) {
                perror("mq_open() failed");
                exit(1);
        }


        if (mq_send(consmq, (char *)&M, sizeof(int), 0) == -1)
        	{
                        perror("mq_send() failed");
            }

//Fork producer processes

        int i;
	
        gettimeofday(&tv, NULL);
	    t1 = tv.tv_sec + tv.tv_usec/1000000.0;
       
        for(i=0;i<P;i++)
        {
		//Store process ID
        pids[pidcount] = i;
		pidcount++;

        //Pass process ID as an argument to producer.c, cast it to a char pointer
		int a = i+48;
		char* t = (char*)&a;

		char* arg_list[] = {"./producer.out", argv[1], argv[2], argv[3], argv[4], t, NULL};
                spawn("./producer.out", arg_list);
        }

//Fork consumer processes
        int j;
        for(j=0;j<C;j++)
        {
		pids[pidcount] = j;
		pidcount++;
		int b = j+48;
		char* u = (char*)&b;
		char* arg_list2[] = {"./consumer.out", argv[1], argv[2], argv[3], argv[4], u, NULL};
 	      	spawn("./consumer.out", arg_list2);	
        }

	gettimeofday(&tv, NULL);
        t2 = tv.tv_sec + tv.tv_usec/1000000.0;

        if (mq_close(qdes) == -1) {
                perror("mq_close() failed");
                exit(2);
        }

        if (mq_close(prodmq) == -1) {
                perror("mq_close() failed");
                exit(2);
        }

        if (mq_close(consmq) == -1) {
                perror("mq_close() failed");
                exit(2);
        }

    //Wait for all processes
	int k;
	for (k=0;k<pidcount;k++)
	{
		wait (pids);
	}
	
    //Unlink message queues
	if(WIFEXITED (*pids))
	{
		if (mq_unlink(qname) != 0) {
		perror("mq_unlink() failed");
		exit(3);

		}
}


if (mq_unlink(prodqname) != 0) {
		perror("mq_unlink() failed");
		exit(3);
}


if (mq_unlink(consqname) != 0) {
			perror("mq_unlink() failed");
			exit(3);
}

//}

printf("System execution time: %f seconds\n", (t2-t1));

return 0;
}
