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
extern int Violation;								//�����ж� 
extern int Sides;									//<��浵����> ��սģʽ - �غ����������ж�����˭�Ļغϣ� 
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
    float Position_X;								//<��浵����> ������ˮƽ���� X 
    float Position_Y;								//<��浵����> �����괹ֱ���� Y
    float Velocity_X;								//���ٶ�ˮƽ���� X 
    float Velocity_Y;								//���ٶȴ�ֱ���� Y 
    int Moving;										//���Ƿ����ƶ� 
    int State;										//���Ƿ���� 
    int IfScored;									//���Ƿ��Ѿ��Ʒ� 
    int Score;										//��ļƷ����
    LPIMAGE Address;						  		//��ͼ���ļ��Ĵ洢·�� 
} Ball[22]; 

extern struct Information
{
	int isSuccessfullyBegin;			//�Ƿ�˳������ 
	int isSideConfirmed;				//�Ƿ��Ѿ�ȷ��˫����ɫ 
	int Turns;							//�غ�����һ�λ�������һ�غϣ� 
	int Reflect;						//һ�غ�����̨��Ե�������� 
	int Collide;						//һ�غ������������ײ�� 
	int Scored;							//һ�غ����Ƿ���򣬼�¼���ǵ�һ������ı��
	bool DoubleScored;					//���ã�����һ���������˫��ɫ����������ѡ��ɫ��Ȩ�� 
	int Winner;							//��ʤ��� 
	int End;							//�����Ƿ����
	bool SideClear[2];					//˫���Ƿ���ճ��ڰ����������� 
	int SideStrip;						//��ɫ����з� 
	int SideSolid;						//ȫɫ����з� 
	int SolidInhole;					//��ǰȫɫ��������� 
	int StripInhole;					//��ǰ��ɫ��������� 
	int Inhole; 						//��ǰȫ�ֽ������� 
	int Ontable;						//��ǰȫ���������� 
	bool isCueInhole;					//ĸ���Ƿ���� 
	bool isBlackInhole;					//�жϺڰ��Ƿ���������
	int BallState[16];					//��¼ʮ������Ľ���״̬ 
	int NewInhole[16];					//��¼�µĽ���
	int TargetBall;						//��¼Ŀ����
	int isSet;							//δ�ڷ����Ƿ��׵�  
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

/*��Ϸʱ��ϵͳ*/
extern LPTIMESYS GameTimeSys;

void LoadSave(int i,LPSAVE data)
{
	FILE *read;
	read = fopen("Save/data.sav", "rb+");
	if(!read){
		ErrorMsg("�浵�ļ���ȡʧ�ܣ�");
		return; 
	}
	
	fseek(read,SAVE_SIZE*(i-1),SEEK_SET);
	
	/*�浵λ���Ƿ�ռ�ñ�ʶ*/
	fread(&data->flag, sizeof(data->flag), 1, read);
	
	/*�浵ʱ��*/
	fread(data->Time, sizeof(data->Time), 1, read);
	
	/*ʱ��ϵͳ*/
	TIMESYS tmpSys;
	fread(&tmpSys.countdown_n, sizeof(tmpSys.countdown_n), 1, read);
	
	/*��Ϸģʽ�ı�*/
	fread(data->ModeName, sizeof(data->ModeName), 1, read);
	
	/*���ID*/
	fread(&data->Uid1, sizeof(data->Uid1), 1, read);
	fread(&data->Uid2, sizeof(data->Uid2), 1, read); 
	
	/*�浵Ԥ��ͼƬ*/
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
		ErrorMsg("�浵�ļ���ȡʧ�ܣ�");
		return; 
	}
	
	fseek(read,SAVE_SIZE*(i-1),SEEK_SET);
	
	/*�浵��־���ݽṹ��*/
	SAVE savedata;
	LPSAVE data=&savedata;
	
	/*�浵λ���Ƿ�ռ�ñ�ʶ*/
	fread(&data->flag, sizeof(data->flag), 1, read);
	
	/*�浵ʱ��*/
	fread(data->Time, sizeof(data->Time), 1, read);
	
	/*ʱ��ϵͳ*/
	fread(&GameTimeSys->countdown_n, sizeof(GameTimeSys->countdown_n), 1, read);
	GameTimeSys->UpdtStr();
	
	/*��Ϸģʽ�ı�*/
	fread(data->ModeName, sizeof(data->ModeName), 1, read);
	
	/*���ID*/
	fread(&data->Uid1, sizeof(data->Uid1), 1, read);
	fread(&data->Uid2, sizeof(data->Uid2), 1, read); 
	
	/*�浵Ԥ��ͼƬ*/
	ReadBMP(&data->IM_Save_Cover,read,0x18744);
	ReadBMP(&data->IM_Save_Blur,read,0x18744);
	FreePicture(&data->IM_Save_Cover);
	FreePicture(&data->IM_Save_Blur);
	
	/*��Ϸ����*/
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
	
	/*��Ҫ����*/
	fread(&HitPosition_X, sizeof(HitPosition_X), 1, read);
	fread(&HitPosition_Y, sizeof(HitPosition_Y), 1, read);
	fread(&CurrentPosition_X, sizeof(CurrentPosition_X), 1, read);
	fread(&CurrentPosition_Y, sizeof(CurrentPosition_Y), 1, read);
	fread(&CueBallPosition_X, sizeof(CueBallPosition_X), 1, read);
	fread(&CueBallPosition_Y, sizeof(CueBallPosition_Y), 1, read);
	
	/*״̬��*/
	fread(&LoadState, sizeof(LoadState), 1, read);
	
	/*̨������*/
	fread(&Ball, sizeof(Ball[0]), 22, read); 
	
	/*ģʽ����ṹ��*/
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
	
	/*��Ч��С*/ 
	printf("Size: %d (%X) Byte\n",ftell(read),ftell(read));
	
	/*�ֽڶ���*/ 
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
		ErrorMsg("�浵�ļ�д��ʧ�ܣ�");
		return; 
	}
	
	fseek(save,SAVE_SIZE*(i-1),SEEK_SET);
	
	/*�浵��־���ݽṹ��*/
	SAVE savedata;
	LPSAVE data=&savedata;
	
	/*�浵λ���Ƿ�ռ�ñ�ʶ*/
	data->flag = true;
	fwrite(&data->flag, sizeof(data->flag), 1, save);
	
	/*�浵ʱ��*/
	time_t timep;
	time(&timep);
	timep+=8*3600;
	struct tm *p = gmtime(&timep);
	sprintf(data->Time,"%04d/%02d/%02d %02d:%02d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min);
	fwrite(data->Time, sizeof(data->Time), 1, save);
	
	/*ʱ��ϵͳ*/
	fwrite(&GameTimeSys->countdown_n, sizeof(GameTimeSys->countdown_n), 1, save);
	
	/*��Ϸģʽ�ı�*/
	switch(GameMode)
	{
		case GM_SINGLEPLAYER:
			sprintf(data->ModeName,"����̨���˻���ս"); 
		break;
		case GM_MULTIPLAYER:
			sprintf(data->ModeName,"����̨��˫�˶�ս"); 
		break;
		case GM_POOL:
			sprintf(data->ModeName,"��ʽ̨��˫�˶�ս"); 
		break;
		case GM_SNOOKER:
			sprintf(data->ModeName,"˹ŵ��˫�˶�ս"); 
		break;
		case GM_TRAINNING:
			sprintf(data->ModeName,"������ϰ"); 
		break;
	}
	fwrite(data->ModeName, sizeof(data->ModeName), 1, save);
	
	/*���ID*/
	data->Uid1=User1->Uid;
	data->Uid2=User2->Uid;
	fwrite(&data->Uid1, sizeof(data->Uid1), 1, save);
	fwrite(&data->Uid2, sizeof(data->Uid2), 1, save); 
	
	/*�浵Ԥ��ͼƬ*/
	MakeSizePicture(&data->IM_Save_Cover,IM_Pause_BK,245,136,NULL);
	MakeBlurPicture(&data->IM_Save_Blur,data->IM_Save_Cover,NULL,3);
	WriteBMP(data->IM_Save_Cover,save);
	WriteBMP(data->IM_Save_Blur,save); 
	FreePicture(&data->IM_Save_Cover);
	FreePicture(&data->IM_Save_Blur);
	
	/*��Ϸ����*/
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
	
	/*��Ҫ����*/
	fwrite(&HitPosition_X, sizeof(HitPosition_X), 1, save);
	fwrite(&HitPosition_Y, sizeof(HitPosition_Y), 1, save);
	fwrite(&CurrentPosition_X, sizeof(CurrentPosition_X), 1, save);
	fwrite(&CurrentPosition_Y, sizeof(CurrentPosition_Y), 1, save);
	fwrite(&CueBallPosition_X, sizeof(CueBallPosition_X), 1, save);
	fwrite(&CueBallPosition_Y, sizeof(CueBallPosition_Y), 1, save);
	
	/*״̬��*/
	fwrite(&LoadState, sizeof(LoadState), 1, save);
	
	/*̨������*/
	fwrite(&Ball, sizeof(Ball[0]), 22, save); 
	
	/*ģʽ����ṹ��*/
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
	
	/*��Ч��С*/ 
	printf("Size: %d (%X) Byte\n",ftell(save),ftell(save));
	
	/*�ֽڶ���*/ 
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
		ErrorMsg("�浵�ļ�д��ʧ�ܣ�");
		return; 
	}
	
	fseek(save,SAVE_SIZE*(i-1),SEEK_SET);
	
	/*�浵��־���ݽṹ��*/
	SAVE savedata;
	LPSAVE data=&savedata;
	
	/*�浵λ���Ƿ�ռ�ñ�ʶ*/
	data->flag = false;
	fwrite(&data->flag, sizeof(data->flag), 1, save);

	fclose(save);
}

