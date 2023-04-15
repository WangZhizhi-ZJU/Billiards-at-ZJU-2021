#include <math.h>
#include <text.h>
#include <stdio.h>
#include <media.h> 
#include <stdlib.h>
#include <string.h>
#include <vector.h>
#include <physics.h>
#include <pictures.h>
#include <graphics.h>
#include <extgraph.h> 

#include <sets.h>
#include <test.h>
#include <public.h>
#include <timesys.h>
#include <callback.h>
#include <tooltips.h>
#include <user.h>
#include <save.h>
#include <course.h>

#include <gui.h>

extern float ForceBarStep;
extern int RefreshRate;
 
extern MODE GameMode;

extern int TimeSysID;
extern int MenuMusicID;
extern int CollisionMusicID;
extern int BackGroundMusicID;

extern HWND TimeSysWindow;
extern HWND MenuMusicWindow;
extern HWND CollisionMusicWindow;
extern HWND BackGroundMusicWindow;

extern MOUSESTATE MOUSE_STATE;
extern KEYBOARDSTATE KEYBOARD_STATE;

extern int NUM;
extern double X, Y;
extern float const MaxVelocity;
extern float Power;
extern int Sides;									//<需存档变量> 对战模式 - 回合数（用于判断属于谁的回合） 
extern int Violation;								//犯规判定 
extern int Menu;
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
extern char CollideAddress[10][9];
extern float const ShelfGap;
extern float Shelf_Y[15];
extern int ViolationShelf_X[2][5];
extern LPIMAGE  IM_Table,
				IM_Player0,
				IM_Player1,
				IM_Player2,
				IM_AI,
				IM_ForceBar,
				IM_RedPoint,
				IM_Stick,
				IM_Stick0,
				IM_Stick1,
				IM_Billiards0,
				IM_PoolBilliards3,
				IM_Billiards1,
				IM_Billiards2,
				IM_Billiards3,
				IM_Billiards4,
				IM_Billiards8,
				IM_Billiards6,
				IM_Billiards7,
				IM_PoolBilliards8,
				IM_PoolBilliards13,
				IM_Violation,
				IM_Close_0,
				IM_Close_1,
				IM_Close_2,
				IM_Minimize_0,
				IM_Minimize_1,
				IM_Minimize_2,
				IM_Maximize_0,
				IM_Target,
				IM_Split,
				IM_Info_0,
				IM_Info_1,
				IM_Info_2,
				IM_Setting_0,
				IM_Setting_1,
				IM_Setting_2,
				IM_Game_0,
				IM_Game_1,
				IM_Game_2,
				IM_Icon,
				IM_StartMenu_0_0,
				IM_StartMenu_0_1,
				IM_StartMenu_0_2,
				IM_StartMenu_0_3,
				IM_StartMenu_0_4,
				IM_StartMenu_1_0,
				IM_StartMenu_1_1,
				IM_StartMenu_1_2,
				IM_StartMenu_1_3,
				IM_StartMenu_1_4,
				IM_GreyBar,
				IM_WhiteBar,
				IM_Frame,
				IM_StartGame_0,
				IM_StartGame_1,
				IM_LoadPage,
				IM_LoadBar,
				IM_Masks_0,
				IM_Masks_1,
				IM_Choice,
				IM_Begin_0,
				IM_Begin_1,
				IM_Begin_2,
				IM_GM_BK_0_0,
				IM_GM_BK_1_0,
				IM_GM_BT_0_0,
				IM_GM_Esc_0,
				IM_GM_Pool,
				IM_GM_Pool_Blur,
				IM_GM_Practice,
				IM_GM_Practice_Blur,
				IM_GM_AI,
				IM_GM_AI_Blur,
				IM_GM_Classical,
				IM_GM_Classical_Blur,
				IM_GM_Snooker,
				IM_GM_Snooker_Blur,
				IM_Pause_0,
				IM_Pause_Continue,
				IM_Pause_Save,
				IM_Pause_Quit,
				IM_Colour,
				IM_StripSide,
				IM_SolidSide,
				IM_BlackSide,
				IM_BilliardsEmoji0,
				IM_BilliardsEmoji1,
				IM_BilliardsEmoji2,
				IM_BilliardsEmoji3,
				IM_BilliardsEmoji4,
				IM_BilliardsEmoji5,
				IM_BilliardsEmoji6,
				IM_BilliardsEmoji7,
				IM_BilliardsEmoji8,
				IM_BilliardsEmoji9,
				IM_LoadSave_1,
				IM_LoadSave_2,
				IM_LoadSave_3,
				IM_LoadSave_4,
				IM_LoadSave_5,
				IM_SetTheta,
				IM_ExpBar_0,
				IM_ExpBar_1,
				IM_Register,
				IM_TextBar,
				IM_Reg_OK,
				IM_Edit,
				IM_Eagle,
				IM_Cloth,
				IM_ScoreBoard,
				IM_Individual,
				IM_AvatarBack,
				IM_UserButton,
				IM_SavePage_Btn,
				IM_Save_Btn0,
				IM_Save_Btn1,
				IM_Save_Btn2,
				IM_Save_Del,
				IM_Save_Load,
				IM_Save_Save,
				IM_Crown,
				IM_WinBtn0,
				IM_WinBtn1,
				IM_StickArr[9][2],
				IM_Select,
				IM_UnSelect;
				
extern LPFONTS FT_Num,
			   FT_Title,
			   FT_Button,
			   FT_Text;
			  
extern LPTOOLTIPS TT_Title,
		   		  TT_Client;

extern LPMUSIC 	MU_Begin,
				MU_Pause,
				MU_Saved,
				MU_Recycle,
				MU_Collide,
				MU_Reflect,
				MU_Inhole,
				MU_OnButton,
				MU_SwitchMenu,
				MU_SwitchSide,
				MU_Quit1,
				MU_Quit2,
				MU_Close1,
				MU_Close2,
				MU_CountDown,
				MU_Win; 

/*游戏时间系统*/
extern LPTIMESYS GameTimeSys; 

/*玩家信息*/
extern LPUSER User1, User2;

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

/*文件版本信息*/
extern char IF_FileTitle[100];

/*-------------------------------------------------------------------------------------------------*/
/*标题栏区域Begin*/

/*标题栏控件*/ 
extern int TBT_minimize,TBT_close,TBT_info,TBT_setting,TBT_game;
static int f_minimize=0,f_close=0,f_info=0,f_setting=0,f_game=0;

extern int CourseAndSetting;

void InitWndTitle()
{
	InitToolTips(&TT_Title,NULL);
	AddToolTips(TT_Title,(UID)&TBT_close,1238,13,28,28,"退出游戏");
	AddToolTips(TT_Title,(UID)&TBT_minimize,1162,13,28,28,"最小化");
	AddToolTips(TT_Title,(UID)&TBT_info,1104,13,28,28,"关于作者");
	AddToolTips(TT_Title,(UID)&TBT_setting,1066,13,28,28,"游戏设置");
	AddToolTips(TT_Title,(UID)&TBT_game,1028,13,28,28,"游戏教程");
	DrawWndTitle(); 
}

void DrawWndTitle()
{
	DrawAreaPicture(IM_Table,0,0,1280,773,0,0,1280,TITLEHEIGHT,NULL,NULL);
	DrawPicture(IM_Icon, 13, 11, 32, 32, NULL, NULL);
	PutTextB(IF_FileTitle,53,2,240,55,132,85,33,28,FT_Title,NULL,NULL);
	switch(TBT_close){
		case 0:
			if(f_close)
				f_close = !f_close;
			DrawPicture(IM_Close_0,1238,13,28,28,NULL,NULL);
		break;
		case 1:
			if(!f_close){
				f_close = !f_close;
				PlayOnBtMusic();
			}
			DrawPicture(IM_Close_1,1238,13,28,28,NULL,NULL);
		break;
		case 2:
			DrawPicture(IM_Close_2,1238,13,28,28,NULL,NULL);
		break;
	}
	DrawPicture(IM_Maximize_0,1200,13,28,28,NULL,NULL);
	switch(TBT_minimize){
		case 0:
			if(f_minimize)
				f_minimize = !f_minimize;
			DrawPicture(IM_Minimize_0,1162,13,28,28,NULL,NULL);
		break;
		case 1:
			if(!f_minimize){
				f_minimize = !f_minimize;
				PlayOnBtMusic();
			}
			DrawPicture(IM_Minimize_1,1162,13,28,28,NULL,NULL);
		break;
		case 2:
			DrawPicture(IM_Minimize_2,1162,13,28,28,NULL,NULL);
		break;
	}
	if(CourseAndSetting){
		DrawPicture(IM_Split,1147,13,1,28,NULL,NULL);
	}
	switch(TBT_info){
		case 0:
			if(f_info)
				f_info = !f_info;
			if(CourseAndSetting){
				DrawPicture(IM_Info_0,1104,13,28,28,NULL,NULL);
			}
		break;
		case 1:
			if(!f_info){
				f_info = !f_info;
				PlayOnBtMusic();
			}
			DrawPicture(IM_Info_1,1104,13,28,28,NULL,NULL);
		break;
		case 2:
			DrawPicture(IM_Info_2,1104,13,28,28,NULL,NULL);
		break;
	}
	switch(TBT_setting){
		case 0:
			if(f_setting)
				f_setting = !f_setting;
			if(CourseAndSetting){
				DrawPicture(IM_Setting_0,1066,13,28,28,NULL,NULL);
			}
		break;
		case 1:
			if(!f_setting){
				f_setting = !f_setting;
				PlayOnBtMusic();
			}
			DrawPicture(IM_Setting_1,1066,13,28,28,NULL,NULL);
		break;
		case 2:
			DrawPicture(IM_Setting_2,1066,13,28,28,NULL,NULL);
		break;
	}
	switch(TBT_game){
		case 0:
			if(f_game)
				f_game = !f_game;
			if(CourseAndSetting){
				DrawPicture(IM_Game_0,1028,13,28,28,NULL,NULL);
			}
		break;
		case 1:
			if(!f_game){
				f_game = !f_game;
				PlayOnBtMusic();
			}
			DrawPicture(IM_Game_1,1028,13,28,28,NULL,NULL);
		break;
		case 2:
			DrawPicture(IM_Game_2,1028,13,28,28,NULL,NULL);
		break;
	}
}

/*标题栏区域End*/
/*-------------------------------------------------------------------------------------------------*/
/*开始界面Begin*/

static int beginpage=0;
static int beginstage=0;

void StartBeginPage(){
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	DrawAlphaPicture(IM_Masks_0,0,TITLEHEIGHT,1280,720,NULL,NULL,255);
	DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,255);
	startTimer(TM_BEGIN_PAGE,20);
}

void EndBeginPage(){
	cancelTimer(TM_BEGIN_PAGE);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
}

void AddBeginPage(){
	beginpage++;
	switch(beginstage){
		case 0:
			/*浙江大学渐显*/ 
			if(beginpage==95){
				beginpage=0;
				beginstage++;
			}
		break;
		case 1:
			/*浙江大学渐隐step1*/ 
			if(beginpage==20){
				beginpage=0;
				beginstage++;
			}
		break;
		case 2:
			/*浙江大学渐隐step2*/ 
			if(beginpage==20){
				beginpage=0;
				beginstage++;
			}
		break;
		case 3:
			/*浙江大学渐隐step3*/ 
			if(beginpage==8){
				beginpage=0;
				beginstage++;
			}
		break;
		case 4:
			/*白色清屏*/ 
			if(beginpage==7){
				beginpage=0;
				beginstage++;
			}
		break;
		case 5:
			/*球图标渐显*/ 
			if(beginpage==5){
				beginpage=0;
				beginstage++;
			}
		break;
		case 6:
			/*球图标放大*/ 
			if(beginpage==5){
				beginpage=0;
				beginstage++;
			}
		break;
		case 7:
			/*球图标缩小*/ 
			if(beginpage==5){
				beginpage=0;
				beginstage++;
			}
		break;
		case 8:
			/*球图标滚动*/ 
			if(beginpage==42){
				beginpage=0;
				beginstage++;
			}
		break;
		case 9:
			/*等待*/ 
			if(beginpage==15){
				beginpage=0;
				beginstage++;
			}
		break;
		case 10:
			/*Logo全显从左到右*/ 
			if(beginpage==60){
				beginpage=0;
				beginstage++;
			}
		break;
		case 11:
			/*等待*/ 
			if(beginpage==60){
				beginpage=0;
				beginstage++;
			}
		break;
		case 12:
			/*Logo渐隐step1*/ 
			if(beginpage==20){
				beginpage=0;
				beginstage++;
			}
		break;
		case 13:
			/*Logo渐隐step2*/ 
			if(beginpage==20){
				beginpage=0;
				beginstage++;
			}
		break;
		case 14:
			/*Logo渐隐step3*/ 
			if(beginpage==20){
				beginpage=0;
				beginstage++;
			}
		break;
		case 15:
			/*Logo渐隐step4*/ 
			if(beginpage==10){
				beginpage=0;
				beginstage++;
			}
		break;
	}
}

void DrawBeginPag(){
	switch(beginstage){
		case 0:
			DrawAlphaPicture(IM_Begin_0,1,1+TITLEHEIGHT,1278,718,NULL,NULL,6);
		break;
		case 1:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,16);
		break;
		case 2:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,32);
		break;
		case 3:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,48);
		break;
		case 4:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,255);
		break;
		case 5:
			DrawAlphaPicture(IM_Begin_1,575,295+TITLEHEIGHT,130,130,NULL,NULL,16);
		break;
		case 6:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,255);
			DrawAvatar(IM_Begin_1,575-beginpage,295+TITLEHEIGHT-beginpage,130+beginpage*2,130+beginpage*2,NULL,NULL);
		break;
		case 7:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,255);
			DrawAvatar(IM_Begin_1,575+beginpage,295+TITLEHEIGHT+beginpage,130-beginpage*2,130-beginpage*2,NULL,NULL);
		break;
		case 8:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,255);
			RotatePictureA(IM_Begin_1,575-(beginpage+1)*5,295+TITLEHEIGHT,130,130,(beginpage+1)*8.57,NULL,NULL);
		break;
		case 9:
			;
		break;
		case 10:
			DrawAreaPicture(IM_Begin_2,365,295+TITLEHEIGHT,553,150,0,0,140+beginpage*7,150,NULL,NULL);
		break;
		case 11:
			;
		break;
		case 12:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,16);
		break;
		case 13:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,32);
		break;
		case 14:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,48);
		break;
		case 15:
			DrawAlphaPicture(IM_Masks_1,1,1+TITLEHEIGHT,1278,718,NULL,NULL,64);
		break;
		case 16:
			EndBeginPage();
			StartMasks(StartRegstMenu);
			extern int CourseAndSetting;
			/*教程和设置激活*/
    		CourseAndSetting=1;
    		DrawWndTitle();
		break;
	}
}

/*开始界面End*/
/*-------------------------------------------------------------------------------------------------*/
/*开始菜单Begin*/

/*客户区控件*/ 
/*StartMenu*/
static char ST_text[20];
extern int BT_ST_start,BT_ST_load,BT_ST_exit,BT_ST_player1,BT_ST_player2;
static int f_st_start=0,f_st_load=0,f_st_exit=0,f_st_player1=0,f_st_player2=0;

