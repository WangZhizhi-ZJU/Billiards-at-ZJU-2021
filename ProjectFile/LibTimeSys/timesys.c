#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <timesys.h>
#include <callback.h>

static LPTIMESYS ActiveTimeSys = NULL;
static void GetTime(TS_OPERATION op){
	switch(op){
		case TS_BG:
			time(&ActiveTimeSys->Start_t);
			ActiveTimeSys->Start_p = gmtime(&ActiveTimeSys->Start_t);
			printf("TIME: %04d/%02d/%02d %02d:%02d:%02d\n", 1900+ActiveTimeSys->Start_p->tm_year,
															1+ActiveTimeSys->Start_p->tm_mon,
															ActiveTimeSys->Start_p->tm_mday,
															8+ActiveTimeSys->Start_p->tm_hour,
															ActiveTimeSys->Start_p->tm_min,
															ActiveTimeSys->Start_p->tm_sec);
		break;
		case TS_ED:
			time(&ActiveTimeSys->End_t);
			ActiveTimeSys->End_p = gmtime(&ActiveTimeSys->End_t);
			printf("TIME: %04d/%02d/%02d %02d:%02d:%02d\n", 1900+ActiveTimeSys->End_p->tm_year,
															1+ActiveTimeSys->End_p->tm_mon,
															ActiveTimeSys->End_p->tm_mday,
															8+ActiveTimeSys->End_p->tm_hour,
															ActiveTimeSys->End_p->tm_min,
															ActiveTimeSys->End_p->tm_sec);

		break;
		case TS_AL:
			ActiveTimeSys->Total_t = ActiveTimeSys->End_t - ActiveTimeSys->Start_t;
			ActiveTimeSys->Total_p->tm_sec = ActiveTimeSys->Total_t % 60;
			ActiveTimeSys->Total_p->tm_min = (ActiveTimeSys->Total_t - ActiveTimeSys->Total_p->tm_sec) / 60 % 60;
			ActiveTimeSys->Total_p->tm_hour = (ActiveTimeSys->Total_t - ActiveTimeSys->Total_p->tm_sec - ActiveTimeSys->Total_p->tm_min * 60) / 3600;
			printf("TIME: %02d:%02d:%02d\n",	ActiveTimeSys->Total_p->tm_hour,
												ActiveTimeSys->Total_p->tm_min,
												ActiveTimeSys->Total_p->tm_sec);
			break;
	}
	return;
}

static void UpdtStr(){
	sprintf(ActiveTimeSys->countdown_s,"　　　 00:%02d",ActiveTimeSys->countdown_n);
	return;
}

void InitTimeSys(LPTIMESYS* TimeSys){
	if(ActiveTimeSys) return;
	(*TimeSys) = (LPTIMESYS)malloc(sizeof(TIMESYS));
	ActiveTimeSys = (*TimeSys);
	(*TimeSys)->Total_p = (struct tm*)malloc(sizeof(struct tm));
	(*TimeSys)->countdown_n = 0;
	(*TimeSys)->countdown_f = 0;
	(*TimeSys)->GetTime = GetTime;
	(*TimeSys)->UpdtStr = UpdtStr;
	return;
}

/*游戏时间系统*/
extern LPTIMESYS GameTimeSys; 

extern int TimeSysID;
extern HWND TimeSysWindow;

void StartCountDown(){
	/*开始倒计时*/
	if(!GameTimeSys->countdown_f){
		GameTimeSys->countdown_f=true;
		SetTimer(TimeSysWindow, TM_HIT_COUNTDOWN, 1000, NULL); 
	}
	if(GameTimeSys->countdown_n==0)
		GameTimeSys->countdown_n=25;
	GameTimeSys->UpdtStr();
}

void ReStartCountDown(){
	SetTimer(TimeSysWindow, TM_HIT_COUNTDOWN, 1000, NULL); 
}

void EndCountDown(){
	/*结束倒计时*/
	KillTimer(TimeSysWindow,TM_HIT_COUNTDOWN);
	GameTimeSys->countdown_f=false;
	GameTimeSys->countdown_n=0;
	GameTimeSys->UpdtStr();
}
