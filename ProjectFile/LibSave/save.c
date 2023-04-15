#include <windows.h>
#include <stdbool.h>
#include <commdlg.h>
#include <stdio.h>
#include <time.h>

#include <gui.h>
#include <user.h>
#include <text.h>
#include <stdio.h>
#include <media.h>
#include <physics.h>
#include <stdbool.h>
#include <pictures.h>
#include <graphics.h>
#include <extgraph.h> 

#include <sets.h>
#include <public.h>
#include <callback.h>

#include <error.h>
#include <save.h>
#include <openfile.h> 

extern MODE GameMode;
extern int Violation;								//犯规判定 
extern int Sides;									//<需存档变量> 对战模式 - 回合数（用于判断属于谁的回合） 
extern float Power;

extern struct Acceleration
{
	float AccelerationMax;
	float Acceleration;
	float SwirlTheta;
	int Reflects;
	int LeftRotation;
	int RightRotation;
	int FrontRotation;
	int BackRotation;
	int RightSwirl;
	int LeftSwirl;
	float SwirlRatio;
	float RotationRatio;
	float MoveRatio;
} Rotation;

extern bool InHoleFinished[16];
extern float Draw_R[16];
extern int WhichHole[16];
extern float FixConst;
extern float CueTheta;

extern struct Billiards
{
    float Position_X;								//<需存档变量> 球坐标水平分量 X 
    float Position_Y;								//<需存档变量> 球坐标垂直分量 Y
    float Velocity_X;								//球速度水平分量 X 
    float Velocity_Y;								//球速度垂直分量 Y 
    int Moving;										//球是否在移动 
    int State;										//球是否进洞 
    int IfScored;									//球是否已经计分 
    int Score;										//球的计分情况
    LPIMAGE Address;						  		//球图像文件的存储路径 
} Ball[22]; 

extern struct Information
{
	int isSuccessfullyBegin;			//是否顺利开局 
	int isSideConfirmed;				//是否已经确定双方球花色 
	int Turns;							//回合数（一次击球即算作一回合） 
	int Reflect;						//一回合内球台边缘反弹次数 
	int Collide;						//一回合内球与球的碰撞数 
	int Scored;							//一回合内是否进球，记录的是第一个进球的标号
	bool DoubleScored;					//备用，若第一方进球存在双花色，给予自行选择花色的权利 
	int Winner;							//获胜玩家 
	int End;							//比赛是否结束
	bool SideClear[2];					//双方是否清空除黑八以外所有球 
	int SideStrip;						//花色球持有方 
	int SideSolid;						//全色球持有方 
	int SolidInhole;					//当前全色球进球球数 
	int StripInhole;					//当前花色球进球球数 
	int Inhole; 						//当前全局进球球数 
	int Ontable;						//当前全局桌上球数 
	bool isCueInhole;					//母球是否进球 
	bool isBlackInhole;					//判断黑八是否仍在桌面
	int BallState[16];					//记录十六个球的进球状态 
	int NewInhole[16];					//记录新的进球
	int TargetBall;						//记录目标球
	int isSet;							//未摆放球是否妥当  
} Pool;

extern struct ClassicInfomation
{
	int Player1Scored;
	int Player2Scored;
	int ScorePlayer1;
	int ScorePlayer2;
	int Player1Violation;
	int Player2Violation;
	int NewInhole[10];
	int IfScored[10];
	bool PlayerScored;
	int End;
	int Winner;
	int AgainScore1;
	int AgainScore2;
	int AgainTimes;
	int Player1LastScored;
	int Player2LastScored;
} Classic;

extern struct TrainningInformation
{
	int Score;
	int NewInhole[16];
	int IfScored[16];
} Trainning;

extern class SnookerInfomation
{
	bool isSuccessfullyBegin;
	bool isRedClear;
	int isRedTurn;
	//int isCollide;
	int TargetBall;
	int Score;
	int NewInhole[22];
	int BallState[22];
	int Player1Score;
	int Player2Score;
	int Player1Scored;
	int Player2Scored;
	int isCueInhole;
	int isColourScored;
	int isRedScored;
	int NowTime;
	int WrongGoal;
	int Skip;
	int End;
	int Winner; 
} Snooker;