void DrawStartBtn(){
	switch(BT_ST_player1){
		case 0:
			if(f_st_player1)
				f_st_player1 = !f_st_player1;
			DrawAvatar(User1->Avatar,808,88+TITLEHEIGHT,80,80,NULL,NULL);
			DrawAlphaPicture(IM_GreyBar,888,88+TITLEHEIGHT,270,80,NULL,NULL,128);
		break;
		case 1:
			if(!f_st_player1){
				f_st_player1 = !f_st_player1;
				PlayOnBtMusic();
			}
			DrawPicture(IM_Frame,808,88+TITLEHEIGHT,80,80,NULL,NULL);
			DrawAvatar(User1->Avatar,810,90+TITLEHEIGHT,76,76,NULL,NULL);
			DrawAlphaPicture(IM_GreyBar,888,88+TITLEHEIGHT,270,80,NULL,NULL,160);
		break;
		case 2:
			DrawPicture(IM_Frame,808,88+TITLEHEIGHT,80,80,NULL,NULL);
			DrawAvatar(User1->Avatar,811,91+TITLEHEIGHT,74,74,NULL,NULL);
			DrawAlphaPicture(IM_GreyBar,888,88+TITLEHEIGHT,270,80,NULL,NULL,192);
		break;
	}
	switch(BT_ST_player2){
		case 0:
			if(f_st_player2)
				f_st_player2 = !f_st_player2;
			DrawAvatar(User2->Avatar,808,188+TITLEHEIGHT,80,80,NULL,NULL);
			DrawAlphaPicture(IM_WhiteBar,888,188+TITLEHEIGHT,270,80,NULL,NULL,128);
		break;
		case 1:
			if(!f_st_player2){
				f_st_player2 = !f_st_player2;
				PlayOnBtMusic();
			}
			DrawPicture(IM_Frame,808,188+TITLEHEIGHT,80,80,NULL,NULL);
			DrawAvatar(User2->Avatar,810,190+TITLEHEIGHT,76,76,NULL,NULL);
			DrawAlphaPicture(IM_WhiteBar,888,188+TITLEHEIGHT,270,80,NULL,NULL,160);
			
		break;
		case 2:
			DrawPicture(IM_Frame,808,188+TITLEHEIGHT,80,80,NULL,NULL);
			DrawAvatar(User2->Avatar,811,191+TITLEHEIGHT,74,74,NULL,NULL);
			DrawAlphaPicture(IM_WhiteBar,888,188+TITLEHEIGHT,270,80,NULL,NULL,192);
		break;
	}
	switch(BT_ST_start){
		case 0:
			if(f_st_start)
				f_st_start = !f_st_start;
			Draw32BitPicture(IM_StartGame_0,503,355+TITLEHEIGHT,270,80,NULL,NULL,48);
		break;
		case 1:
			if(!f_st_start){
				f_st_start = !f_st_start;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_StartGame_0,503,355+TITLEHEIGHT,270,80,NULL,NULL,80);
		break;
		case 2:
			Draw32BitPicture(IM_StartGame_0,503,355+TITLEHEIGHT,270,80,NULL,NULL,112);
		break;
	}
	switch(BT_ST_load){
		case 0:
			if(f_st_load)
				f_st_load = !f_st_load;
			Draw32BitPicture(IM_StartGame_0,534,464+TITLEHEIGHT,207,60,NULL,NULL,48);
		break;
		case 1:
			if(!f_st_load){
				f_st_load = !f_st_load;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_StartGame_0,534,464+TITLEHEIGHT,207,60,NULL,NULL,80);
		break;
		case 2:
			Draw32BitPicture(IM_StartGame_0,534,464+TITLEHEIGHT,207,60,NULL,NULL,112);
		break;
	}
	switch(BT_ST_exit){
		case 0:
			if(f_st_exit)
				f_st_exit = !f_st_exit;
			Draw32BitPicture(IM_StartGame_0,534,555+TITLEHEIGHT,207,60,NULL,NULL,48);
		break;
		case 1:
			if(!f_st_exit){
				f_st_exit = !f_st_exit;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_StartGame_0,534,555+TITLEHEIGHT,207,60,NULL,NULL,80);
		break;
		case 2:
			Draw32BitPicture(IM_StartGame_0,534,555+TITLEHEIGHT,207,60,NULL,NULL,112);
		break;
	}
	PutText("开始游戏",503,355+TITLEHEIGHT,773,435+TITLEHEIGHT,255,255,255,60,FT_Button,NULL,NULL);
	PutText("继续游戏",534,464+TITLEHEIGHT,741,524+TITLEHEIGHT,255,255,255,45,FT_Button,NULL,NULL);
	PutText("退出游戏",534,555+TITLEHEIGHT,741,615+TITLEHEIGHT,255,255,255,45,FT_Button,NULL,NULL);
	PutTextB(User1->Name,900,88+TITLEHEIGHT,1158,138+TITLEHEIGHT,255,255,255,25,FT_Text,NULL,NULL);
	PutTextB(User2->Name,900,188+TITLEHEIGHT,1158,238+TITLEHEIGHT,255,255,255,25,FT_Text,NULL,NULL);
	sprintf(ST_text,"Lv.%-3d",User1->Level);
	PutTextC(ST_text,900,88+TITLEHEIGHT,1140,138+TITLEHEIGHT,255,255,255,18,FT_Text,NULL,NULL);
	sprintf(ST_text,"Lv.%-3d",User2->Level);
	PutTextC(ST_text,900,188+TITLEHEIGHT,1140,238+TITLEHEIGHT,255,255,255,18,FT_Text,NULL,NULL);
	DrawPicture(IM_ExpBar_0,903,143+TITLEHEIGHT,240,8,NULL,NULL);
	if(User1->Exp) DrawAreaPicture(IM_ExpBar_1,903,143+TITLEHEIGHT,240,8,0,0,(int)240*User1->Exp/1000.0,8,NULL,NULL);
	DrawPicture(IM_ExpBar_0,903,243+TITLEHEIGHT,240,8,NULL,NULL);
	if(User2->Exp) DrawAreaPicture(IM_ExpBar_1,903,243+TITLEHEIGHT,240,8,0,0,(int)240*User2->Exp/1000.0,8,NULL,NULL);
}

void StartStartMenu(){
	InitToolTips(&TT_Client,NULL);
	AddToolTips(TT_Client,(UID)&User1,808,88+TITLEHEIGHT,350,80,"点击设置玩家一信息");
	AddToolTips(TT_Client,(UID)&User2,808,188+TITLEHEIGHT,350,80,"点击设置玩家二信息");
	startTimer(TM_SHAKE_START_MENU,1000);
	startTimer(TM_SHAKE,10);
	MOUSE_STATE=MS_START_MENU;
	KEYBOARD_STATE=KB_START_MENU;
}

void EndStartMenu(){
	DelToolTips(TT_Client,(UID)&User1);
	DelToolTips(TT_Client,(UID)&User2);
	DestroyToolTips(&TT_Client);
	cancelTimer(TM_SHAKE_START_MENU);
	cancelTimer(TM_SHAKE);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
}

static ShakePage=1;

void RsetShakeMnu(){
	ShakePage=1;
}

void AddShakePage(){
	if(ShakePage<10) ShakePage++;
}

