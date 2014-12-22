#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h> 
#include <string.h> 
#include<semaphore.h>
#define QUEUE_SIZE  6
#define _XOPEN_SOURCE 600

//typedef struct producePara;
//typedef struct consumePara;
void* produce(void *ptr);
void* consume(void *ptr);
//use unnamed semaphore
sem_t ConsumeSem;
sem_t ProduceSem;
sem_t lock;
int *Buffer = NULL;
int count = 0; 
pthread_mutex_t lock_mutex = PTHREAD_MUTEX_INITIALIZER;
int P;
int C;
int N;
int B;
int countC =0; 

double getTime(){
	
	struct timeval tv;
	double t1;

	gettimeofday(&tv, NULL);
	t1 = tv.tv_sec + tv.tv_usec/1000000.0;
	return t1; 
	

}


struct producePara{
	int PID;
	int MaxNum;
	int NumOfP;
	int Buffersize;
	int NumOfC;

}producePara;

struct consumePara{
	int CID;
	int size;
	int Buffersize;

}consumePara;



// we need to implement both thread and process and compare the two 
//when a Process run due to system scheduling, it determines whether it's C or P and perform send/receive. 
//also need a function to see if the squareroot of itself is a int 
//need to manually set up buffer for the thread method 

//for process method we fork as many as we want, and then assign each child process to either C or P 
//according to the assignment when the process starts we know it's either C or  P and then we produce/consume 






int is_square(int Num){

	double a;
	if(Num == 0){
	return 0;}
	double c = (double) Num * 1.00;
	a = sqrt(c);
	while(a>1){
	a--;
	}
	if(a == 1){
	
	return 0;
	}
	else{return 1;}

}
void* produce(void* ptr){//MaxNum = N-1; 
//int PID, int *buffer, int MaxNum, int NumOfP
	////printf("in produce\n");
	struct producePara *data;            
    data = (struct producePara*) ptr;
	//////printf("Check buffer\n");	
	int i; 
	int n = data->PID;
	int semValue;
	while(n < data->MaxNum){
	//	printf("producer%d in while\n",data->PID);
		//printf("n = %d, data->MaxNum = %d\n", n, data->MaxNum );
		
		sem_getvalue(&ProduceSem, &semValue);
	//	printf("producer%d has the produceSem value of %d \n",data->PID, semValue);
		sem_wait(&ProduceSem);  
	//	printf("producer%d passed semaphore\n",data->PID);
		
		pthread_mutex_lock (&lock_mutex);
	//	printf("producer locked\n");	
		
		for(i = 0; i <data->Buffersize; i++ ){
	//	printf("in for, buffer[%d] = %d\n", i, Buffer[i]); 
			if(Buffer[i] == -1){
				
				//printf("producer%d locked mutex\n",data->PID);
				Buffer[i] = n; 
	//			printf("Buffer[%d] = %d Produced\n", i, Buffer[i] );	
			
				break; 	
			}
		}
		pthread_mutex_unlock (&lock_mutex);
//		printf("producer unlocked\n");
		
		
		sem_post(&ConsumeSem);
		//printf("producer%d unlocked sema\n",data->PID);
		n = n + data->NumOfP; 
	//	printf("n = %d\n",n);		
	}

	//printf("done produce\n");
	return NULL;
	/**/
} 
void* consume(void* ptr){
	//int CID, int *buffer, int size

	struct consumePara *data;            
    data = (struct consumePara *) ptr;
	//printf("consumer %d Entered\n",data->CID);
	countC++; 
	int semValue;
	////printf("countC =  %d whiled\n",countC);
	while(count < data->size){
		//printf("consumer %d whiled\n",data->CID);
		
		
			//printf("In consumer loop\n");
			int i;
			int flag = 0;
			sem_getvalue(&ConsumeSem, &semValue);
	//		printf("consumer%d has the ConsumeSem value of %d \n",data->CID, semValue);
			sem_wait(&ConsumeSem);
			//printf("consumer%d passed Semaphore\n",data->CID);
		/**/for(i = 0; i <data->Buffersize; i++ ){
			pthread_mutex_lock (&lock_mutex);
//			printf("consumer %d locked\n",data->CID);
				if(Buffer[i] != -1){
				
					
					
					//printf("buffer[%d] = %d\n",i, Buffer[i]);
					
					if(is_square(Buffer[i]) == 0){
						
						printf("%d, %d, %d\n", data->CID, Buffer[i], (int)sqrt(Buffer[i])); 
					}
	
	//				printf(" Buffer[%d] = %d is consumed ,\n", i, Buffer[i]);
					Buffer[i] = -1;
					count++;
					sem_post(&ProduceSem);
					
				
				}
				pthread_mutex_unlock (&lock_mutex);	
//				printf("consumer%d unlocked\n",data->CID);
					
					
			}
			
	//		printf("count = %d\n",count);
		
		
	}
		if(count >= data->size-1)
			{
				//printf("semaphore added\n");
				sem_post(&ConsumeSem); 
			}	
		

return NULL;
} 
int main (int argc, char *argv[])
{
	//////printf("1\n");
	
	double TimeStamp1;
	double TimeStamp2;	
	TimeStamp1 = getTime();
	char *Bn; 
	B = strtol(argv[2], &Bn, 10);
	char *Nn;
	N = strtol(argv[1], &Nn, 10);
	char *Pn;
	P = strtol(argv[3], &Pn, 10);
	char *Cn;
	C = strtol(argv[4], &Cn, 10);
	sem_init(&ProduceSem, 0, B);
	sem_init(&ConsumeSem, 0, 0);
	//////printf(" N = %d\n", N);
	//////printf("B = %d\n", B);
	//////printf("P = %d\n", P);
	//////printf("C = %d\n", C);
	pthread_t threadid[P+C];
	Buffer = malloc(sizeof(int)*B);
	int i;
	for(i = 0; i < B; i ++){
		Buffer[i] = -1; 
	}
	
	int a; 
	struct producePara data[P]; 
	
	for(a = 0; a < P; a++){
		//////printf("a\n");
		data[a].PID = a; 
		data[a].Buffersize = B; 
		data[a].MaxNum = N; 
		data[a].NumOfP = P; 
		data[a].NumOfC = C;
		//pthread_t thread; 
		int threadNum;
		threadNum = pthread_create(&threadid[a], NULL, &produce,(void *) &data[a]);
		//////printf("Thread = %d\n",threadNum );
		
	}
	struct consumePara data1[C];
	int b;
	for(b = 0; b < C; b++){
		
		data1[b].CID =b;
		data1[b].size = N; 
		data1[b].Buffersize = B; 
		//pthread_t thread; 
	//	////printf("data->CID = %d\n",data1.CID );
		int threadNum;
		threadNum = pthread_create(&threadid[b+P], NULL,  &consume,(void *) &data1[b]);
		//threadid[b+P] = thread;
		
	}
	
	/*while(1){}*/
	//printf("quitting" ); 
	int c;
	for(c =0 ; c <C+P; c++ ){
		pthread_join (threadid[c], NULL);
	}
/* Make sure the second thread has finished. */
	TimeStamp2 = getTime();
	printf ("System Excution Time:   %.6lf  seconds\n", TimeStamp2 -TimeStamp1);
	
	return 0; 
}

