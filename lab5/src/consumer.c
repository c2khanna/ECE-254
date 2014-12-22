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

int is_square(int Num){

        double a = (double) Num;
        double b = sqrt(a);

        if(floor(b) == b)
        {
                return 1;
        }
        else{
                return 0;
        }

}

int main(int argc, char *argv[])
{
//	int N = atoi(argv[1]);
        int B = atoi(argv[2]);
//      int P = atoi(argv[3]);
//      int C = atoi(argv[4]);
        int id = atoi(argv[5]);

        //Main mq
        mqd_t qdes;

        char  qname[] = "/mailbox1_srajguru";

        mode_t mode = S_IRUSR | S_IWUSR;
        struct mq_attr attr;

        attr.mq_maxmsg  = B;
        attr.mq_msgsize = sizeof(int);
        attr.mq_flags   = 0;

        qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
        if (qdes == -1 ) {
                perror("mq_open() failed");
                exit(1);
        }

        //Cons mq - communicates a counter to limit items consumed
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

        int conscount;

        while(1)
        {
                mq_receive(consmq, (char *) &conscount, sizeof(int), 0);
                if(conscount!=0)
                {
                //Decrement counter if all of the numbers produced still haven't been consumed
                 conscount--;
                 mq_send(consmq, (char *)&conscount, sizeof(int), 0);

                 int number;
		
                 if ((mq_receive(qdes, (char *) &number, sizeof(int), 0)) == -1)
                 {
                   perror("mq_receive() failed");

           } else {

                //Check if the number received meets the print conditions as mentioned in the manual
                if(is_square(number) == 1)
                {
                    int res = sqrt(number);
                    printf("%i %i %i\n", id, number, res);
                }

                }
                }

//Last number to be consumed
else
{
 mq_send(consmq, (char *)&conscount, sizeof(int), 0);
 break;
}
}

if (mq_close(qdes) == -1) {
  perror("mq_close() failed");
  exit(2);
}

if (mq_close(consmq) == -1) {
  perror("mq_close() failed");
  exit(2);
}

return 0;
}
