#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

int main(int argc, char *argv[])
{

//      int N = atoi(argv[1]);
        int B = atoi(argv[2]);
        int P = atoi(argv[3]);
//      int C = atoi(argv[4]);

        //Process ID, passed from main
        int id = atoi(argv[5]);

        //Main mq
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

        //Prodcount mq - communicates a counter to make sure only N numbers are produced
        mqd_t prodmq;

        char  prodqname[] = "/prodmq";

        mode_t modepmq = S_IRUSR | S_IWUSR;
        struct mq_attr pmqattr;

        pmqattr.mq_maxmsg  = 1;
        pmqattr.mq_msgsize = sizeof(int);
        pmqattr.mq_flags   = 0;            /* a blocking queue  */

        prodmq  = mq_open(prodqname, O_RDWR | O_CREAT, modepmq, &pmqattr);
        if (prodmq == -1 ) {
                perror("mq_open() failed");
                exit(1);
        }

 
int prodcount;
int count=0;
       
	while(1)
{
        mq_receive(prodmq, (char *) &prodcount, sizeof(int), 0);
        
        //Ensure that only N items are produced
        if(prodcount!=0)
        {
                //Decrement the producer counter and send it back
        	prodcount--;
        	mq_send(prodmq, (char *)&prodcount, sizeof(int), 0);
        
        
                int number;
                number = id + (count*P);

        //Send number produced to the message queue
        if (mq_send(qdes, (char *)&number, sizeof(int), 0) == -1) {
                        perror("mq_send() failed");
                }

	count++;
        }

//Last item to be produced
else
{
 mq_send(prodmq, (char *)&prodcount, sizeof(int), 0);
break;
}
}

if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}

if (mq_close(prodmq) == -1) {
		perror("mq_close() failed");
		exit(2);
	}
	return 0;
}

