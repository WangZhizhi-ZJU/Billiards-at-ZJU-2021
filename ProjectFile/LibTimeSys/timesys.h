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
	/*游戏开始时间*/
	time_t Start_t;
	struct tm *Start_p;
	/*游戏结束时间*/
	time_t End_t;
	struct tm *End_p;
	/*已玩游戏时长*/
	time_t Total_t;
	struct tm *Total_p;
	/*游戏倒计时*/
	bool countdown_f;
	int countdown_n;
	char countdown_s[20]; 
	/*方法*/
	void (*UpdtStr)(); 
	void (*GetTime)(TS_OPERATION op); 
	
} TIMESYS,*LPTIMESYS;

void InitTimeSys(LPTIMESYS* TimeSys);

void ReStartCountDown();
void StartCountDown();
void EndCountDown();

#endif
