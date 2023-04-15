#ifndef _time_sys_h
#define _time_sys_h

#include <time.h>
#include <stdbool.h>

typedef enum{
	TS_BG,
	TS_ED,
	TS_AL,
} TS_OPERATION;

typedef struct{
	/*��Ϸ��ʼʱ��*/
	time_t Start_t;
	struct tm *Start_p;
	/*��Ϸ����ʱ��*/
	time_t End_t;
	struct tm *End_p;
	/*������Ϸʱ��*/
	time_t Total_t;
	struct tm *Total_p;
	/*��Ϸ����ʱ*/
	bool countdown_f;
	int countdown_n;
	char countdown_s[20]; 
	/*����*/
	void (*UpdtStr)(); 
	void (*GetTime)(TS_OPERATION op); 
	
} TIMESYS,*LPTIMESYS;

void InitTimeSys(LPTIMESYS* TimeSys);

void ReStartCountDown();
void StartCountDown();
void EndCountDown();

#endif
