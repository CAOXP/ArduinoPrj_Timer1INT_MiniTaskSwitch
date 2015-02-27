#include <TimerOne.h>


/****小小调度器开始**********************************************/
#define MAXTASKS 3
volatile unsigned char timers[MAXTASKS];
#define _SS static unsigned char _lc=0; switch(_lc){default: 
#define _EE ;}; _lc=0; return 255;
#define WaitX(tickets)  do {_lc=(__LINE__+((__LINE__%256)==0))%256; return tickets ;} while(0); case (__LINE__+((__LINE__%256)==0))%256: 

#define RunTask(TaskName,TaskID) do { if (timers[TaskID]==0) timers[TaskID]=TaskName(); }  while(0); 
#define RunTaskA(TaskName,TaskID) { if (timers[TaskID]==0) {timers[TaskID]=TaskName(); continue;} }   //前面的任务优先保证执行

#define CallSub(SubTaskName) do {unsigned char currdt; _lc=(__LINE__+((__LINE__%256)==0))%256; return 0; case (__LINE__+((__LINE__%256)==0))%256:  currdt=SubTaskName(); if(currdt!=255) return currdt;} while(0);
#define InitTasks() {unsigned char i; for(i=MAXTASKS;i>0 ;i--) timers[i-1]=0; }
#define UpdateTimers() {unsigned char i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=255)) timers[i-1]--;}}

#define SEM unsigned int 

//初始化信号量
#define InitSem(sem) sem=0;

//等待信号量
#define WaitSem(sem) do{ sem=1; WaitX(0); if (sem>0) return 1;} while(0);

//等待信号量或定时器溢出， 定时器tickets 最大为0xFFFE
#define WaitSemX(sem,tickets)  do { sem=tickets+1; WaitX(0); if(sem>1){ sem--;  return 1;} } while(0);

//发送信号量
#define SendSem(sem)  do {sem=0;} while(0);

/*****小小调度器结束*******************************************************/



unsigned long task0tick = 0;
unsigned char  task0()
{
    _SS
    while(1)
    {
        WaitX(100);				//wait Tickets, less than 256
        task0tick++;
        Serial.println(task0tick);
    }
    _EE
}

unsigned char  task1()
{
    _SS
    while(1)
    {
        WaitX(200);
        Serial.print(".");
    }
    _EE
}
unsigned char task2()
{
    _SS
    while(1)
    {
        WaitX(500);
        digitalWrite( 13, digitalRead( 13 ) ^ 1 );
    }
    _EE
}




void Timer1CallFunc()
{
	UpdateTimers();

	//任务0具有精确按时获得执行的权限，要求：task0每次执行消耗时间<0.5个 ticket
    RunTask(task0,0);
}
 

void setup() 
{
	// Initialize the digital pin as an output.
	// Pin 13 has an LED connected on most Arduino boards
	pinMode(13, OUTPUT);

	Serial.begin(115200);


	//initialize the timer1
	Timer1.initialize(1000);	//unit : microSecond us    1000us = 1ms
	Timer1.attachInterrupt( Timer1CallFunc ); // attach the service routine here



	InitTasks(); //初始化任务，实际上是给timers清零


	while(1)
	{
		//RunTask(task0,0);
		RunTaskA(task1,1);//任务1具有比任务2高的运行权限                   
		RunTaskA(task2,2);//任务2具有低的运行权限     
		/*
			以上任务带优先级，1.优先级不得重复。
							  2.数值不得高于任务数量
		*/              
	}
}
 





void loop(){}
/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
    // Toggle LED
    digitalWrite( 13, digitalRead( 13 ) ^ 1 );
}