extern int HitPosition_X;
extern int HitPosition_Y;
extern int CurrentPosition_X;
extern int CurrentPosition_Y;
extern int CueBallPosition_X;
extern int CueBallPosition_Y;

extern int NUM;
extern int ifLoad;
extern int LoadState;

extern LPIMAGE IM_Pause_BK;

extern int S_Player1_ID;
extern int S_Player2_ID;

extern LPUSER User1, User2;

/*游戏时间系统*/
extern LPTIMESYS GameTimeSys;

void LoadSave(int i,LPSAVE data)
{
	FILE *read;
	read = fopen("Save/data.sav", "rb+");
	if(!read){
		ErrorMsg("存档文件读取失败！");
		return; 
	}
	
	fseek(read,SAVE_SIZE*(i-1),SEEK_SET);
	
	/*存档位置是否占用标识*/
	fread(&data->flag, sizeof(data->flag), 1, read);
	
	/*存档时间*/
	fread(data->Time, sizeof(data->Time), 1, read);
	
	/*时间系统*/
	TIMESYS tmpSys;
	fread(&tmpSys.countdown_n, sizeof(tmpSys.countdown_n), 1, read);
	
	/*游戏模式文本*/
	fread(data->ModeName, sizeof(data->ModeName), 1, read);
	
	/*玩家ID*/
	fread(&data->Uid1, sizeof(data->Uid1), 1, read);
	fread(&data->Uid2, sizeof(data->Uid2), 1, read); 
	
	/*存档预览图片*/
	if(data->IM_Save_Cover != NULL) FreePicture(&data->IM_Save_Cover);
	if(data->IM_Save_Blur != NULL) FreePicture(&data->IM_Save_Blur);
	ReadBMP(&data->IM_Save_Cover,read,0x18744);
	ReadBMP(&data->IM_Save_Blur,read,0x18744);
	
	fclose(read);
}