void DrawShakeMnu(){
	switch(Menu){
		case 0:
			switch(ShakePage){
				case 1: case 6:
					DrawPicture(IM_StartMenu_0_1,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
				case 2: case 7:
					DrawPicture(IM_StartMenu_0_2,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
				case 3: case 8:
					DrawPicture(IM_StartMenu_0_3,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
				case 4: case 9:
					DrawPicture(IM_StartMenu_0_4,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
				case 5: case 10:
					DrawPicture(IM_StartMenu_0_0,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
			}
		break;
		case 1:
			switch(ShakePage){
				case 1: case 6:
					DrawPicture(IM_StartMenu_1_1,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
				case 2: case 7:
					DrawPicture(IM_StartMenu_1_2,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
				case 3: case 8:
					DrawPicture(IM_StartMenu_1_3,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
				case 4: case 9:
					DrawPicture(IM_StartMenu_1_4,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
				case 5: case 10:
					DrawPicture(IM_StartMenu_1_0,0,TITLEHEIGHT,1280,720,NULL,NULL);
				break;
			}		
		break;
	}	
}

/*开始菜单End*/
/*-------------------------------------------------------------------------------------------------*/
/*加载界面Begin*/

static int LoadPage=1;
static int LoadText=1;

extern int ifLoad;
extern int LoadState;

void StartLoad(){
	LoadPage=1;
	LoadText=1;
	MOUSE_STATE = MS_NULL;
	KEYBOARD_STATE = KB_NULL;
	startTimer(TM_LOAD_PAGE,5);
	startTimer(TM_LOAD_TEXT,500);
}

void EndLoad(){
	cancelTimer(TM_LOAD_PAGE);
	cancelTimer(TM_LOAD_TEXT);
	MOUSE_STATE = MS_NULL;
	KEYBOARD_STATE = KB_NULL;
}

void AddLoadTexts(){
	LoadText++;
	if(LoadText>4) LoadText=1;
}

void AddLoadPages(){
	LoadPage++;
	if(log(1+LoadPage/100.0)>1.1){
		EndLoad();
		/*初始化需计算参数*/
		InitParameter(); 
		/*初始化桌面*/
		LoadBalls();
		SetBalls();
		GameTimeSys->GetTime(TS_BG);
		if(ifLoad){
			switch(LoadState){
				case MS_GET_FORCE:
					if(GameMode != GM_TRAINNING)
						ReStartCountDown();
					StartMasks(StartGetPower);
				break;
				case MS_GET_HIT_POSITION:
					if(GameMode != GM_TRAINNING)
						ReStartCountDown();
					StartMasks(StartGetHit);
				break;
				case MS_FREE_RESET_CUEBALL:
					if(GameMode != GM_TRAINNING)
						ReStartCountDown();
					StartMasks(StartFreeResetCueBall);
					ShowHintMsge("自由球：请选择合适位置摆放母球"); 
				break;
				case MS_PREPARE_TO_HIT:
					StartMasks(StartPrepare);
				break;
				default:
					StartMasks(StartHit);
				break;
			} 
		} 
		else
		{
			//Hereitis
			if(GameMode != GM_SNOOKER && GameMode != GM_POOL)
				StartMasks(StartGetHit);
			else{
				if(GameMode == GM_SNOOKER){
					ShowHintMsge("自由球：请在半圆区域内摆放母球"); 
				}else{
					ShowHintMsge("自由球：请在左半区域内摆放母球"); 
				}
				StartMasks(StartFreeResetCueBall);
			} 
			startTimer(TM_START_SIDE,900);
		}	
	}
}

void DrawLoadPage(){
	DrawPicture(IM_LoadPage,0,TITLEHEIGHT,1280,720,NULL,NULL);
	DrawAreaPicture(IM_LoadBar,45,636+TITLEHEIGHT,1190,43,0,0,1190*log(1+LoadPage/100.0),43,NULL,NULL);
	switch(LoadText){
		case 1:
			PutText("加载中.  ",45,600+TITLEHEIGHT,1235,636+TITLEHEIGHT,162,175,232,30,FT_Title,NULL,NULL);
		break;
		case 2:
			PutText("加载中.. ",45,600+TITLEHEIGHT,1235,636+TITLEHEIGHT,162,175,232,30,FT_Title,NULL,NULL);
		break;
		case 3:
			PutText("加载中...",45,600+TITLEHEIGHT,1235,636+TITLEHEIGHT,162,175,232,30,FT_Title,NULL,NULL);
		break;
		case 4:
			PutText("加载中   ",45,600+TITLEHEIGHT,1235,636+TITLEHEIGHT,162,175,232,30,FT_Title,NULL,NULL);
		break;
	}
}

void OriginalSide(){
	cancelTimer(TM_START_SIDE);
	PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_SwitchSide);
}

/*加载界面End*/
/*-------------------------------------------------------------------------------------------------*/
/*界面切换动画Begin*/

static int MaskPage=0;
static int EndPages=48;
static NEXTFUNC nextfunc=NULL;

extern bool EndFlag;

void StartMasks(NEXTFUNC fun){
	MaskPage=0;
	PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_SwitchMenu);
	nextfunc=fun;
	MOUSE_STATE = MS_NULL;
	KEYBOARD_STATE = KB_NULL;
	startTimer(TM_SHADES,5);
}

void StartWinMasks(NEXTFUNC fun){
	MaskPage=0;
	nextfunc=fun;
	MOUSE_STATE = MS_NULL;
	KEYBOARD_STATE = KB_NULL;
	startTimer(TM_SHADES,1);
}

void EndMasks(){
	cancelTimer(TM_SHADES);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	(*nextfunc)();
	nextfunc=NULL;
}

void AddMaskPages(){
	MaskPage++;
	if(MaskPage>=EndPages){
		MaskPage=0;
		EndMasks();
	}
}

void DrawBlkMasks(){
	DrawAlphaPicture(IM_Masks_0,0,TITLEHEIGHT,1280,720,NULL,NULL,16);
}

void StartEndMasks(NEXTFUNC fun){
	MaskPage=0;
	nextfunc=fun;
	MOUSE_STATE = MS_NULL;
	KEYBOARD_STATE = KB_NULL;
	EndFlag = true;
	EndPages = 70;
	startTimer(TM_END_SHADES,40);
}

/*界面切换动画End*/
/*-------------------------------------------------------------------------------------------------*/
/*选择模式菜单Begin*/

/*客户区控件*/ 
/*GameMode*/
extern int BT_GM_back,BT_GM_mode1,BT_GM_mode2,BT_GM_mode3,BT_GM_mode4,BT_GM_mode5; 
static int PicAlpha1=225,PicAlpha2=225,PicAlpha3=255;
static int f_gm_back=0,f_gm_mode1=0,f_gm_mode2=0,f_gm_mode3=0,f_gm_mode4=0,f_gm_mode5=0;

void DrawGmodeBtn(){
	//DrawBlurPicture(IM_Choice,0,0+TITLEHEIGHT,1280,720,NULL,NULL,3);
	DrawPicture(IM_Choice,0,0+TITLEHEIGHT,1280,720,NULL,NULL);
	PutText("选择模式",90,50+TITLEHEIGHT,330,110+TITLEHEIGHT,255,255,255,80,FT_Title,NULL,NULL);
	switch(BT_GM_mode1){
		case 0:
			if(f_gm_mode1)
				f_gm_mode1 = !f_gm_mode1;
			DrawAlphaPicture(IM_GM_Pool,120,160+TITLEHEIGHT,290,350,NULL,NULL,PicAlpha1);
			DrawAlphaPicture(IM_GM_BT_0_0,120,510+TITLEHEIGHT,290,57,NULL,NULL,255);
			PutText("美式台球双人对战",120,510+TITLEHEIGHT,410,567+TITLEHEIGHT,0,0,0,40,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_gm_mode1){
				f_gm_mode1 = !f_gm_mode1;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_GM_Pool_Blur,120-5,160-5+TITLEHEIGHT,290+10,350+10,NULL,NULL,PicAlpha2);
			DrawAlphaPicture(IM_GM_BT_0_0,120-5,510+5+TITLEHEIGHT,290+10,57+5,NULL,NULL,255);
			PutText("美式台球双人对战",120-5,510+5+TITLEHEIGHT,410+5,567+10+TITLEHEIGHT,0,0,0,43,FT_Title,NULL,NULL);
			PutText("点击开始美式台球双人对战模式",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_GM_Pool_Blur,120-8,160-8+TITLEHEIGHT,290+16,350+16,NULL,NULL,PicAlpha3);
			DrawAlphaPicture(IM_GM_BT_0_0,120-8,510+8+TITLEHEIGHT,290+16,57+8,NULL,NULL,255);
			PutText("美式台球双人对战",120-8,510+8+TITLEHEIGHT,410+8,567+16+TITLEHEIGHT,0,0,0,45,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_GM_mode2){
		case 0:
			if(f_gm_mode2)
				f_gm_mode2 = !f_gm_mode2;
			DrawAlphaPicture(IM_GM_Practice,495,160+TITLEHEIGHT,290,130,NULL,NULL,PicAlpha1);
			DrawAlphaPicture(IM_GM_BT_0_0,495,290+TITLEHEIGHT,290,57,NULL,NULL,255);
			PutText("自由练习",495,290+TITLEHEIGHT,785,347+TITLEHEIGHT,0,0,0,40,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_gm_mode2){
				f_gm_mode2 = !f_gm_mode2;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_GM_Practice_Blur,495-5,160-5+TITLEHEIGHT,290+10,130+10,NULL,NULL,PicAlpha2);
			DrawAlphaPicture(IM_GM_BT_0_0,495-5,290+5+TITLEHEIGHT,290+10,57+5,NULL,NULL,255);
			PutText("自由练习",495-5,290+5+TITLEHEIGHT,785+5,347+10+TITLEHEIGHT,0,0,0,43,FT_Title,NULL,NULL);
			PutText("点击开始自由练习模式",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_GM_Practice_Blur,495-8,160-8+TITLEHEIGHT,290+16,130+16,NULL,NULL,PicAlpha3);
			DrawAlphaPicture(IM_GM_BT_0_0,495-8,290+8+TITLEHEIGHT,290+16,57+8,NULL,NULL,255);
			PutText("自由练习",495-8,290+8+TITLEHEIGHT,785+8,347+16+TITLEHEIGHT,0,0,0,45,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_GM_mode3){
		case 0:
			if(f_gm_mode3)
				f_gm_mode3 = !f_gm_mode3;
			DrawAlphaPicture(IM_GM_AI,870,160+TITLEHEIGHT,290,130,NULL,NULL,PicAlpha1);
			DrawAlphaPicture(IM_GM_BT_0_0,870,290+TITLEHEIGHT,290,57,NULL,NULL,255);
			PutText("经典台球人机对战",870,290+TITLEHEIGHT,1160,347+TITLEHEIGHT,0,0,0,40,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_gm_mode3){
				f_gm_mode3 = !f_gm_mode3;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_GM_AI_Blur,870-5,160-5+TITLEHEIGHT,290+10,130+10,NULL,NULL,PicAlpha2);
			DrawAlphaPicture(IM_GM_BT_0_0,870-5,290+5+TITLEHEIGHT,290+10,57+5,NULL,NULL,255);
			PutText("经典台球人机对战",870-5,290+5+TITLEHEIGHT,1160+5,347+10+TITLEHEIGHT,0,0,0,43,FT_Title,NULL,NULL);
			PutText("点击开始经典台球人机对战模式",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_GM_AI_Blur,870-8,160-8+TITLEHEIGHT,290+16,130+16,NULL,NULL,PicAlpha3);
			DrawAlphaPicture(IM_GM_BT_0_0,870-8,290+8+TITLEHEIGHT,290+16,57+8,NULL,NULL,255);
			PutText("经典台球人机对战",870-8,290+8+TITLEHEIGHT,1160+8,347+16+TITLEHEIGHT,0,0,0,45,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_GM_mode4){
		case 0:
			if(f_gm_mode4)
				f_gm_mode4 = !f_gm_mode4;
			DrawAlphaPicture(IM_GM_Classical,495,380+TITLEHEIGHT,290,130,NULL,NULL,PicAlpha1);
			DrawAlphaPicture(IM_GM_BT_0_0,495,510+TITLEHEIGHT,290,57,NULL,NULL,255);
			PutText("经典台球双人对战",495,510+TITLEHEIGHT,785,567+TITLEHEIGHT,0,0,0,40,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_gm_mode4){
				f_gm_mode4 = !f_gm_mode4;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_GM_Classical_Blur,495-5,380-5+TITLEHEIGHT,290+10,130+10,NULL,NULL,PicAlpha2);
			DrawAlphaPicture(IM_GM_BT_0_0,495-5,510+5+TITLEHEIGHT,290+10,57+5,NULL,NULL,255);
			PutText("经典台球双人对战",495-5,510+5+TITLEHEIGHT,785+5,567+10+TITLEHEIGHT,0,0,0,43,FT_Title,NULL,NULL);
			PutText("点击开始经典台球双人对战模式",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_GM_Classical_Blur,495-8,380-8+TITLEHEIGHT,290+16,130+16,NULL,NULL,PicAlpha3);
			DrawAlphaPicture(IM_GM_BT_0_0,495-8,510+8+TITLEHEIGHT,290+16,57+8,NULL,NULL,255);
			PutText("经典台球双人对战",495-8,510+8+TITLEHEIGHT,785+8,567+16+TITLEHEIGHT,0,0,0,45,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_GM_mode5){
		case 0:
			if(f_gm_mode5)
				f_gm_mode5 = !f_gm_mode5;
			DrawAlphaPicture(IM_GM_Snooker,870,380+TITLEHEIGHT,290,130,NULL,NULL,PicAlpha1);
			DrawAlphaPicture(IM_GM_BT_0_0,870,510+TITLEHEIGHT,290,57,NULL,NULL,255);
			PutText("斯诺克双人对战",870,510+TITLEHEIGHT,1160,567+TITLEHEIGHT,0,0,0,40,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_gm_mode5){
				f_gm_mode5 = !f_gm_mode5;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_GM_Snooker_Blur,870-5,380-5+TITLEHEIGHT,290+10,130+10,NULL,NULL,PicAlpha2);
			DrawAlphaPicture(IM_GM_BT_0_0,870-5,510+5+TITLEHEIGHT,290+10,57+5,NULL,NULL,255);
			PutText("斯诺克双人对战",870-5,510+5+TITLEHEIGHT,1160+5,567+10+TITLEHEIGHT,0,0,0,43,FT_Title,NULL,NULL);
			PutText("点击开始斯诺克双人对战模式",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_GM_Snooker_Blur,870-8,380-8+TITLEHEIGHT,290+16,130+16,NULL,NULL,PicAlpha3);
			DrawAlphaPicture(IM_GM_BT_0_0,870-8,510+8+TITLEHEIGHT,290+16,57+8,NULL,NULL,255);
			PutText("斯诺克双人对战",870-8,510+8+TITLEHEIGHT,1160+8,567+16+TITLEHEIGHT,0,0,0,45,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_GM_back){
		case 0:
			if(f_gm_back)
				f_gm_back = !f_gm_back;
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,192);
			PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_gm_back){
				f_gm_back = !f_gm_back;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,225);
			PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			PutText("点击返回主界面",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,255);
			PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
	}
}

void StartGmodeMenu(){
	startTimer(TM_CHOOSE_GAMEMODE,1);
	MOUSE_STATE=MS_CHOOSE_GAMEMODE;
	KEYBOARD_STATE=KB_CHOOSE_GAMEMODE;
}

void EndGmodeMenu(){
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	cancelTimer(TM_CHOOSE_GAMEMODE);
}

/*选择模式菜单End*/
/*-------------------------------------------------------------------------------------------------*/
/*选择存档菜单Begin*/

int savepage = 1;
int savemode = LS_LOAD; 

/*客户区控件*/ 
/*LoadSave*/
extern int BT_LS_back,BT_LS_save1,BT_LS_save2,BT_LS_save3,BT_LS_save4,
		   BT_LS_page1,BT_LS_page2,BT_LS_page3,BT_LS_page4,BT_LS_page5,
		   BT_LS_sav1,BT_LS_del1,BT_LS_sav2,BT_LS_del2,BT_LS_sav3,BT_LS_del3,BT_LS_sav4,BT_LS_del4;

static int LS_BtAlpha=32;
static int f_ls_back=0,f_ls_save1=0,f_ls_save2=0,f_ls_save3=0,f_ls_save4=0,
		   f_ls_page1=0,f_ls_page2=0,f_ls_page3=0,f_ls_page4=0,f_ls_page5=0,
		   f_ls_sav1=0,f_ls_del1=0,f_ls_sav2=0,f_ls_del2=0,f_ls_sav3=0,f_ls_del3=0,f_ls_sav4=0,f_ls_del4=0;

static char LS_tmp_text[20];

/*存档标志数据结构体*/
SAVE LS_Save_Data[4]={0};

void UpdtLdSavMsg(){
	LoadSave(4*(savepage-1) + 1,&LS_Save_Data[0]);
	LoadSave(4*(savepage-1) + 2,&LS_Save_Data[1]);
	LoadSave(4*(savepage-1) + 3,&LS_Save_Data[2]);
	LoadSave(4*(savepage-1) + 4,&LS_Save_Data[3]);
}

void DrawLdSavBtn(){
	switch(savepage){
		case 1:
			DrawPicture(IM_LoadSave_1,0,0+TITLEHEIGHT,1280,720,NULL,NULL);
		break;
		case 2:
			DrawPicture(IM_LoadSave_2,0,0+TITLEHEIGHT,1280,720,NULL,NULL);
		break;
		case 3:
			DrawPicture(IM_LoadSave_3,0,0+TITLEHEIGHT,1280,720,NULL,NULL);
		break;
		case 4:
			DrawPicture(IM_LoadSave_4,0,0+TITLEHEIGHT,1280,720,NULL,NULL);
		break;
		case 5:
			DrawPicture(IM_LoadSave_5,0,0+TITLEHEIGHT,1280,720,NULL,NULL);
		break;
	}
	PutText("选择存档",90,50+TITLEHEIGHT,330,110+TITLEHEIGHT,255,255,255,80,FT_Title,NULL,NULL);
	sprintf(LS_tmp_text,"No.%02d",4*(savepage-1) + 1);
	PutTextB(LS_tmp_text,398,173+TITLEHEIGHT,583,203+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
	sprintf(LS_tmp_text,"No.%02d",4*(savepage-1) + 2);
	PutTextB(LS_tmp_text,957,173+TITLEHEIGHT,1142,203+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
	sprintf(LS_tmp_text,"No.%02d",4*(savepage-1) + 3);
	PutTextB(LS_tmp_text,398,412+TITLEHEIGHT,583,442+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
	sprintf(LS_tmp_text,"No.%02d",4*(savepage-1) + 4);
	PutTextB(LS_tmp_text,957,412+TITLEHEIGHT,1142,442+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
	/*存档槽*/ 
	switch(BT_LS_del1){
		case 0:
			if(f_ls_del1)
				f_ls_del1 = !f_ls_del1;
			Draw32BitPicture(IM_Save_Btn0,392,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,407,288+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",410,284+TITLEHEIGHT,482,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_ls_del1){
				f_ls_del1 = !f_ls_del1;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Save_Btn1,392,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,407,288+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",410,284+TITLEHEIGHT,482,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
			PutText("点击删除存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_Save_Btn2,392,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,407,288+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",410,284+TITLEHEIGHT,482,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_LS_sav1){
		case 0:
			if(f_ls_sav1)
				f_ls_sav1 = !f_ls_sav1;
			Draw32BitPicture(IM_Save_Btn0,489,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,504,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",507,284+TITLEHEIGHT,579,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,504,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",507,284+TITLEHEIGHT,579,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[0].flag){
				DrawPicture(LS_Save_Data[0].IM_Save_Cover,137,173+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[0].ModeName,398,203+TITLEHEIGHT,583,243+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[0].Time,398,243+TITLEHEIGHT,583,276+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
		case 1:
			if(!f_ls_sav1){
				f_ls_sav1 = !f_ls_sav1;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Save_Btn1,489,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,504,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",507,284+TITLEHEIGHT,579,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
					PutText("点击保存存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,504,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",507,284+TITLEHEIGHT,579,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
					PutText("点击加载存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[0].flag){
				DrawPicture(LS_Save_Data[0].IM_Save_Blur,137,173+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[0].ModeName,398,203+TITLEHEIGHT,583,243+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[0].Time,398,243+TITLEHEIGHT,583,276+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
		case 2:
			Draw32BitPicture(IM_Save_Btn2,489,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,504,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",507,284+TITLEHEIGHT,579,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,504,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",507,284+TITLEHEIGHT,579,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[0].flag){
				DrawPicture(LS_Save_Data[0].IM_Save_Blur,137,173+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[0].ModeName,398,203+TITLEHEIGHT,583,243+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[0].Time,398,243+TITLEHEIGHT,583,276+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
	}
	switch(BT_LS_del2){
		case 0:
			if(f_ls_del2)
				f_ls_del2 = !f_ls_del2;
			Draw32BitPicture(IM_Save_Btn0,951,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,966,288+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",969,284+TITLEHEIGHT,1041,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_ls_del2){
				f_ls_del2 = !f_ls_del2;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Save_Btn1,951,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,966,288+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",969,284+TITLEHEIGHT,1041,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
			PutText("点击删除存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_Save_Btn2,951,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,966,288+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",969,284+TITLEHEIGHT,1041,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_LS_sav2){
		case 0:
			if(f_ls_sav2)
				f_ls_sav2 = !f_ls_sav2;
			Draw32BitPicture(IM_Save_Btn0,1048,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,1063,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",1066,284+TITLEHEIGHT,1138,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,1063,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",1066,284+TITLEHEIGHT,1138,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[1].flag){
				DrawPicture(LS_Save_Data[1].IM_Save_Cover,696,173+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[1].ModeName,957,203+TITLEHEIGHT,1142,243+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[1].Time,957,243+TITLEHEIGHT,1142,276+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
		case 1:
			if(!f_ls_sav2){
				f_ls_sav2 = !f_ls_sav2;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Save_Btn1,1048,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,1063,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",1066,284+TITLEHEIGHT,1138,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
					PutText("点击保存存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,1063,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",1066,284+TITLEHEIGHT,1138,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
					PutText("点击加载存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[1].flag){
				DrawPicture(LS_Save_Data[1].IM_Save_Blur,696,173+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[1].ModeName,957,203+TITLEHEIGHT,1142,243+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[1].Time,957,243+TITLEHEIGHT,1142,276+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
		case 2:
			Draw32BitPicture(IM_Save_Btn2,1048,284+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,1063,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",1066,284+TITLEHEIGHT,1138,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,1063,288+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",1066,284+TITLEHEIGHT,1138,309+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[1].flag){
				DrawPicture(LS_Save_Data[1].IM_Save_Blur,696,173+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[1].ModeName,957,203+TITLEHEIGHT,1142,243+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[1].Time,957,243+TITLEHEIGHT,1142,276+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
	}
	switch(BT_LS_del3){
		case 0:
			if(f_ls_del3)
				f_ls_del3 = !f_ls_del3;
			Draw32BitPicture(IM_Save_Btn0,392,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,407,527+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",410,523+TITLEHEIGHT,482,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_ls_del3){
				f_ls_del3 = !f_ls_del3;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Save_Btn1,392,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,407,527+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",410,523+TITLEHEIGHT,482,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
			PutText("点击删除存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_Save_Btn0,392,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,407,527+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",410,523+TITLEHEIGHT,482,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_LS_sav3){
		case 0:
			if(f_ls_sav3)
				f_ls_sav3 = !f_ls_sav3;
			Draw32BitPicture(IM_Save_Btn0,489,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,504,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",507,523+TITLEHEIGHT,579,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,504,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",507,523+TITLEHEIGHT,579,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[2].flag){
				DrawPicture(LS_Save_Data[2].IM_Save_Cover,138,412+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[2].ModeName,398,442+TITLEHEIGHT,583,482+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[2].Time,398,482+TITLEHEIGHT,583,515+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
		case 1:
			if(!f_ls_sav3){
				f_ls_sav3 = !f_ls_sav3;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Save_Btn1,489,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,504,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",507,523+TITLEHEIGHT,579,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
					PutText("点击保存存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,504,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",507,523+TITLEHEIGHT,579,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
					PutText("点击加载存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[2].flag){
				DrawPicture(LS_Save_Data[2].IM_Save_Blur,138,412+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[2].ModeName,398,442+TITLEHEIGHT,583,482+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[2].Time,398,482+TITLEHEIGHT,583,515+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
		case 2:
			Draw32BitPicture(IM_Save_Btn2,489,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,504,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",507,523+TITLEHEIGHT,579,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,504,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",507,523+TITLEHEIGHT,579,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[2].flag){
				DrawPicture(LS_Save_Data[2].IM_Save_Blur,138,412+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[2].ModeName,398,442+TITLEHEIGHT,583,482+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[2].Time,398,482+TITLEHEIGHT,583,515+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
	}
	switch(BT_LS_del4){
		case 0:
			if(f_ls_del4)
				f_ls_del4 = !f_ls_del4;
			Draw32BitPicture(IM_Save_Btn0,951,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,966,527+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",969,523+TITLEHEIGHT,1041,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_ls_del4){
				f_ls_del4 = !f_ls_del4;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Save_Btn1,951,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,966,527+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",969,523+TITLEHEIGHT,1041,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
			PutText("点击删除存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_Save_Btn2,951,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			DrawKeyColorPicture(IM_Save_Del,966,527+TITLEHEIGHT,16,16,NULL,NULL);
			PutText("删除",969,523+TITLEHEIGHT,1041,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_LS_sav4){
		case 0:
			if(f_ls_sav4)
				f_ls_sav4 = !f_ls_sav4;
			Draw32BitPicture(IM_Save_Btn0,1048,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,1063,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",1066,523+TITLEHEIGHT,1138,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,1063,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",1066,523+TITLEHEIGHT,1138,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[3].flag){
				DrawPicture(LS_Save_Data[3].IM_Save_Cover,696,412+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[3].ModeName,957,442+TITLEHEIGHT,1142,482+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[3].Time,957,482+TITLEHEIGHT,1142,515+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
		case 1:
			if(!f_ls_sav4){
				f_ls_sav4 = !f_ls_sav4;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Save_Btn1,1048,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,1063,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",1066,523+TITLEHEIGHT,1138,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
					PutText("点击保存存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,1063,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",1066,523+TITLEHEIGHT,1138,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
					PutText("点击加载存档",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[3].flag){
				DrawPicture(LS_Save_Data[3].IM_Save_Blur,696,412+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[3].ModeName,957,442+TITLEHEIGHT,1142,482+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[3].Time,957,482+TITLEHEIGHT,1142,515+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
		case 2:
			Draw32BitPicture(IM_Save_Btn2,1048,523+TITLEHEIGHT,90,25,NULL,NULL,0xFF);
			switch(savemode){
				case LS_SAVE:
					DrawKeyColorPicture(IM_Save_Save,1063,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("保存",1066,523+TITLEHEIGHT,1138,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					DrawKeyColorPicture(IM_Save_Load,1063,527+TITLEHEIGHT,16,16,NULL,NULL);
					PutText("加载",1066,523+TITLEHEIGHT,1138,548+TITLEHEIGHT,255,255,255,20,FT_Title,NULL,NULL);
				break;
			}
			if(LS_Save_Data[3].flag){
				DrawPicture(LS_Save_Data[3].IM_Save_Blur,696,412+TITLEHEIGHT,245,136,NULL,NULL);
				PutTextB(LS_Save_Data[3].ModeName,957,442+TITLEHEIGHT,1142,482+TITLEHEIGHT,36,63,73,28,FT_Title,NULL,NULL);
				PutTextB(LS_Save_Data[3].Time,957,482+TITLEHEIGHT,1142,515+TITLEHEIGHT,36,63,73,25,FT_Title,NULL,NULL);
			}
		break;
	}
	/*页面切换*/
	switch(BT_LS_page1){
		case 0:
			if(f_ls_page1)
				f_ls_page1 = !f_ls_page1;
		break;
		case 1:
			if(!f_ls_page1){
				f_ls_page1 = !f_ls_page1;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_SavePage_Btn,860,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
			PutText("转到存档第一页",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_SavePage_Btn,860,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
		break;
	}
	switch(BT_LS_page2){
		case 0:
			if(f_ls_page2)
				f_ls_page2 = !f_ls_page2;
		break;
		case 1:
			if(!f_ls_page2){
				f_ls_page2 = !f_ls_page2;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_SavePage_Btn,920,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
			PutText("转到存档第二页",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_SavePage_Btn,920,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
		break;
	}
	switch(BT_LS_page3){
		case 0:
			if(f_ls_page3)
				f_ls_page3 = !f_ls_page3;
		break;
		case 1:
			if(!f_ls_page3){
				f_ls_page3 = !f_ls_page3;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_SavePage_Btn,980,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
			PutText("转到存档第三页",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_SavePage_Btn,980,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
		break;
	}
	switch(BT_LS_page4){
		case 0:
			if(f_ls_page4)
				f_ls_page4 = !f_ls_page4;
		break;
		case 1:
			if(!f_ls_page4){
				f_ls_page4 = !f_ls_page4;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_SavePage_Btn,1040,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
			PutText("转到存档第四页",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_SavePage_Btn,1040,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
		break;
	}
	switch(BT_LS_page5){
		case 0:
			if(f_ls_page5)
				f_ls_page5 = !f_ls_page5;
		break;
		case 1:
			if(!f_ls_page5){
				f_ls_page5 = !f_ls_page5;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_SavePage_Btn,1100,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
			PutText("转到存档第五页",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawAlphaPicture(IM_SavePage_Btn,1100,62+TITLEHEIGHT,44,44,NULL,NULL,LS_BtAlpha);
		break;
	}
	switch(BT_LS_back){
		case 0:
			if(f_ls_back)
				f_ls_back = !f_ls_back;
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,192);
			switch(savemode){
				case LS_SAVE:
					PutText("返回暂停界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
				break;
			}
		break;
		case 1:
			if(!f_ls_back){
				f_ls_back = !f_ls_back;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,225);
			switch(savemode){
				case LS_SAVE:
					PutText("返回暂停界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
					PutText("点击返回暂停界面",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
					PutText("点击返回主界面",365,625+TITLEHEIGHT,1180,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
				break;
			}
		break;
		case 2:
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,255);
			switch(savemode){
				case LS_SAVE:
					PutText("返回暂停界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
				break;
				case LS_LOAD:
					PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
				break;
			}
		break;
	}
}

void StartLdSavMenu(){
	savepage = 1;
	UpdtLdSavMsg();
	startTimer(TM_LOAD_SAVE,1);
	MOUSE_STATE=MS_LOAD_SAVE;
	KEYBOARD_STATE=KB_LOAD_SAVE;
}

void EndLdSavMenu(){
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	cancelTimer(TM_LOAD_SAVE);
}

/*选择存档菜单End*/
/*-------------------------------------------------------------------------------------------------*/
/*玩家注册Begin*/

/*Register*/
bool f_register = false;
bool f_register_1 = false;
bool f_register_2 = false;

/*玩家信息*/
static USER tempUser;
extern LPUSER User1, User2;
static LPIMAGE tempImage;
extern int S_Player1_ID;
extern int S_Player2_ID;

/*文本框资源*/
LPTEXT TX_REG_text;
static char REG_title[20];

/*客户区控件*/ 
extern int BT_REG_avatar,BT_REG_text,BT_REG_ok,BT_REG_back; 
static int f_reg_avatar=0,f_reg_text=0,f_reg_ok=0,f_reg_back=0;

extern int ShowCourse;

static void ReDrawRegstMenu(){
	DrawRegstBtn();
}

void NewAvarar(char* file){
	LoadPicture(file,&tempImage,NULL);
	MakeSizePicture(&tempUser.Avatar,tempImage,500,500,NULL);
	DrawRegstBtn();
}

void DrawRegstBtn(){
	DrawPicture(IM_Register,0,0+TITLEHEIGHT,1280,720,NULL,NULL);
	PutTextB(REG_title,90,50+TITLEHEIGHT,600,110+TITLEHEIGHT,255,255,255,80,FT_Title,NULL,NULL);
	switch(BT_REG_avatar){
		case 0:
			if(f_reg_avatar)
				f_reg_avatar = !f_reg_avatar;
			DrawAlphaPicture(tempUser.Avatar,527,153+TITLEHEIGHT,225,225,NULL,NULL,225);
		break;
		case 1:
			if(!f_reg_avatar){
				f_reg_avatar = !f_reg_avatar;
				PlayOnBtMusic();
			}
			DrawBlurPicture(tempUser.Avatar,527,153+TITLEHEIGHT,225,225,NULL,NULL,3);
			PutText("点击修改头像",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawBlurPicture(tempUser.Avatar,527,153+TITLEHEIGHT,225,225,NULL,NULL,3);
		break;
	}
	switch(BT_REG_text){
		case 0:
			if(f_reg_text)
				f_reg_text = !f_reg_text;
			Draw32BitPicture(IM_TextBar,415,460+TITLEHEIGHT,450,60,NULL,NULL,225);
		break;
		case 1:
			if(!f_reg_text){
				f_reg_text = !f_reg_text;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_TextBar,415,460+TITLEHEIGHT,450,60,NULL,NULL,250);
			PutText("点击修改昵称",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_TextBar,415,460+TITLEHEIGHT,450,60,NULL,NULL,255);
		break;
	}
	switch(BT_REG_ok){
		case 0:
			if(f_reg_ok)
				f_reg_ok = !f_reg_ok;
			Draw32BitPicture(IM_Reg_OK,980,625+TITLEHEIGHT,205,45,NULL,NULL,192);
			PutText("完成注册",980,625+TITLEHEIGHT,1185,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_reg_ok){
				f_reg_ok = !f_reg_ok;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Reg_OK,980,625+TITLEHEIGHT,205,45,NULL,NULL,225);
			PutText("完成注册",980,625+TITLEHEIGHT,1185,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			PutText("点击完成注册",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_Reg_OK,980,625+TITLEHEIGHT,205,45,NULL,NULL,255);
			PutText("完成注册",980,625+TITLEHEIGHT,1185,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
	}
	if(f_register){
		switch(BT_REG_back){
			case 0:
				if(f_reg_back)
					f_reg_back = !f_reg_back;
				Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,192);
				PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			break;
			case 1:
				if(!f_reg_back){
					f_reg_back = !f_reg_back;
					PlayOnBtMusic();
				}
				Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,225);
				PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
				PutText("点击返回主界面",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
			break;
			case 2:
				Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,255);
				PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			break;
		}
	}
	DrawTextsBox();
}

void ActiveRegText(){
	ActiveTextBox(TX_REG_text);
}

void FinishRegist(){
	strcpy(tempUser.Name,TX_REG_text->text);
	MakeSizePicture(&tempUser.BackGround,IM_Choice,1280,720,NULL);
	if(!f_register_1){
		f_register_1 = true;
		CreateUsers(&tempUser,&User1);
		S_Player1_ID = User1->Uid;
		ResetSets();
	}
	else if(!f_register_2){
		f_register_2 = true;
		CreateUsers(&tempUser,&User2);
		S_Player2_ID = User2->Uid;	
		ResetSets();
	}
}

void StartRegstMenu(){
	if(!f_register && f_register_1 && f_register_2){
		f_register = true;
		StartStartMenu();
		if(ShowCourse){
			/*说明文档*/
			SHELLEXECUTEINFO ShellInfo;
			memset(&ShellInfo, 0, sizeof(ShellInfo));
			ShellInfo.cbSize = sizeof(ShellInfo);
			ShellInfo.hwnd = NULL;
			ShellInfo.lpVerb = "open";
			ShellInfo.lpFile = "readme.txt";
			ShellInfo.nShow = SW_SHOWMAXIMIZED;
			ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShellExecuteEx(&ShellInfo);
			/*教程*/
			ShowCourseWindow();
			ShowCourse=!ShowCourse;
			ResetSets();
		}
		return;
	}
	else if(!f_register_1){
		strcpy(REG_title,"注册玩家一账号");
	}
	else if(!f_register_2){
		strcpy(REG_title,"注册玩家二账号");
	}
	else{
		strcpy(REG_title,"注册新用户");
	}
	tempUser.Avatar=IM_Player0;
	InitTextBox(&TX_REG_text,427,465+TITLEHEIGHT,420,50,12,0,0,0,FT_Title,50,NULL,NULL);
	RegistTextBox(&TX_REG_text);
	InitSetText(TX_REG_text,"点击修改昵称");
	RegistRedrawFunc(ReDrawRegstMenu);
	startTimer(TM_REGISTER,1);
	MOUSE_STATE=MS_REGISTER;
	KEYBOARD_STATE=KB_REGISTER;
}

void EndRegstMenu(){
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	CancelTextBox(); 
	cancelTimer(TM_REGISTER);
	if(f_register){
		/*暂不支持*/
		printf("返回\n");
	}else{
		StartMasks(StartRegstMenu);
	}
}

/*玩家注册End*/
/*-------------------------------------------------------------------------------------------------*/
/*用户界面Begin*/

/*对应玩家信息*/
LPUSER WhichUser = NULL; 

/*玩家信息*/
extern int S_Player1_ID;
extern int S_Player2_ID;

/*文本框资源*/
bool editing = false;
LPTEXT TX_USER_text;
static char user_temptext[20];

/*客户区控件*/ 
extern int BT_USER_avatar,BT_USER_text,BT_USER_background,BT_USER_back,BT_USER_def; 
static int f_user_avatar=0,f_user_text=0,f_user_background=0,f_user_back=0,f_user_def=0;

static void ReDrawUsersMenu(){
	DrawUsersBtn();
}

static LPIMAGE user_tempImage;

void NewUserAvarar(char* file){
	LoadPicture(file,&user_tempImage,NULL);
	FreePicture(&WhichUser->Avatar);
	MakeSizePicture(&WhichUser->Avatar,user_tempImage,500,500,NULL);
	FreePicture(&user_tempImage);
	SaveUsers(WhichUser);
	DrawUsersBtn();
}

void NewUserBackground(char* file){
	LoadPicture(file,&user_tempImage,NULL);
	FreePicture(&WhichUser->BackGround);
	MakeSizePicture(&WhichUser->BackGround,user_tempImage,1280,720,NULL);
	FreePicture(&user_tempImage);
	MakeBlurPicture(&user_tempImage,WhichUser->BackGround,NULL,5);
	FreePicture(&WhichUser->BackGround);
	MakeSizePicture(&WhichUser->BackGround,user_tempImage,1280,720,NULL);
	FreePicture(&user_tempImage);
	SaveUsers(WhichUser);
	DrawUsersBtn();
}

void DrawUsersBtn(){	
	DrawPicture(WhichUser->BackGround,0,0+TITLEHEIGHT,1280,720,NULL,NULL);
	DrawAlphaPicture(IM_AvatarBack,133,58+TITLEHEIGHT,179,179,NULL,NULL,225);
	Draw32BitPicture(IM_ScoreBoard,135,295+TITLEHEIGHT,310,135,NULL,NULL,96);
	Draw32BitPicture(IM_ScoreBoard,135,450+TITLEHEIGHT,310,135,NULL,NULL,96);
	Draw32BitPicture(IM_ScoreBoard,485,295+TITLEHEIGHT,310,135,NULL,NULL,96);
	Draw32BitPicture(IM_ScoreBoard,485,450+TITLEHEIGHT,310,135,NULL,NULL,96);
	Draw32BitPicture(IM_ScoreBoard,835,295+TITLEHEIGHT,310,135,NULL,NULL,96);
	Draw32BitPicture(IM_ScoreBoard,835,450+TITLEHEIGHT,310,135,NULL,NULL,96);
	DrawKeyColorPicture(IM_Eagle,909,55+TITLEHEIGHT,236,202,NULL,NULL);
	DrawAlphaPicture(IM_ExpBar_0,370,192+TITLEHEIGHT,550,18,NULL,NULL,200);
	if(WhichUser->Exp) DrawAreaPicture(IM_ExpBar_1,370,192+TITLEHEIGHT,550,18,0,0,(int)550*WhichUser->Exp/1000.0,18,NULL,NULL);
	PutText("游戏时长",135,315+TITLEHEIGHT,445,350+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
	PutText("总游戏场数",485,315+TITLEHEIGHT,795,350+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
	PutText("总胜率",835,315+TITLEHEIGHT,1145,350+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
	PutText("经典模式双人对战胜场",135,475+TITLEHEIGHT,445,505+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
	PutText("美式台球双人对战胜场",485,475+TITLEHEIGHT,795,505+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
	PutText("斯诺克双人对战胜场",835,475+TITLEHEIGHT,1145,505+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
	sprintf(user_temptext,"%d点 / 1000点 ",WhichUser->Exp);
	PutTextC(user_temptext,370,210+TITLEHEIGHT,920,250+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
	sprintf(user_temptext,"%d小时 %d分钟",(WhichUser->PlayTime - (WhichUser->PlayTime % 60)
											  				   - (WhichUser->PlayTime - WhichUser->PlayTime % 60) / 60 % 60 * 60) / 3600,
										  (WhichUser->PlayTime - WhichUser->PlayTime % 60) / 60 % 60);
	PutText(user_temptext,135,345+TITLEHEIGHT,445,430+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
	sprintf(user_temptext,"%d 场",WhichUser->Total);
	PutText(user_temptext,485,345+TITLEHEIGHT,795,430+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
	sprintf(user_temptext,"%2.1f %%",WhichUser->Total?(WhichUser->PoolWin
													 + WhichUser->SnookerWin
													 + WhichUser->ClassicMultiWin)*100.0/WhichUser->Total:0);
	PutText(user_temptext,835,345+TITLEHEIGHT,1145,430+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
	sprintf(user_temptext,"%d 场",WhichUser->ClassicMultiWin);
	PutText(user_temptext,135,500+TITLEHEIGHT,445,585+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
	sprintf(user_temptext,"%d 场",WhichUser->PoolWin);
	PutText(user_temptext,485,500+TITLEHEIGHT,795,585+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
	sprintf(user_temptext,"%d 场",WhichUser->SnookerWin);
	PutText(user_temptext,835,500+TITLEHEIGHT,1145,585+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
	sprintf(user_temptext,"%d",WhichUser->Level);
	PutText(user_temptext,909,110+TITLEHEIGHT,1145,170+TITLEHEIGHT,254,244,29,65,FT_Num,NULL,NULL);
	switch(BT_USER_avatar){
		case 0:
			if(f_user_avatar)
				f_user_avatar = !f_user_avatar;
			DrawAvatar(WhichUser->Avatar,135,60+TITLEHEIGHT,175,175,NULL,NULL);
		break;
		case 1:
			if(!f_user_avatar){
				f_user_avatar = !f_user_avatar;
				PlayOnBtMusic();
			}
			DrawBlurPicture(WhichUser->Avatar,135,60+TITLEHEIGHT,175,175,NULL,NULL,2);
			PutText("点击修改头像",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			DrawBlurPicture(WhichUser->Avatar,135,60+TITLEHEIGHT,175,175,NULL,NULL,2);
		break;
	}
	switch(BT_USER_text){
		case 0:
			if(f_user_text)
				f_user_text = !f_user_text;
			Draw32BitPicture(IM_UserButton,790,95+TITLEHEIGHT,40,40,NULL,NULL,128);
			DrawKeyColorPicture(IM_Edit,790,95+TITLEHEIGHT,40,40,NULL,NULL);
		break;
		case 1:
			if(!f_user_text){
				f_user_text = !f_user_text;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_UserButton,790,95+TITLEHEIGHT,40,40,NULL,NULL,192);
			DrawKeyColorPicture(IM_Edit,790,95+TITLEHEIGHT,40,40,NULL,NULL);
			PutText("点击修改昵称",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_UserButton,790,95+TITLEHEIGHT,40,40,NULL,NULL,225);
			DrawKeyColorPicture(IM_Edit,790,95+TITLEHEIGHT,40,40,NULL,NULL);
		break;
	}
	switch(BT_USER_background){
		case 0:
			if(f_user_background)
				f_user_background = !f_user_background;
			Draw32BitPicture(IM_UserButton,860,95+TITLEHEIGHT,40,40,NULL,NULL,128);
			DrawKeyColorPicture(IM_Cloth,860,95+TITLEHEIGHT,40,40,NULL,NULL);
		break;
		case 1:
			if(!f_user_background){
				f_user_background = !f_user_background;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_UserButton,860,95+TITLEHEIGHT,40,40,NULL,NULL,192);
			DrawKeyColorPicture(IM_Cloth,860,95+TITLEHEIGHT,40,40,NULL,NULL);
			PutText("点击修改背景",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_UserButton,860,95+TITLEHEIGHT,40,40,NULL,NULL,225);
			DrawKeyColorPicture(IM_Cloth,860,95+TITLEHEIGHT,40,40,NULL,NULL);
		break;
	}
	switch(BT_USER_def){
		case 0:
			if(f_user_def)
				f_user_def = !f_user_def;
			Draw32BitPicture(IM_Individual,980,625+TITLEHEIGHT,205,45,NULL,NULL,192);
			PutText("自定义球杆",980,625+TITLEHEIGHT,1185,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_user_def){
				f_user_def = !f_user_def;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Individual,980,625+TITLEHEIGHT,205,45,NULL,NULL,225);
			PutText("自定义球杆",980,625+TITLEHEIGHT,1185,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			PutText("点击自定义球杆",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_Individual,980,625+TITLEHEIGHT,205,45,NULL,NULL,255);
			PutText("自定义球杆",980,625+TITLEHEIGHT,1185,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_USER_back){
		case 0:
			if(f_user_back)
				f_user_back = !f_user_back;
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,192);
			PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_user_back){
				f_user_back = !f_user_back;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,225);
			PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			PutText("点击返回主界面",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,255);
			PutText("返回主界面",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
	}
	if(InitCheck()){
		DrawTextsBox();
	}
	else{
		if(editing){
			editing = false;
			strcpy(WhichUser->Name,TX_USER_text->text);
			InitSetText(TX_USER_text,"\0");
			SaveUsers(WhichUser);
		}
		PutTextB(WhichUser->Name,370,58+TITLEHEIGHT,920,170+TITLEHEIGHT,255,255,255,65,FT_Text,NULL,NULL);
	}
}

void ActiveUsrText(){
	editing = true;
	InitSetText(TX_USER_text,WhichUser->Name);
	ActiveTextBox(TX_USER_text);
}

void FinishResets(){
	strcpy(tempUser.Name,TX_REG_text->text);
	MakeSizePicture(&tempUser.BackGround,IM_Choice,1280,720,NULL);
	if(!f_register_1){
		f_register_1 = true;
		CreateUsers(&tempUser,&User1);
		S_Player1_ID = User1->Uid;
		ResetSets();
	}
	else if(!f_register_2){
		f_register_2 = true;
		CreateUsers(&tempUser,&User2);
		S_Player2_ID = User2->Uid;	
		ResetSets();
	}
}

void StartUserMenu(){
	InitToolTips(&TT_Client,NULL);
	AddToolTips(TT_Client,(UID)&WhichUser,135,60+TITLEHEIGHT,175,175,"更换头像");
	AddToolTips(TT_Client,(UID)&IM_Edit,790,95+TITLEHEIGHT,40,40,"修改昵称");
	AddToolTips(TT_Client,(UID)&IM_Cloth,860,95+TITLEHEIGHT,40,40,"自定义背景图片");
	InitTextBox(&TX_USER_text,370,81+TITLEHEIGHT,420,65,12,255,255,255,FT_Text,65,NULL,NULL);
	RegistTextBox(&TX_USER_text);
	RegistRedrawFunc(ReDrawUsersMenu);
	startTimer(TM_USER,1);
	MOUSE_STATE=MS_USER;
	KEYBOARD_STATE=KB_USER;
}

void EndUserMenu(){
	DelToolTips(TT_Client,(UID)&WhichUser);
	DelToolTips(TT_Client,(UID)&IM_Edit);
	DelToolTips(TT_Client,(UID)&IM_Cloth);
	DestroyToolTips(&TT_Client);
	CancelTextBox(); 
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	cancelTimer(TM_USER);
}

/*用户界面End*/
/*-------------------------------------------------------------------------------------------------*/
/*暂停游戏菜单Begin*/

/*客户区控件*/ 
/*Pause*/
LPIMAGE IM_Pause_BK;
extern int BT_Pause_Continue,BT_Pause_Save,BT_Pause_Quit; 
static int PausePicAlpha1=64,PausePicAlpha2=96,PausePicAlpha3=128;
static int f_pause_continue=0,f_pause_save=0,f_pause_quit=0;

static LPIMAGE IM_Pause_BK_Alpha,IM_Pause_BK_Blur;
static NEXTFUNC pausefunc=NULL;
static bool CDtime=false;

void EnableCDTime(){
	CDtime=false;
}

void DrawPauseBtn(){
	DrawPicture(IM_Pause_BK_Blur,0,TITLEHEIGHT,1280,720,NULL,NULL);
	PutText("游戏暂停",90,50+TITLEHEIGHT,330,110+TITLEHEIGHT,255,255,255,80,FT_Title,NULL,NULL);
	switch(BT_Pause_Continue){
		case 0:
			if(f_pause_continue)
				f_pause_continue = !f_pause_continue;
			Draw32BitPicture(IM_Pause_0,155,250+TITLEHEIGHT,260,220,NULL,NULL,PausePicAlpha1);
			DrawKeyColorPicture(IM_Pause_Continue,235,275+TITLEHEIGHT,100,100,NULL,NULL);
			PutText("继续游戏",155,350+TITLEHEIGHT,415,470+TITLEHEIGHT,255,255,255,40,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_pause_continue){
				f_pause_continue = !f_pause_continue;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Pause_0,155-5,250-5+TITLEHEIGHT,260+10,220+10,NULL,NULL,PausePicAlpha2);
			DrawKeyColorPicture(IM_Pause_Continue,235-5,275-5+TITLEHEIGHT,100+10,100+10,NULL,NULL);
			PutText("继续游戏",155-5,350+5+TITLEHEIGHT,415+5,470+10+TITLEHEIGHT,255,255,255,43,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_Pause_0,155-8,250-8+TITLEHEIGHT,260+16,220+16,NULL,NULL,PausePicAlpha3);
			DrawKeyColorPicture(IM_Pause_Continue,235-8,275-8+TITLEHEIGHT,100+16,100+16,NULL,NULL);
			PutText("继续游戏",155-8,350+8+TITLEHEIGHT,415+8,470+16+TITLEHEIGHT,255,255,255,45,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_Pause_Save){
		case 0:
			if(f_pause_save)
				f_pause_save = !f_pause_save;
			Draw32BitPicture(IM_Pause_0,510,250+TITLEHEIGHT,260,220,NULL,NULL,PausePicAlpha1);
			DrawKeyColorPicture(IM_Pause_Save,590,275+TITLEHEIGHT,100,100,NULL,NULL);
			PutText("保存游戏",510,350+TITLEHEIGHT,780,470+TITLEHEIGHT,255,255,255,40,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_pause_save){
				f_pause_save = !f_pause_save;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Pause_0,510-5,250-5+TITLEHEIGHT,260+10,220+10,NULL,NULL,PausePicAlpha2);
			DrawKeyColorPicture(IM_Pause_Save,590-5,275-5+TITLEHEIGHT,100+10,100+10,NULL,NULL);
			PutText("保存游戏",510-5,350+5+TITLEHEIGHT,780+5,470+10+TITLEHEIGHT,255,255,255,43,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_Pause_0,510-8,250-8+TITLEHEIGHT,260+16,220+16,NULL,NULL,PausePicAlpha3);
			DrawKeyColorPicture(IM_Pause_Save,590-8,275-8+TITLEHEIGHT,100+16,100+16,NULL,NULL);
			PutText("保存游戏",510-8,350+8+TITLEHEIGHT,780+8,470+16+TITLEHEIGHT,255,255,255,45,FT_Title,NULL,NULL);
		break;
	}
	switch(BT_Pause_Quit){
		case 0:
			if(f_pause_quit)
				f_pause_quit = !f_pause_quit;
			Draw32BitPicture(IM_Pause_0,865,250+TITLEHEIGHT,260,220,NULL,NULL,PausePicAlpha1);
			DrawKeyColorPicture(IM_Pause_Quit,945,275+TITLEHEIGHT,100,100,NULL,NULL);
			PutText("返回标题",865,350+TITLEHEIGHT,1125,470+TITLEHEIGHT,255,255,255,40,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_pause_quit){
				f_pause_quit = !f_pause_quit;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_Pause_0,865-5,250-5+TITLEHEIGHT,260+10,220+10,NULL,NULL,PausePicAlpha2);
			DrawKeyColorPicture(IM_Pause_Quit,945-5,275-5+TITLEHEIGHT,100+10,100+10,NULL,NULL);
			PutText("返回标题",865-5,350+5+TITLEHEIGHT,1125+5,470+10+TITLEHEIGHT,255,255,255,43,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_Pause_0,865-8,250-8+TITLEHEIGHT,260+16,220+16,NULL,NULL,PausePicAlpha3);
			DrawKeyColorPicture(IM_Pause_Quit,945-8,275-8+TITLEHEIGHT,100+16,100+16,NULL,NULL);
			PutText("返回标题",865-8,350+8+TITLEHEIGHT,1125+8,470+16+TITLEHEIGHT,255,255,255,45,FT_Title,NULL,NULL);
		break;
	}
}

void StartPauseMenu(NEXTFUNC fun){
	if(CDtime) return;
	PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_Pause);  
	pausefunc=fun;
	CDtime=true;
	startTimer(TM_PAUSE_MENU,10);
	startTimer(TM_PAUSE_MENU_CD,20);
	if(GameMode != GM_TRAINNING)
		KillTimer(TimeSysWindow,TM_HIT_COUNTDOWN);
	GameTimeSys->countdown_f=false;
	MOUSE_STATE=MS_PAUSE_MENU;
	KEYBOARD_STATE=KB_PAUSE_MENU;
	LoadPictureFromWnd(&IM_Pause_BK,0,TITLEHEIGHT,1280,720,NULL);
	MakeBlurPicture(&IM_Pause_BK_Alpha,IM_Pause_BK,NULL,0);
	MakeAlphaPicture(&IM_Pause_BK_Alpha,IM_Masks_0,NULL,96);
	MakeBlurPicture(&IM_Pause_BK_Blur,IM_Pause_BK_Alpha,NULL,3);
}

void EndPauseMenu(){
	if(CDtime) return;
	DrawPauseBtn();
	cancelTimer(TM_PAUSE_MENU);
	cancelTimer(TM_PAUSE_MENU_CD);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	FreePicture(&IM_Pause_BK);
	FreePicture(&IM_Pause_BK_Blur);
	FreePicture(&IM_Pause_BK_Alpha);
	(*pausefunc)();
	pausefunc=NULL;
	CDtime=false;
	//SetTimer(TimeSysWindow, TM_HIT_COUNTDOWN, 1000, NULL); 
}

void EndPauseMenuWithoutCallBack(){
	if(CDtime) return;
	DrawPauseBtn();
	cancelTimer(TM_PAUSE_MENU);
	cancelTimer(TM_PAUSE_MENU_CD);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	FreePicture(&IM_Pause_BK);
	FreePicture(&IM_Pause_BK_Blur);
	FreePicture(&IM_Pause_BK_Alpha);
	pausefunc=NULL;
	CDtime=false;
	if(GameMode != GM_TRAINNING)
		EndCountDown();
}

void EndPauseMenuState(){
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
}

void StartPauseMenuState(){
	startTimer(TM_PAUSE_MENU,10);
	startTimer(TM_PAUSE_MENU_CD,20);
	MOUSE_STATE=MS_PAUSE_MENU;
	KEYBOARD_STATE=KB_PAUSE_MENU;
}

/*暂停游戏菜单End*/
/*-------------------------------------------------------------------------------------------------*/
/*游戏结束界面Begin*/

/*客户区控件*/ 
/*Win*/
extern int BT_Win_Back,BT_Win_Save; 
static int WinPicAlpha1=128,WinPicAlpha2=148,WinPicAlpha3=192;
static int f_win_back=0,f_win_save=0;

/*胜利信息*/
int Winscore;
LPUSER WinUser;

static char win_temp_text[50];

void DrawWinerBtn(){
	if(WinUser!=NULL)
		DrawPicture(WinUser->BackGround,0,TITLEHEIGHT,1280,720,NULL,NULL);
	else
		DrawPicture(IM_Choice,0,TITLEHEIGHT,1280,720,NULL,NULL);
	DrawAlphaPicture(IM_Masks_0,0,TITLEHEIGHT,1280,720,NULL,NULL,64);
	PutText("游戏结束",90,50+TITLEHEIGHT,330,110+TITLEHEIGHT,255,255,255,80,FT_Title,NULL,NULL);
	if(ifLoad){
		PutTextC("读档后比赛用时",640,45+TITLEHEIGHT,1190,80+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
	}
	else{
		PutTextC("比赛用时",640,45+TITLEHEIGHT,1190,80+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
	}
	sprintf(win_temp_text,"%d小时 %d分钟 %d秒钟",(GameTimeSys->Total_t - (GameTimeSys->Total_t % 60)
											  						   - (GameTimeSys->Total_t - GameTimeSys->Total_t % 60) / 60 % 60 * 60) / 3600,
																		 (GameTimeSys->Total_t - GameTimeSys->Total_t % 60) / 60 % 60,
																		 (GameTimeSys->Total_t % 60));
	PutTextC(win_temp_text,640,80+TITLEHEIGHT,1190,115+TITLEHEIGHT,254,203,23,35,FT_Title,NULL,NULL);
	DrawAlphaPicture(IM_AvatarBack,172,200+TITLEHEIGHT,168,168,NULL,NULL,225);
	switch(GameMode){
		case GM_SINGLEPLAYER:
			if(WinUser!=NULL){
				DrawAvatar(WinUser->Avatar,174,202+TITLEHEIGHT,164,164,NULL,NULL);
				PutText(WinUser->Name,370,273+TITLEHEIGHT,820,367+TITLEHEIGHT,254,203,23,75,FT_Text,NULL,NULL);
			}
			else{
				DrawAvatar(IM_AI,174,202+TITLEHEIGHT,164,164,NULL,NULL);
				PutText("人工智能",370,273+TITLEHEIGHT,820,367+TITLEHEIGHT,254,203,23,75,FT_Text,NULL,NULL);
			}
			PutText("获胜者",370,202+TITLEHEIGHT,820,273+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
			PutText("得分",820,202+TITLEHEIGHT,1120,273+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
			sprintf(win_temp_text,"%d分",Winscore);
			PutText(win_temp_text,820,273+TITLEHEIGHT,1120,367+TITLEHEIGHT,254,203,23,75,FT_Text,NULL,NULL);
			PutText("玩家一",150,460+TITLEHEIGHT,300,510+TITLEHEIGHT,255,255,255,40,FT_Text,NULL,NULL);
			DrawAlphaPicture(IM_ExpBar_0,350,475+TITLEHEIGHT,590,18,NULL,NULL,200);
			if(User1->Exp) DrawAreaPicture(IM_ExpBar_1,350,475+TITLEHEIGHT,590,18,0,0,(int)590*User1->Exp/1000.0,18,NULL,NULL);
			sprintf(win_temp_text,"Lv.%d",User1->Level);
			PutTextB(win_temp_text,990,460+TITLEHEIGHT,1100,510+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
		case GM_MULTIPLAYER:
			PutText("获胜者",370,202+TITLEHEIGHT,820,273+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
			DrawAvatar(WinUser->Avatar,174,202+TITLEHEIGHT,164,164,NULL,NULL);
			PutText(WinUser->Name,370,273+TITLEHEIGHT,820,367+TITLEHEIGHT,254,203,23,75,FT_Text,NULL,NULL);
			PutText("得分",820,202+TITLEHEIGHT,1120,273+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
			sprintf(win_temp_text,"%d分",Winscore);
			PutText(win_temp_text,820,273+TITLEHEIGHT,1120,367+TITLEHEIGHT,254,203,23,75,FT_Text,NULL,NULL);
			PutText("玩家一",150,460+TITLEHEIGHT,300,510+TITLEHEIGHT,255,255,255,40,FT_Text,NULL,NULL);
			DrawAlphaPicture(IM_ExpBar_0,350,475+TITLEHEIGHT,590,18,NULL,NULL,200);
			if(User1->Exp) DrawAreaPicture(IM_ExpBar_1,350,475+TITLEHEIGHT,590,18,0,0,(int)590*User1->Exp/1000.0,18,NULL,NULL);
			PutText("玩家二",150,530+TITLEHEIGHT,300,580+TITLEHEIGHT,255,255,255,40,FT_Text,NULL,NULL);
			DrawAlphaPicture(IM_ExpBar_0,350,545+TITLEHEIGHT,590,18,NULL,NULL,200);
			if(User2->Exp) DrawAreaPicture(IM_ExpBar_1,350,545+TITLEHEIGHT,590,18,0,0,(int)590*User2->Exp/1000.0,18,NULL,NULL);
			sprintf(win_temp_text,"Lv.%d",User1->Level);
			PutTextB(win_temp_text,990,460+TITLEHEIGHT,1100,510+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			sprintf(win_temp_text,"Lv.%d",User2->Level);
			PutTextB(win_temp_text,990,530+TITLEHEIGHT,1100,580+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
		case GM_POOL:
			PutText("获胜者",370,202+TITLEHEIGHT,820,273+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
			DrawAvatar(WinUser->Avatar,174,202+TITLEHEIGHT,164,164,NULL,NULL);
			PutText(WinUser->Name,370,273+TITLEHEIGHT,820,367+TITLEHEIGHT,254,203,23,75,FT_Text,NULL,NULL);
			PutText("进球",820,202+TITLEHEIGHT,1120,273+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
			if(Winscore!=0)
				sprintf(win_temp_text,"%d个",Winscore);
			else
				sprintf(win_temp_text,"-");
			PutText(win_temp_text,820,273+TITLEHEIGHT,1120,367+TITLEHEIGHT,254,203,23,75,FT_Text,NULL,NULL);
			PutText("玩家一",150,460+TITLEHEIGHT,300,510+TITLEHEIGHT,255,255,255,40,FT_Text,NULL,NULL);
			DrawAlphaPicture(IM_ExpBar_0,350,475+TITLEHEIGHT,590,18,NULL,NULL,200);
			if(User1->Exp) DrawAreaPicture(IM_ExpBar_1,350,475+TITLEHEIGHT,590,18,0,0,(int)590*User1->Exp/1000.0,18,NULL,NULL);
			PutText("玩家二",150,530+TITLEHEIGHT,300,580+TITLEHEIGHT,255,255,255,40,FT_Text,NULL,NULL);
			DrawAlphaPicture(IM_ExpBar_0,350,545+TITLEHEIGHT,590,18,NULL,NULL,200);
			if(User2->Exp) DrawAreaPicture(IM_ExpBar_1,350,545+TITLEHEIGHT,590,18,0,0,(int)590*User2->Exp/1000.0,18,NULL,NULL);
			sprintf(win_temp_text,"Lv.%d",User1->Level);
			PutTextB(win_temp_text,990,460+TITLEHEIGHT,1100,510+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			sprintf(win_temp_text,"Lv.%d",User2->Level);
			PutTextB(win_temp_text,990,530+TITLEHEIGHT,1100,580+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
		case GM_SNOOKER:
			PutText("获胜者",370,202+TITLEHEIGHT,820,273+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
			DrawAvatar(WinUser->Avatar,174,202+TITLEHEIGHT,164,164,NULL,NULL);
			PutText(WinUser->Name,370,273+TITLEHEIGHT,820,367+TITLEHEIGHT,254,203,23,75,FT_Text,NULL,NULL);
			PutText("得分",820,202+TITLEHEIGHT,1120,273+TITLEHEIGHT,255,255,255,50,FT_Title,NULL,NULL);
			sprintf(win_temp_text,"%d分",Winscore);
			PutText(win_temp_text,820,273+TITLEHEIGHT,1120,367+TITLEHEIGHT,254,203,23,75,FT_Text,NULL,NULL);
			PutText("玩家一",150,460+TITLEHEIGHT,300,510+TITLEHEIGHT,255,255,255,40,FT_Text,NULL,NULL);
			DrawAlphaPicture(IM_ExpBar_0,350,475+TITLEHEIGHT,590,18,NULL,NULL,200);
			if(User1->Exp) DrawAreaPicture(IM_ExpBar_1,350,475+TITLEHEIGHT,590,18,0,0,(int)590*User1->Exp/1000.0,18,NULL,NULL);
			PutText("玩家二",150,530+TITLEHEIGHT,300,580+TITLEHEIGHT,255,255,255,40,FT_Text,NULL,NULL);
			DrawAlphaPicture(IM_ExpBar_0,350,545+TITLEHEIGHT,590,18,NULL,NULL,200);
			if(User2->Exp) DrawAreaPicture(IM_ExpBar_1,350,545+TITLEHEIGHT,590,18,0,0,(int)590*User2->Exp/1000.0,18,NULL,NULL);
			sprintf(win_temp_text,"Lv.%d",User1->Level);
			PutTextB(win_temp_text,990,460+TITLEHEIGHT,1100,510+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			sprintf(win_temp_text,"Lv.%d",User2->Level);
			PutTextB(win_temp_text,990,530+TITLEHEIGHT,1100,580+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
	}
	Draw32BitPicture(IM_Crown,125,150+TITLEHEIGHT,100,100,NULL,NULL,255);
	switch(BT_Win_Back){
		case 0:
			if(f_win_back)
				f_win_back = !f_win_back;
			DrawAlphaPicture(IM_WinBtn0,30,625+TITLEHEIGHT,210,65,NULL,NULL,WinPicAlpha1);
		break;
		case 1:
			if(!f_win_back){
				f_win_back = !f_win_back;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_WinBtn0,30,625+TITLEHEIGHT,210,65,NULL,NULL,WinPicAlpha2);
		break;
		case 2:
			DrawAlphaPicture(IM_WinBtn0,30,625+TITLEHEIGHT,210,65,NULL,NULL,WinPicAlpha3);
		break;
	}
	switch(BT_Win_Save){
		case 0:
			if(f_win_save)
				f_win_save = !f_win_save;
			DrawAlphaPicture(IM_WinBtn1,1040,625+TITLEHEIGHT,210,65,NULL,NULL,WinPicAlpha1);
		break;
		case 1:
			if(!f_win_save){
				f_win_save = !f_win_save;
				PlayOnBtMusic();
			}
			DrawAlphaPicture(IM_WinBtn1,1040,625+TITLEHEIGHT,210,65,NULL,NULL,WinPicAlpha2);
		break;
		case 2:
			DrawAlphaPicture(IM_WinBtn1,1040,625+TITLEHEIGHT,210,65,NULL,NULL,WinPicAlpha3);
		break;
	}
	PutText("返回主界面",30,625+TITLEHEIGHT,240,690+TITLEHEIGHT,255,255,255,40,FT_Title,NULL,NULL);
	PutText("保存截图",1040,625+TITLEHEIGHT,1250,690+TITLEHEIGHT,255,255,255,40,FT_Title,NULL,NULL);
}

void StartWinMenu(){
	PlayWinnMusic();
	startTimer(TM_WIN,1);
	MOUSE_STATE=MS_WIN;
	KEYBOARD_STATE=KB_WIN;
}

void EndWinMenu(){
	cancelTimer(TM_WIN);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
}

void SaveWinPage(){
	char temp[50];
	LPIMAGE IM_WinPage;
	LoadPictureFromWnd(&IM_WinPage,0,TITLEHEIGHT,1280,720,NULL);
	/*存档时间*/
	time_t timep;
	time(&timep);
	timep+=8*3600;
	struct tm *p = gmtime(&timep);
	sprintf(temp,"Capt/%04d-%02d-%02d-%02d-%02d-%02d.BMP",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	FILE *fp=fopen(temp,"wb");
	WriteBMP(IM_WinPage,fp);
	fclose(fp);
	FreePicture(&IM_WinPage);
}

/*游戏结束界面End*/
/*-------------------------------------------------------------------------------------------------*/
/*绘制游戏提示Begin*/

bool f_PlayMsg=false;
char s_PlayMsg[100];

void DrawHintMsge(){
	if(f_PlayMsg){
		DrawAlphaPicture(IM_Masks_0,0,260+TITLEHEIGHT,1280,110,NULL,NULL,128);
		PutText(s_PlayMsg,0,260+TITLEHEIGHT,1280,370+TITLEHEIGHT,255,255,255,80,FT_Title,NULL,NULL);
	}
}

void ShowHintMsge(char* Msg){
	f_PlayMsg=true;
	strcpy(s_PlayMsg,Msg);
	startTimer(TM_HINT,2000);
}

void HideHintMsge(){
	f_PlayMsg=false;
	cancelTimer(TM_HINT);
}

/*绘制游戏提示End*/
/*-------------------------------------------------------------------------------------------------*/
/*绘制游戏界面Begin*/

void DrawBKGround()
{
	DrawAreaPicture(IM_Table,0,TITLEHEIGHT,1280,720+TITLEHEIGHT,0,TITLEHEIGHT,1280,720,NULL,NULL);
	DrawAvatar(User1->Avatar,1,624+TITLEHEIGHT,95,95,NULL,NULL);
	PutText(User1->Name,132,627+TITLEHEIGHT,318,658+TITLEHEIGHT,0,0,0,25,FT_Text,NULL,NULL);
	switch(GameMode)
	{
		case GM_SINGLEPLAYER: case GM_MULTIPLAYER: case GM_POOL: case GM_SNOOKER:
		{
			if(GameMode == GM_SINGLEPLAYER)
			{
				DrawAvatar(IM_AI,1184,624+TITLEHEIGHT,95,95,NULL,NULL);
				PutText("人工智能",960,628+TITLEHEIGHT,1146,659+TITLEHEIGHT,0,0,0,25,FT_Text,NULL,NULL);			
			}
			else
			{
				DrawAvatar(User2->Avatar,1184,624+TITLEHEIGHT,95,95,NULL,NULL);
				PutText(User2->Name,960,628+TITLEHEIGHT,1146,659+TITLEHEIGHT,0,0,0,25,FT_Text,NULL,NULL);
			}
			if(GameMode != GM_POOL && GameMode != GM_SNOOKER)
			{
				char ScoreText1[5];
				char ScoreText2[5];
				sprintf(ScoreText1, "%d", Classic.ScorePlayer1);
				sprintf(ScoreText2, "%d", Classic.ScorePlayer2);
				PutText(ScoreText1,353,654+TITLEHEIGHT,482,720+TITLEHEIGHT,0,0,0,80,FT_Num,NULL,NULL);
				PutText(ScoreText2,794,654+TITLEHEIGHT,924,720+TITLEHEIGHT,0,0,0,80,FT_Num,NULL,NULL);
			}
			else if(GameMode == GM_POOL)
			{	
				//绘制玩家花色 
				if(Pool.SideSolid == 0 && Pool.SideStrip == 1)
				{
					if(Pool.SideClear[0])
						DrawKeyColorPicture(IM_BlackSide, 353, 654 + TITLEHEIGHT, 129, 65, NULL, NULL);
					else
						DrawKeyColorPicture(IM_SolidSide, 354, 654 + TITLEHEIGHT, 129, 65, NULL, NULL);
					if(Pool.SideClear[1])
						DrawKeyColorPicture(IM_BlackSide, 795, 654 + TITLEHEIGHT, 129, 65, NULL, NULL);
					else
						DrawKeyColorPicture(IM_StripSide, 795, 654 + TITLEHEIGHT, 129, 65, NULL, NULL);					
				}
				else if(Pool.SideSolid == 1 && Pool.SideStrip == 0)
				{
					if(Pool.SideClear[0])
						DrawKeyColorPicture(IM_BlackSide, 353, 654 + TITLEHEIGHT, 129, 65, NULL, NULL);
					else
						DrawKeyColorPicture(IM_StripSide, 353, 654 + TITLEHEIGHT, 129, 65, NULL, NULL);
					if(Pool.SideClear[1])
						DrawKeyColorPicture(IM_BlackSide, 795, 654 + TITLEHEIGHT, 129, 65, NULL, NULL);
					else
						DrawKeyColorPicture(IM_SolidSide, 795, 654 + TITLEHEIGHT, 129, 65, NULL, NULL);
				}
			}
			else if(GameMode == GM_SNOOKER)
			{
				char ScoreText1[5];
				char ScoreText2[5];
				sprintf(ScoreText1, "%d", Snooker.Player1Score);
				sprintf(ScoreText2, "%d", Snooker.Player2Score);
				PutText(ScoreText1, 353, 654 + TITLEHEIGHT, 482, 720 + TITLEHEIGHT, 0, 0, 0, 80, FT_Num, NULL, NULL);
				PutText(ScoreText2, 794, 654 + TITLEHEIGHT, 924, 720 + TITLEHEIGHT, 0, 0, 0, 80, FT_Num, NULL, NULL);				
				if(!Sides)
				{
					/*倒计时*/ 
					PutText("倒计时　　　",143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					if(GameTimeSys->countdown_n<=5)
					{
						PutText(GameTimeSys->countdown_s,143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,10,10,35,FT_Title,NULL,NULL);
					}
					else if(GameTimeSys->countdown_n<=10)
					{
						PutText(GameTimeSys->countdown_s,143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,111,20,35,FT_Title,NULL,NULL);
					}
					else
					{
						PutText(GameTimeSys->countdown_s,143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					}
					PutText("玩家一球权",935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					/*击球提示*/ 
					if(!Snooker.isRedClear)
					{
						if(Snooker.isRedTurn)
							DrawKeyColorPicture(IM_Billiards3, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL);
						else
							DrawKeyColorPicture(IM_Colour, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL);			
					}
					else
					{
						switch(Snooker.NowTime)
						{
							case 1: DrawKeyColorPicture(IM_Billiards1, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 2: DrawKeyColorPicture(IM_Billiards6, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 3: DrawKeyColorPicture(IM_Billiards7, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 4: DrawKeyColorPicture(IM_Billiards2, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 5: DrawKeyColorPicture(IM_Billiards4, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 6: DrawKeyColorPicture(IM_Billiards8, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
						}
					}
					DrawKeyColorPicture(IM_Billiards0, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL);
				}
				else
				{
					/*倒计时*/ 
					PutText("玩家二球权",143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					PutText("倒计时　　　",935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					if(GameTimeSys->countdown_n<=5)
					{
						PutText(GameTimeSys->countdown_s,935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,10,10,35,FT_Title,NULL,NULL);
					}
					else if(GameTimeSys->countdown_n<=10)
					{
						PutText(GameTimeSys->countdown_s,935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,111,20,35,FT_Title,NULL,NULL);
					}
					else
					{
						PutText(GameTimeSys->countdown_s,935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					}
					/*击球提示*/
					if(!Snooker.isRedClear)
					{
						if(Snooker.isRedTurn)
							DrawKeyColorPicture(IM_Billiards3, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL);
						else
							DrawKeyColorPicture(IM_Colour, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL);			
					}
					else
					{
						switch(Snooker.NowTime)
						{
							case 1: DrawKeyColorPicture(IM_Billiards1, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 2: DrawKeyColorPicture(IM_Billiards6, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 3: DrawKeyColorPicture(IM_Billiards7, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 4: DrawKeyColorPicture(IM_Billiards2, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 5: DrawKeyColorPicture(IM_Billiards4, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
							case 6: DrawKeyColorPicture(IM_Billiards8, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL); break;
						}
					}
					DrawKeyColorPicture(IM_Billiards0, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL);	 
				}
			}
			if(GameMode != GM_SNOOKER)
			{
				if(!Sides){
					/*倒计时*/ 
					PutText("倒计时　　　",143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					if(GameTimeSys->countdown_n<=5)
					{
						PutText(GameTimeSys->countdown_s,143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,10,10,35,FT_Title,NULL,NULL);
					}
					else if(GameTimeSys->countdown_n<=10)
					{
						PutText(GameTimeSys->countdown_s,143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,111,20,35,FT_Title,NULL,NULL);
					}
					else
					{
						PutText(GameTimeSys->countdown_s,143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					}
					PutText("玩家一球权",935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					/*击球提示*/ 
					switch(GameMode)
					{
						case GM_SINGLEPLAYER: case GM_MULTIPLAYER:
						{
							DrawKeyColorPicture(IM_BilliardsEmoji0, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL);
						} break;
						case GM_POOL:
						{
							DrawKeyColorPicture(IM_Billiards3, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL);
						} break;
					}
					DrawKeyColorPicture(IM_Billiards0, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL);
				}
				else{
					/*倒计时*/ 
					PutText("玩家二球权",143,678+TITLEHEIGHT,342,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					PutText("倒计时　　　",935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					if(GameTimeSys->countdown_n<=5)
					{
						PutText(GameTimeSys->countdown_s,935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,10,10,35,FT_Title,NULL,NULL);
					}
					else if(GameTimeSys->countdown_n<=10)
					{
						PutText(GameTimeSys->countdown_s,935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,111,20,35,FT_Title,NULL,NULL);
					}
					else
					{
						PutText(GameTimeSys->countdown_s,935,678+TITLEHEIGHT,1136,719+TITLEHEIGHT,255,249,237,35,FT_Title,NULL,NULL);
					}
					/*击球提示*/ 
					DrawKeyColorPicture(IM_Billiards0, 115, 685+TITLEHEIGHT, 28, 28, NULL, NULL);
					switch(GameMode)
					{
						case GM_SINGLEPLAYER: case GM_MULTIPLAYER:
						{
							DrawKeyColorPicture(IM_BilliardsEmoji0, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL);
						} break;
						case GM_POOL:
						{
							DrawKeyColorPicture(IM_Billiards3, 1136, 685+TITLEHEIGHT, 28, 28, NULL, NULL);
						} break;
					}
				}
			}
		} break;
		case GM_TRAINNING:
		{
			char ScoreText[5];
			sprintf(ScoreText, "%d", Trainning.Score);
			PutText(ScoreText,353,654+TITLEHEIGHT,482,720+TITLEHEIGHT,0,0,0,80,FT_Num,NULL,NULL);
		} break;
	}
}

/*绘制游戏界面End*/
/*-------------------------------------------------------------------------------------------------*/
/*获取击球力度Begin*/

extern HCURSOR CS_Cursor; 

void StartGetPower()
{
	Power = 0;
	MOUSE_STATE=MS_GET_FORCE;
	KEYBOARD_STATE=KB_GET_FORCE;
	startTimer(TM_FORCEBAR_CHANGE, 10);
}

void EndGetPower()
{
	cancelTimer(TM_FORCEBAR_CHANGE);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	ResetCursors();
}

void DrawForceBar(){
	static int f = -1;
	DrawAreaPicture(IM_ForceBar,512,685+TITLEHEIGHT,253,30,0,0,(int)(253*Power/100.0),30,NULL,NULL);
	Power+=f*ForceBarStep;
	if(Power>100 || Power<0) Power-=f*ForceBarStep,f=-f;
}

void UpdtForceBar(){
	DrawAreaPicture(IM_ForceBar,512,685+TITLEHEIGHT,253,30,0,0,(int)(253*Power/100.0),30,NULL,NULL);
}

/*获取击球力度End*/
/*-------------------------------------------------------------------------------------------------*/
/*获取击球点Begin*/

extern int const Dot_R;
extern int HitPosition_X;
extern int HitPosition_Y;
extern int CurrentPosition_X;
extern int CurrentPosition_Y;
extern int CueBallPosition_X;
extern int CueBallPosition_Y;
extern int HitStopKey;
extern float CueTheta;	

static int theta = 0;
static int theta_r = 5;
static int theta_R = 65;
static char theta_text[5];

static bool theta_bar = false;

void StartGetHit()
{
	theta = 0;
	MOUSE_STATE=MS_GET_HIT_POSITION;
	KEYBOARD_STATE=KB_GET_HIT_POSITION;
	startTimer(TM_HIT_POSITION_CHANGE,1);
	if(GameMode != GM_TRAINNING)
		StartCountDown();
}

void EndGetHit()
{
	cancelTimer(TM_HIT_POSITION_CHANGE);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
	CueTheta=theta*M_PI/180.0;
}

void UpdateHitPos() //旋球 
{
	DrawKeyColorPicture(IM_RedPoint, HitPosition_X - Dot_R, HitPosition_Y - Dot_R, 11, 11, NULL, NULL);
}

void TargetPoints()
{
	DrawAimLines();
	if(CurrentPosition_X != -1)
		DrawKeyColorPicture(IM_Target, CurrentPosition_X - Ball_R, CurrentPosition_Y - Ball_R, 2 * Ball_R, 2 * Ball_R, NULL, NULL);
}

void DrawAimLines()
{
	if(CurrentPosition_X != -1){
		MovePen(ScaleXInches(CurrentPosition_X),ScaleYInches(CurrentPosition_Y));
		DrawLine(InchesX(Ball[0].Position_X - CurrentPosition_X),InchesY(CurrentPosition_Y - Ball[0].Position_Y));
	}
}

void DrawThetaBar()
{
	if(theta_bar){
		sprintf(theta_text,"%d°",theta);
		Draw32BitPicture(IM_SetTheta,565,285+TITLEHEIGHT,150,150,NULL,NULL,225);
		PutText(theta_text,665,300+TITLEHEIGHT,715,335+TITLEHEIGHT,250,140,65,20,FT_Text,NULL,NULL);
		RotatePictureB(IM_Stick,594 + (theta_r + theta_R)*(float)cos(theta*M_PI/180.0) - theta_R,
								406 + TITLEHEIGHT - (theta_r + theta_R)*(float)sin(theta*M_PI/180.0) - theta_R,
								theta_R*2,
								theta_R*2,
								(float)cos(theta*M_PI/180.0),
								(float)sin(theta*M_PI/180.0),
								NULL,
								NULL);
	}
}

void AddTheta(){
	if(theta_bar)
		cancelTimer(TM_GET_THETA);
	theta_bar = true;
	if(theta+5<=90)
		theta += 5;
	startTimer(TM_GET_THETA,1000);
}

void CutTheta(){
	if(theta_bar)
		cancelTimer(TM_GET_THETA);
	theta_bar = true;
	if(theta-5>=0)
		theta -= 5;
	startTimer(TM_GET_THETA,1000);
}

void HideThetaBar(){
	cancelTimer(TM_GET_THETA);
	theta_bar = false;
}

/*获取击球点End*/
/*-------------------------------------------------------------------------------------------------*/
/*自由放置母球Begin*/

void StartFreeResetCueBall()
{
	MOUSE_STATE=MS_FREE_RESET_CUEBALL;
	KEYBOARD_STATE=KB_FREE_RESET_CUEBALL;
	startTimer(TM_FREE_RESET_CUEBALL,10);
	if(GameMode != GM_TRAINNING)
		StartCountDown();
}

void EndFreeResetCueBall()
{
	cancelTimer(TM_FREE_RESET_CUEBALL);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
}

void DrawACueBall()
{
	if(CueBallPosition_X != -1)
		DrawKeyColorPicture(Ball[0].Address, CueBallPosition_X - Ball_R, CueBallPosition_Y - Ball_R, 2 * Ball_R, 2 * Ball_R, NULL, NULL);
}

/*自由放置母球End*/
/*-------------------------------------------------------------------------------------------------*/
/*显示球杆Begin*/

LPIMAGE Active_Stick[2];

static int stick_d = 16;

static float PowerToDistance(float _power){
	return pow(_power,1.2)/3;
}

void DrawStickBar(){
	VECTOR Direction = {CurrentPosition_X - Ball[0].Position_X, CurrentPosition_Y - Ball[0].Position_Y};
	UnitVec(Direction);
	//printf("cos= %f,sin= %f\n",-(float)VecCos(Direction),(float)VecSin(Direction));
	RotatePictureD(	Active_Stick[0],
					Active_Stick[1],
					Ball[0].Position_X,
					Ball[0].Position_Y,
					stick_d,
					Direction[0],
					Direction[1],
					NULL,
					NULL);
}

void UpdtStickBar(){
	VECTOR Direction = {CurrentPosition_X - Ball[0].Position_X, CurrentPosition_Y - Ball[0].Position_Y};
	UnitVec(Direction);
	//printf("cos= %f,sin= %f\n",-(float)VecCos(Direction),(float)VecSin(Direction));
	RotatePictureD(	Active_Stick[0],
					Active_Stick[1],
					Ball[0].Position_X,
					Ball[0].Position_Y,
					stick_d + PowerToDistance(Power),
					Direction[0],
					Direction[1],
					NULL,
					NULL);
}

/*显示球杆End*/
/*-------------------------------------------------------------------------------------------------*/
/*准备击球Begin*/

static bool _mode;
static float _power;

void SetPrepareMode(bool mode)
{
	_mode = mode;
}

void StartPrepare()
{
	if(GameMode != GM_TRAINNING)
		EndCountDown();
	if(!_mode){
		_power = PowerToDistance(Power);
		startTimer(TM_PREPARE_TO_HIT,1);
	}
	else{
		_power = 0;
		startTimer(TM_PREPARE_TO_HIT,40);
	}
	MOUSE_STATE=MS_PREPARE_TO_HIT;
	KEYBOARD_STATE=KB_PREPARE_TO_HIT;
}

void EndPrepare()
{
	cancelTimer(TM_PREPARE_TO_HIT);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
}

void PrepareStick()
{
	//printf("P: %f\n",_power);
	VECTOR Direction = {CurrentPosition_X - Ball[0].Position_X, CurrentPosition_Y - Ball[0].Position_Y};
	UnitVec(Direction);
	if(!_mode){
		_power -= 300/_power;
		if(_power <= 5){
			EndPrepare();
			CurrentPosition_X = -1;
			CurrentPosition_Y = -1;
			StartHit();
		}
		else
			RotatePictureD(Active_Stick[0],Active_Stick[1],Ball[0].Position_X,Ball[0].Position_Y,stick_d + _power,Direction[0],Direction[1],NULL,NULL);
	}
	else{
		_power += ForceBarStep;
		if(_power >= PowerToDistance(Power)){
			_mode = !_mode;
			cancelTimer(TM_PREPARE_TO_HIT);
			startTimer(TM_PREPARE_TO_HIT,1);
		}
		else
			RotatePictureD(Active_Stick[0],Active_Stick[1],Ball[0].Position_X,Ball[0].Position_Y,stick_d + PowerToDistance(_power),Direction[0],Direction[1],NULL,NULL);
	}
}

/*准备击球End*/
/*-------------------------------------------------------------------------------------------------*/
/*更换球杆Begin*/

/*客户区控件*/ 
extern int BT_Stick_back,BT_Stick_0,BT_Stick_1,BT_Stick_2,BT_Stick_3,
		   BT_Stick_4,BT_Stick_5,BT_Stick_6,BT_Stick_7,BT_Stick_8; 
static int f_stick_back=0,f_stick_0=0,f_stick_1=0,f_stick_2=0,f_stick_3=0,
		   f_stick_4=0,f_stick_5=0,f_stick_6=0,f_stick_7=0,f_stick_8=0;

int Stick_Choice;

void UpdtSticks(){
	if(Sides == 0){
		Active_Stick[0] = IM_StickArr[User1->Stick][0];
		Active_Stick[1] = IM_StickArr[User1->Stick][1];
	}else{
		if(GameMode == GM_SINGLEPLAYER){
			Active_Stick[0] = IM_StickArr[0][0];
			Active_Stick[1] = IM_StickArr[0][1];
		}else{
			Active_Stick[0] = IM_StickArr[User2->Stick][0];
			Active_Stick[1] = IM_StickArr[User2->Stick][1];
		}
	}
}

void DrawStickBtn(){	
	DrawPicture(WhichUser->BackGround,0,0+TITLEHEIGHT,1280,720,NULL,NULL);
	PutText("自定义球杆",90,50+TITLEHEIGHT,410,110+TITLEHEIGHT,255,255,255,80,FT_Title,NULL,NULL);
	PutTextC("*此处有缩放请以实物为准",640,50+TITLEHEIGHT,1165,110+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
	/*底板*/
	switch(BT_Stick_0){
		case 0:
			if(f_stick_0)
				f_stick_0 = !f_stick_0;
			Draw32BitPicture(IM_ScoreBoard,120,160+TITLEHEIGHT,320,110,NULL,NULL,128);
		break;
		case 1:
			if(!f_stick_0){
				f_stick_0 = !f_stick_0;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_ScoreBoard,120,160+TITLEHEIGHT,320,110,NULL,NULL,192);
			PutText("点击选择橡木球杆",300,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_ScoreBoard,120,160+TITLEHEIGHT,320,110,NULL,NULL,225);
		break;
	}
	switch(BT_Stick_1){
		case 0:
			if(f_stick_1)
				f_stick_1 = !f_stick_1;
			Draw32BitPicture(IM_ScoreBoard,480,160+TITLEHEIGHT,320,110,NULL,NULL,128);
		break;
		case 1:
			if(!f_stick_1){
				f_stick_1 = !f_stick_1;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_ScoreBoard,480,160+TITLEHEIGHT,320,110,NULL,NULL,192);
			PutText("点击选择碳素轻杆",300,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_ScoreBoard,480,160+TITLEHEIGHT,320,110,NULL,NULL,225);
		break;
	}
	switch(BT_Stick_2){
		case 0:
			if(f_stick_2)
				f_stick_2 = !f_stick_2;
			Draw32BitPicture(IM_ScoreBoard,840,160+TITLEHEIGHT,320,110,NULL,NULL,128);
		break;
		case 1:
			if(!f_stick_2){
				f_stick_2 = !f_stick_2;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_ScoreBoard,840,160+TITLEHEIGHT,320,110,NULL,NULL,192);
			PutText("点击选择白蜡木球杆",300,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_ScoreBoard,840,160+TITLEHEIGHT,320,110,NULL,NULL,225);
		break;
	}
	switch(BT_Stick_3){
		case 0:
			if(f_stick_3)
				f_stick_3 = !f_stick_3;
			Draw32BitPicture(IM_ScoreBoard,120,310+TITLEHEIGHT,320,110,NULL,NULL,128);
		break;
		case 1:
			if(!f_stick_3){
				f_stick_3 = !f_stick_3;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_ScoreBoard,120,310+TITLEHEIGHT,320,110,NULL,NULL,192);
			PutText("点击选择斯诺克球杆",300,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_ScoreBoard,120,310+TITLEHEIGHT,320,110,NULL,NULL,225);
		break;
	}
	switch(BT_Stick_4){
		case 0:
			if(f_stick_4)
				f_stick_4 = !f_stick_4;
			Draw32BitPicture(IM_ScoreBoard,480,310+TITLEHEIGHT,320,110,NULL,NULL,128);
		break;
		case 1:
			if(!f_stick_4){
				f_stick_4 = !f_stick_4;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_ScoreBoard,480,310+TITLEHEIGHT,320,110,NULL,NULL,192);
			PutText("点击选择金属重杆",300,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_ScoreBoard,480,310+TITLEHEIGHT,320,110,NULL,NULL,225);
		break;
	}
	switch(BT_Stick_5){
		case 0:
			if(f_stick_5)
				f_stick_5 = !f_stick_5;
			Draw32BitPicture(IM_ScoreBoard,840,310+TITLEHEIGHT,320,110,NULL,NULL,128);
		break;
		case 1:
			if(!f_stick_5){
				f_stick_5 = !f_stick_5;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_ScoreBoard,840,310+TITLEHEIGHT,320,110,NULL,NULL,192);
			PutText("点击选择枫木球杆",300,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_ScoreBoard,840,310+TITLEHEIGHT,320,110,NULL,NULL,225);
		break;
	}
	switch(BT_Stick_6){
		case 0:
			if(f_stick_6)
				f_stick_6 = !f_stick_6;
			Draw32BitPicture(IM_ScoreBoard,120,460+TITLEHEIGHT,320,110,NULL,NULL,128);
		break;
		case 1:
			if(!f_stick_6){
				f_stick_6 = !f_stick_6;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_ScoreBoard,120,460+TITLEHEIGHT,320,110,NULL,NULL,192);
			PutText("点击选择光剑",300,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_ScoreBoard,120,460+TITLEHEIGHT,320,110,NULL,NULL,225);
		break;
	}
	switch(BT_Stick_7){
		case 0:
			if(f_stick_7)
				f_stick_7 = !f_stick_7;
			Draw32BitPicture(IM_ScoreBoard,480,460+TITLEHEIGHT,320,110,NULL,NULL,128);
		break;
		case 1:
			if(!f_stick_7){
				f_stick_7 = !f_stick_7;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_ScoreBoard,480,460+TITLEHEIGHT,320,110,NULL,NULL,192);
			PutText("点击选择晾衣杆",300,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_ScoreBoard,480,460+TITLEHEIGHT,320,110,NULL,NULL,225);
		break;
	}
	switch(BT_Stick_8){
		case 0:
			if(f_stick_8)
				f_stick_8 = !f_stick_8;
			Draw32BitPicture(IM_ScoreBoard,840,460+TITLEHEIGHT,320,110,NULL,NULL,128);
		break;
		case 1:
			if(!f_stick_8){
				f_stick_8 = !f_stick_8;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_ScoreBoard,840,460+TITLEHEIGHT,320,110,NULL,NULL,192);
			PutText("点击选择求是强嘤杆",300,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_ScoreBoard,840,460+TITLEHEIGHT,320,110,NULL,NULL,225);
		break;
	}
	/*选择圈*/
	Draw32BitPicture(IM_UnSelect,130,170+TITLEHEIGHT,25,25,NULL,NULL,192);
	Draw32BitPicture(IM_UnSelect,490,170+TITLEHEIGHT,25,25,NULL,NULL,192);
	Draw32BitPicture(IM_UnSelect,850,170+TITLEHEIGHT,25,25,NULL,NULL,192);
	Draw32BitPicture(IM_UnSelect,130,320+TITLEHEIGHT,25,25,NULL,NULL,192);
	Draw32BitPicture(IM_UnSelect,490,320+TITLEHEIGHT,25,25,NULL,NULL,192);
	Draw32BitPicture(IM_UnSelect,850,320+TITLEHEIGHT,25,25,NULL,NULL,192);
	Draw32BitPicture(IM_UnSelect,130,470+TITLEHEIGHT,25,25,NULL,NULL,192);
	Draw32BitPicture(IM_UnSelect,490,470+TITLEHEIGHT,25,25,NULL,NULL,192);
	Draw32BitPicture(IM_UnSelect,850,470+TITLEHEIGHT,25,25,NULL,NULL,192);
	switch(Stick_Choice){
		case 0:
			Draw32BitPicture(IM_Select,130,170+TITLEHEIGHT,25,25,NULL,NULL,192);
		break;
		case 1:
			Draw32BitPicture(IM_Select,490,170+TITLEHEIGHT,25,25,NULL,NULL,192);
		break;
		case 2:
			Draw32BitPicture(IM_Select,850,170+TITLEHEIGHT,25,25,NULL,NULL,192);
		break;
		case 3:
			Draw32BitPicture(IM_Select,130,320+TITLEHEIGHT,25,25,NULL,NULL,192);
		break;
		case 4:
			Draw32BitPicture(IM_Select,490,320+TITLEHEIGHT,25,25,NULL,NULL,192);
		break;
		case 5:
			Draw32BitPicture(IM_Select,850,320+TITLEHEIGHT,25,25,NULL,NULL,192);
		break;
		case 6:
			Draw32BitPicture(IM_Select,130,470+TITLEHEIGHT,25,25,NULL,NULL,192);
		break;
		case 7:
			Draw32BitPicture(IM_Select,490,470+TITLEHEIGHT,25,25,NULL,NULL,192);
		break;
		case 8:
			Draw32BitPicture(IM_Select,850,470+TITLEHEIGHT,25,25,NULL,NULL,192);
		break;
	}
	/*杆子*/
	DrawKeyColorPicture(IM_StickArr[0][0],150,200+TITLEHEIGHT,260,25,NULL,NULL);
	DrawKeyColorPicture(IM_StickArr[1][0],510,200+TITLEHEIGHT,260,25,NULL,NULL);
	DrawKeyColorPicture(IM_StickArr[2][0],870,200+TITLEHEIGHT,260,25,NULL,NULL);
	DrawKeyColorPicture(IM_StickArr[3][0],150,350+TITLEHEIGHT,260,25,NULL,NULL);
	DrawKeyColorPicture(IM_StickArr[4][0],510,350+TITLEHEIGHT,260,25,NULL,NULL);
	DrawKeyColorPicture(IM_StickArr[5][0],870,350+TITLEHEIGHT,260,25,NULL,NULL);
	DrawKeyColorPicture(IM_StickArr[6][0],150,500+TITLEHEIGHT,260,25,NULL,NULL);
	DrawKeyColorPicture(IM_StickArr[7][0],510,500+TITLEHEIGHT,260,25,NULL,NULL);
	DrawKeyColorPicture(IM_StickArr[8][0],970,475+TITLEHEIGHT,70,80,NULL,NULL);
	switch(BT_Stick_back){
		case 0:
			if(f_stick_back)
				f_stick_back = !f_stick_back;
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,192);
			PutText("保存并返回",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
		case 1:
			if(!f_stick_back){
				f_stick_back = !f_stick_back;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,225);
			PutText("保存并返回",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
			PutText("点击保存并返回上一界面",0,625+TITLEHEIGHT,1280,670+TITLEHEIGHT,255,255,255,35,FT_Title,NULL,NULL);
		break;
		case 2:
			Draw32BitPicture(IM_GM_Esc_0,95,625+TITLEHEIGHT,205,45,NULL,NULL,255);
			PutText("保存并返回",95,625+TITLEHEIGHT,255,670+TITLEHEIGHT,255,255,255,30,FT_Title,NULL,NULL);
		break;
	}
}

void StartChooseStick(){
	Stick_Choice=WhichUser->Stick;
	startTimer(TM_CHOOSE_STICK,1);
	MOUSE_STATE=MS_CHOOSE_STICK;
	KEYBOARD_STATE=KB_CHOOSE_STICK;
}

void EndChooseStick(){
	WhichUser->Stick=Stick_Choice;
	SaveUsers(WhichUser);
	cancelTimer(TM_CHOOSE_STICK);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
}

/*更换球杆End*/
/*-------------------------------------------------------------------------------------------------*/
/*更新台球运动状态Begin*/

extern float RealHole_X[6];
extern float RealHole_Y[6];
extern bool InHoleFinished[16];
extern float Draw_R[16];
extern int WhichHole[16];

void StartHit(){
	MOUSE_STATE=MS_START_HIT;
	KEYBOARD_STATE=KB_START_HIT;
	startTimer(TM_START_HIT, RefreshRate);
}

void EndHit(){
	cancelTimer(TM_START_HIT);
	MOUSE_STATE=MS_NULL;
	KEYBOARD_STATE=KB_NULL;
}

void DrawAllBalls()
{
	//printf("Ball 0: Position - [%.2f, %.2f]\n", Ball[0].Position_X, Ball[0].Position_Y);
	int i, j;		
	for(i = 0; i < NUM; i ++)
	{
		if(Ball[i].State == ONTABLE)
			DrawKeyColorPicture(Ball[i].Address, (int)Ball[i].Position_X - Ball_R, (int)Ball[i].Position_Y - Ball_R, 2 * Ball_R, 2 * Ball_R, NULL, NULL);
		if(Ball[i].State == INHOLE)
		{
			if(InHoleFinished[i] && i != 0 && GameMode != GM_TRAINNING)
				DrawKeyColorPicture(Ball[i].Address, (int)Ball[i].Position_X - Ball_R, (int)Ball[i].Position_Y - Ball_R, 2 * Ball_R, 2 * Ball_R, NULL, NULL);
			else if(!InHoleFinished[i])
			{
				j = WhichHole[i];
				DrawKeyColorPicture(Ball[i].Address, (int)RealHole_X[j] - (int)Draw_R[i], (int)RealHole_Y[j] - (int)Draw_R[i], 2 * Draw_R[i], 2 * Draw_R[i], NULL, NULL);
				Draw_R[i] -= 0.2;
				if(Draw_R[i] <= 12)
				{
					Draw_R[i] = 16;
					InHoleFinished[i] = true;
					Ball[i].Moving = STOP;
					if(i == 0)
						Violation = 1; 
				}
			}
		}
	}
	if(GameMode != GM_TRAINNING && GameMode != GM_POOL && GameMode != GM_SNOOKER)
	{
		for(i = 0; i < Classic.Player1Violation; i ++)
			DrawKeyColorPicture(IM_Violation, ViolationShelf_X[0][i], 655 + TITLEHEIGHT, 15, 15, NULL, NULL);
		for(i = 0; i < Classic.Player2Violation; i ++)
			DrawKeyColorPicture(IM_Violation, ViolationShelf_X[1][i], 655 + TITLEHEIGHT, 15, 15, NULL, NULL);		
	}
}

/*更新台球运动状态End*/
/*-------------------------------------------------------------------------------------------------*/
/*重置鼠标指针Begin*/

void ResetCursors(){
	DestroyCursor(CS_Cursor);
	CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
	SetCursor(CS_Cursor);
}

/*重置鼠标指针End*/
/*-------------------------------------------------------------------------------------------------*/
/*音乐相关Begin*/

void PlayCtDnMusic(){
	PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_CountDown); 
}

void PlayWinnMusic(){
	PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_Win); 
}

void PlayDlteMusic(){
	PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_Recycle); 
}

void PlaySaveMusic(){
	PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_Saved); 
}

void PlayOnBtMusic(){
	PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_OnButton); 
}

void PlayQuitMusic(){
	srand((unsigned)time(NULL));
	switch(rand() % 2){
		case 0:
			PlaySound(NULL, NULL, SND_FILENAME);
			PlaySound((LPCSTR)MU_Quit1->data, NULL, SND_MEMORY | SND_ASYNC);
		break;
		case 1:
			PlaySound(NULL, NULL, SND_FILENAME);
			PlaySound((LPCSTR)MU_Quit2->data, NULL, SND_MEMORY | SND_ASYNC);
		break;
	}
}

void PlayClosMusic(){
	srand((unsigned)time(NULL));
	switch(rand() % 2){
		case 0:
			PlaySound(NULL, NULL, SND_FILENAME);
			PlaySound((LPCSTR)MU_Close1->data, NULL, SND_MEMORY | SND_ASYNC);
		break;
		case 1:
			PlaySound(NULL, NULL, SND_FILENAME);
			PlaySound((LPCSTR)MU_Close2->data, NULL, SND_MEMORY | SND_ASYNC);
		break;
	} 
}

/*音乐相关End*/
/*-------------------------------------------------------------------------------------------------*/
