#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <time.h>


#define STACK_SIZE 4096

struct ThreadInfo {
	ucontext_t context;
	int weight;
	int rem_time;
	int state;  // 0 ready, 1 running, 2 finished
};

struct ThreadInfo threads[5];


int num_threads=1;

ucontext_t sch; // a thread for the schedulers


void fn1(){
	printf("sa\n");
}


void counter(i){
	threads[i].state=0;
	while(threads[i].rem_time>0){
		threads[i].rem_time--;
		printf("       %d\n",threads[i].weight-threads[i].rem_time);
		sleep(1);
	}
	if(threads[i].rem_time==0){
		threads[i].state=2;
		sleep(3);}
	
	
}

void initialize_thread(int i,int n){
	ucontext_t *t=&threads[i].context;
	threads[i].rem_time=n;
	threads[i].weight=n;
	threads[i].state=0;
	getcontext(t);
	t->uc_link=&sch;
	t->uc_stack.ss_sp=malloc(STACK_SIZE);
	t->uc_stack.ss_size=STACK_SIZE;
	makecontext(t,(void*)&counter,1,i);
}

int create_thread(int i,int n){
//give error and return -1 if the thread array is full
	if(num_threads==5){
		printf("No more threads can be created\n");
		return -1; }
		
// use initialize thread function to 		
	initialize_thread(i,n);
	num_threads++;
	return 0;
}

void run_thread(int i){
//swithces to the ith thread in the thread array
	threads[i].state=1;
	printf("running> T%d        ready> ",i);
	for(int j=0;j<5;j++)
		if(threads[j].state==0)
			printf("T%d, ",j);
	printf("     finished> ");
	for(int j=0;j<5;j++)
		if(threads[j].state==2)
			printf("T%d, ",j);
	printf("\n");
	
	getcontext(&sch); 
	swapcontext(&sch,&threads[i].context); 
}

void exit_thread(int i){
	threads[i].rem_time=0;
	threads[i].weight=0;
	printf("Exiting\n");
	free(threads[i].context.uc_stack.ss_sp);
}

void PWF_scheduler(){
	int cnt=0;
	alarm(2);    //run_thread prints out the status f ther threads 
	srand(time(0));  //instead of schedulers
	int prob=rand()%100;
	if(prob<20&&threads[1].state!=2)
		run_thread(1);
	else if(prob>=20&&prob<32&&threads[2].state!=2)
		run_thread(2);
	else if(prob>=32&&prob<73&&threads[3].state!=2)
		run_thread(3);
	else if(threads[4].state!=2)
		run_thread(4);
	
	for(int j=1;j<5;j++)
		if(threads[j].rem_time>0)
			sleep(3);

}

void SRTF_scheduler(){
	alarm(2);
	int min=1,fin=1;
	for(int i=1;i<5;i++)
		if(threads[i].rem_time>0){
			min=i;
			fin=0;
			break;}
			
	for(int i=1;i<5;i++)
		if(threads[i].rem_time<threads[min].rem_time&&
		threads[i].rem_time>0)
		min=i;
		
	if(fin==0)
	run_thread(min);
	

}

int main()
{   
	//thread 0 is the main
	int scheduler_fun=1; //choose the scheduler: 0 PWF, 1 SRTF
	create_thread(1,5);
	create_thread(2,9);
	create_thread(3,3);
	create_thread(4,12);
	alarm(2);
	if(scheduler_fun==0){
		signal(SIGALRM,PWF_scheduler);
		PWF_scheduler();}
	else{
		signal(SIGALRM,SRTF_scheduler);
		SRTF_scheduler();}
	
	while(1);
	
	return 0;
}