void ReadSave(int i)
{
	FILE *read;
	read = fopen("Save/data.sav", "rb+");
	if(!read){
		ErrorMsg("存档文件读取失败！");
		return; 
	}
	
	fseek(read,SAVE_SIZE*(i-1),SEEK_SET);
	
	/*存档标志数据结构体*/
	SAVE savedata;
	LPSAVE data=&savedata;
	
	/*存档位置是否占用标识*/
	fread(&data->flag, sizeof(data->flag), 1, read);
	
	/*存档时间*/
	fread(data->Time, sizeof(data->Time), 1, read);
	
	/*时间系统*/
	fread(&GameTimeSys->countdown_n, sizeof(GameTimeSys->countdown_n), 1, read);
	GameTimeSys->UpdtStr();
	
	/*游戏模式文本*/
	fread(data->ModeName, sizeof(data->ModeName), 1, read);
	
	/*玩家ID*/
	fread(&data->Uid1, sizeof(data->Uid1), 1, read);
	fread(&data->Uid2, sizeof(data->Uid2), 1, read); 
	
	/*存档预览图片*/
	ReadBMP(&data->IM_Save_Cover,read,0x18744);
	ReadBMP(&data->IM_Save_Blur,read,0x18744);
	FreePicture(&data->IM_Save_Cover);
	FreePicture(&data->IM_Save_Blur);
	
	/*游戏数据*/
	fread(&GameMode, sizeof(GameMode), 1, read);
	fread(&Violation, sizeof(Violation), 1, read); 
	fread(&Sides, sizeof(Sides), 1, read); 
	fread(&Power, sizeof(Power), 1, read); 
	fread(&Rotation, sizeof(Rotation), 1, read); 
	fread(&InHoleFinished, sizeof(InHoleFinished), 1, read); 
	fread(&Draw_R, sizeof(Draw_R), 1, read); 
	fread(&WhichHole, sizeof(WhichHole), 1, read); 
	fread(&FixConst, sizeof(FixConst), 1, read); 
	fread(&CueTheta, sizeof(CueTheta), 1, read); 
	fread(&K_Friction, sizeof(K_Friction), 1, read); 
	fread(&NUM, sizeof(NUM), 1, read);
	
	/*重要坐标*/
	fread(&HitPosition_X, sizeof(HitPosition_X), 1, read);
	fread(&HitPosition_Y, sizeof(HitPosition_Y), 1, read);
	fread(&CurrentPosition_X, sizeof(CurrentPosition_X), 1, read);
	fread(&CurrentPosition_Y, sizeof(CurrentPosition_Y), 1, read);
	fread(&CueBallPosition_X, sizeof(CueBallPosition_X), 1, read);
	fread(&CueBallPosition_Y, sizeof(CueBallPosition_Y), 1, read);
	
	/*状态机*/
	fread(&LoadState, sizeof(LoadState), 1, read);
	
	/*台球数据*/
	fread(&Ball, sizeof(Ball[0]), 22, read); 
	
	/*模式规则结构体*/
	switch(GameMode)
	{
		case GM_SINGLEPLAYER:
			AI_Init();
			fread(&Classic, sizeof(Classic), 1, read); 
		break;
		case GM_MULTIPLAYER:
			fread(&Classic, sizeof(Classic), 1, read); 
		break;
		case GM_POOL:
			fread(&Pool, sizeof(Pool), 1, read); 
		break;
		case GM_SNOOKER:
			fread(&Snooker, sizeof(Snooker), 1, read); 
		break;
		case GM_TRAINNING:
			fread(&Trainning, sizeof(Trainning), 1, read); 
		break;
	}
	
	/*有效大小*/ 
	printf("Size: %d (%X) Byte\n",ftell(read),ftell(read));
	
	/*字节对齐*/ 
	char ch=0;
	for(;ftell(read)<SAVE_SIZE*i;)
		fread(&ch, sizeof(ch), 1, read); 
	printf("Size: %d (%X) Byte\n",ftell(read),ftell(read));
	
	fclose(read);
	
	ifLoad = 1;
}

