#include <math.h>
#include <text.h>
#include <stdio.h>
#include <media.h> 
#include <stdlib.h>
#include <pictures.h>
#include <graphics.h>
#include <extgraph.h> 

#include <gui.h>
#include <user.h>
#include <timesys.h>
#include <sets.h>
#include <test.h>
#include <public.h>
#include <callback.h>

extern LPUSER User1, User2;
extern LPTIMESYS GameTimeSys;
extern MODE GameMode;
extern struct PoolInformation
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
	int SolidSet;						//��ǰȫɫ���Ѱڷ����� 
	int StripSet;						//��ǰ��ɫ���Ѱڷ����� 
	int Inhole; 						//��ǰȫ�ֽ������� 
	int Ontable;						//��ǰȫ���������� 
	bool isCueInhole;					//ĸ���Ƿ���� 
	bool isBlackInhole;					//�жϺڰ��Ƿ���������
	int BallState[16];					//��¼ʮ������Ľ���״̬ 
	int NewInhole[16];					//��¼�µĽ���
	int TargetBall;						//��¼Ŀ���� 
	int isSet;							//δ�ڷ����Ƿ��׵�
	int SolidMoved;
	int StripMoved;
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

extern class SnookerInfomation
{
	bool isSuccessfullyBegin;
	bool isRedClear;
	int isRedTurn;
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
	int	End;
	int Winner; 
} Snooker;

/*ʤ����*/
extern int Winscore;
extern LPUSER WinUser;

extern LPUSER User1,User2;

extern int ifLoad;

void LevelUpdate()
{
	int Winner, ExpGet=0, GameTime;
	GameTimeSys->GetTime(TS_ED);
	GameTimeSys->GetTime(TS_AL);
	GameTime = (int)GameTimeSys->Total_t;
	printf("GameTime = %d\n", GameTime);
	
	/*����Ԥ��*/
	if(GameMode == GM_SINGLEPLAYER){
		if(Classic.Winner == 0){
			WinUser = User1;
			Winscore = Classic.ScorePlayer1;
		}
		else{
			WinUser = NULL;
			Winscore = Classic.ScorePlayer2;
		} 
		Pool.End = Snooker.End = Classic.End = 0;
		Classic.Winner = -1;
		return;
	}
	
	if(ifLoad){
		switch(GameMode)
		{
			case GM_MULTIPLAYER:
			{
				if(Classic.Winner == 0){
					WinUser = User1;
					Winscore = Classic.ScorePlayer1;
				}
				else{
					WinUser = User2;
					Winscore = Classic.ScorePlayer2;
				} 
				Pool.End = Snooker.End = Classic.End = 0;
				Classic.Winner = -1;
				return;
			} break;
			case GM_POOL:
			{
				if(Pool.Winner == 0){
					WinUser = User1;
					Winscore = Pool.SideSolid==0?Pool.SolidInhole:Pool.StripInhole;
				} 	
				else{
					WinUser = User2;
					Winscore = Pool.SideSolid==1?Pool.SolidInhole:Pool.StripInhole;
				} 
				Pool.End = Snooker.End = Classic.End = 0;
				Pool.Winner = -1;
				return;
			} break;
			case GM_SNOOKER:
			{
				if(Snooker.Winner == 0){
					WinUser = User1;
					Winscore = Snooker.Player1Score;
				} 
				else{
					WinUser = User2;
					Winscore = Snooker.Player2Score;
				} 
				Pool.End = Snooker.End = Classic.End = 0;
				Snooker.Winner = -1;
				return;
			} break;
		}
	} 
	
	int PrevLv_User1  = User1->Level;
	int PrevLv_User2  = User2->Level;
	int PrevExp_User1 = User1->Exp;
	int PrevExp_User2 = User2->Exp;
	
	User1->Total += 1;
	User2->Total += 1;
	switch(GameMode)
	{
		case GM_MULTIPLAYER:
		{
			Winner = Classic.Winner;
			ExpGet = 45;
			if(Winner == 0){
				User1->ClassicMultiWin ++;	
				WinUser = User1;
				Winscore = Classic.ScorePlayer1;
			}
			else{
				User2->ClassicMultiWin ++;
				WinUser = User2;
				Winscore = Classic.ScorePlayer2;
			} 
			Pool.End = Snooker.End = Classic.End = 0;
			Classic.Winner = -1;
		} break;
		case GM_POOL:
		{
			Winner = Pool.Winner;
			ExpGet = 75;
			if(Pool.SideClear[0])
				User1->Exp += 25;
			if(Pool.SideClear[1])
				User2->Exp += 25;
			if(Winner == 0){
				User1->PoolWin ++;
				WinUser = User1;
				Winscore = Pool.SideSolid==0?Pool.SolidInhole:Pool.StripInhole;
			} 	
			else{
				User2->PoolWin ++;
				WinUser = User2;
				Winscore = Pool.SideSolid==1?Pool.SolidInhole:Pool.StripInhole;
			} 
			Pool.End = Snooker.End = Classic.End = 0;
			Pool.Winner = -1;
		} break;
		case GM_SNOOKER:
		{
			Winner = Snooker.Winner;
			ExpGet = 105;
			if(Snooker.isRedClear)
				ExpGet += 30; 
			if(Winner == 0){
				User1->SnookerWin ++;	
				WinUser = User1;
				Winscore = Snooker.Player1Score;
			} 
			else{
				User2->SnookerWin ++;
				WinUser = User2;
				Winscore = Snooker.Player2Score;
			} 
			Pool.End = Snooker.End = Classic.End = 0;
			Snooker.Winner = -1;
		} break;
	}
	ExpGet += GameTime * 1.2;
	if(Winner == 0)
	{
		User1->Exp += ExpGet * 1.25;
		User2->Exp += ExpGet;
	}
	else
	{
		User2->Exp += ExpGet * 1.25;
		User1->Exp += ExpGet;		
	}
	while(User1->Exp >= 1000)
	{
		User1->Level += 1;
		User1->Exp -= 1000;
	}
	while(User2->Exp >= 1000)
	{
		User2->Level += 1;
		User2->Exp -= 1000;
	}	
	User1->PlayTime += GameTime;
	User2->PlayTime += GameTime;
	printf("----Infomation User1----\n");
	printf("Level:    %04d -> %04d\n", PrevLv_User1, User1->Level);
	printf("Exp:   	  %04d -> %04d\n", PrevExp_User1, User1->Exp);
	printf("PlayTime: %lld\n", User1->PlayTime);
	printf("CMWin:    %d\n", User1->ClassicMultiWin);
	printf("PWin:     %d\n", User1->PoolWin);
	printf("SWin:	  %d\n", User1->SnookerWin); 
	printf("Total:	  %d\n", User1->Total); 
	printf("---Infomation User2----\n");
	printf("Level:    %04d -> %04d\n", PrevLv_User2, User2->Level);
	printf("Exp:   	  %04d -> %04d\n", PrevExp_User2, User2->Exp);
	printf("PlayTime: %lld\n", User2->PlayTime);
	printf("CMWin:    %d\n", User2->ClassicMultiWin);
	printf("PWin:     %d\n", User2->PoolWin);
	printf("SWin:	  %d\n", User2->SnookerWin); 
	printf("Total:	  %d\n", User2->Total); 
	SaveUsers(User1);
	SaveUsers(User2); 
} 