void SaveSave(int i)
{
	FILE *save;
	save = fopen("Save/data.sav", "rb+");
	if(!save){
		ErrorMsg("存档文件写入失败！");
		return; 
	}
	
	fseek(save,SAVE_SIZE*(i-1),SEEK_SET);
	
	/*存档标志数据结构体*/
	SAVE savedata;
	LPSAVE data=&savedata;
	
	/*存档位置是否占用标识*/
	data->flag = true;
	fwrite(&data->flag, sizeof(data->flag), 1, save);
	
	/*存档时间*/
	time_t timep;
	time(&timep);
	timep+=8*3600;
	struct tm *p = gmtime(&timep);
	sprintf(data->Time,"%04d/%02d/%02d %02d:%02d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min);
	fwrite(data->Time, sizeof(data->Time), 1, save);
	
	/*时间系统*/
	fwrite(&GameTimeSys->countdown_n, sizeof(GameTimeSys->countdown_n), 1, save);
	
	/*游戏模式文本*/
	switch(GameMode)
	{
		case GM_SINGLEPLAYER:
			sprintf(data->ModeName,"经典台球人机对战"); 
		break;
		case GM_MULTIPLAYER:
			sprintf(data->ModeName,"经典台球双人对战"); 
		break;
		case GM_POOL:
			sprintf(data->ModeName,"美式台球双人对战"); 
		break;
		case GM_SNOOKER:
			sprintf(data->ModeName,"斯诺克双人对战"); 
		break;
		case GM_TRAINNING:
			sprintf(data->ModeName,"自由练习"); 
		break;
	}
	fwrite(data->ModeName, sizeof(data->ModeName), 1, save);
	
	/*玩家ID*/
	data->Uid1=User1->Uid;
	data->Uid2=User2->Uid;
	fwrite(&data->Uid1, sizeof(data->Uid1), 1, save);
	fwrite(&data->Uid2, sizeof(data->Uid2), 1, save); 
	
	/*存档预览图片*/
	MakeSizePicture(&data->IM_Save_Cover,IM_Pause_BK,245,136,NULL);
	MakeBlurPicture(&data->IM_Save_Blur,data->IM_Save_Cover,NULL,3);
	WriteBMP(data->IM_Save_Cover,save);
	WriteBMP(data->IM_Save_Blur,save); 
	FreePicture(&data->IM_Save_Cover);
	FreePicture(&data->IM_Save_Blur);
	
	/*游戏数据*/
	fwrite(&GameMode, sizeof(GameMode), 1, save);
	fwrite(&Violation, sizeof(Violation), 1, save); 
	fwrite(&Sides, sizeof(Sides), 1, save); 
	fwrite(&Power, sizeof(Power), 1, save); 
	fwrite(&Rotation, sizeof(Rotation), 1, save); 
	fwrite(&InHoleFinished, sizeof(InHoleFinished), 1, save); 
	fwrite(&Draw_R, sizeof(Draw_R), 1, save); 
	fwrite(&WhichHole, sizeof(WhichHole), 1, save); 
	fwrite(&FixConst, sizeof(FixConst), 1, save); 
	fwrite(&CueTheta, sizeof(CueTheta), 1, save); 
	fwrite(&K_Friction, sizeof(K_Friction), 1, save); 
	fwrite(&NUM, sizeof(NUM), 1, save); 
	
	/*重要坐标*/
	fwrite(&HitPosition_X, sizeof(HitPosition_X), 1, save);
	fwrite(&HitPosition_Y, sizeof(HitPosition_Y), 1, save);
	fwrite(&CurrentPosition_X, sizeof(CurrentPosition_X), 1, save);
	fwrite(&CurrentPosition_Y, sizeof(CurrentPosition_Y), 1, save);
	fwrite(&CueBallPosition_X, sizeof(CueBallPosition_X), 1, save);
	fwrite(&CueBallPosition_Y, sizeof(CueBallPosition_Y), 1, save);
	
	/*状态机*/
	fwrite(&LoadState, sizeof(LoadState), 1, save);
	
	/*台球数据*/
	fwrite(&Ball, sizeof(Ball[0]), 22, save); 
	
	/*模式规则结构体*/
	switch(GameMode)
	{
		case GM_SINGLEPLAYER:
			fwrite(&Classic, sizeof(Classic), 1, save); 
		break;
		case GM_MULTIPLAYER:
			fwrite(&Classic, sizeof(Classic), 1, save); 
		break;
		case GM_POOL:
			fwrite(&Pool, sizeof(Pool), 1, save); 
		break;
		case GM_SNOOKER:
			fwrite(&Snooker, sizeof(Snooker), 1, save); 
		break;
		case GM_TRAINNING:
			fwrite(&Trainning, sizeof(Trainning), 1, save); 
		break;
	}
	
	/*有效大小*/ 
	printf("Size: %d (%X) Byte\n",ftell(save),ftell(save));
	
	/*字节对齐*/ 
	char ch=0;
	for(;ftell(save)<SAVE_SIZE*i;)
		fwrite(&ch, sizeof(ch), 1, save); 
	printf("Size: %d (%X) Byte\n",ftell(save),ftell(save));

	fclose(save);
}

void DlteSave(int i)
{
	FILE *save;
	save = fopen("Save/data.sav", "rb+");
	if(!save){
		ErrorMsg("存档文件写入失败！");
		return; 
	}
	
	fseek(save,SAVE_SIZE*(i-1),SEEK_SET);
	
	/*存档标志数据结构体*/
	SAVE savedata;
	LPSAVE data=&savedata;
	
	/*存档位置是否占用标识*/
	data->flag = false;
	fwrite(&data->flag, sizeof(data->flag), 1, save);

	fclose(save);
}

