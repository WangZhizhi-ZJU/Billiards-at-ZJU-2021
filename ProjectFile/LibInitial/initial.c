#include <gui.h>
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
#include <timesys.h>
#include <callback.h>

#include <test.h>
#include <error.h>
#include <initial.h>

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
				IM_GameMode,
				IM_PauseMenu,
				IM_Billiards0,
				IM_Billiards1,
				IM_Billiards2,
				IM_Billiards3,
				IM_Billiards4,
				IM_Billiards5,
				IM_Billiards6,
				IM_Billiards7,
				IM_Billiards8,
				IM_Billiards9,
				IM_PoolBilliards0,
				IM_PoolBilliards1,
				IM_PoolBilliards2,
				IM_PoolBilliards3,
				IM_PoolBilliards4,
				IM_PoolBilliards5,
				IM_PoolBilliards6,
				IM_PoolBilliards7,
				IM_PoolBilliards8,
				IM_PoolBilliards9,
				IM_PoolBilliards10,
				IM_PoolBilliards11,
				IM_PoolBilliards12,
				IM_PoolBilliards13,
				IM_PoolBilliards14,
				IM_PoolBilliards15,
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
				IM_UnSelect,
				IM_CourseBK0,
				IM_CourseBK1,
				IM_CourseBK2,
				IM_CourseBK3,
				IM_CoursePlay,
				IM_CoursePause,
				IM_CourseRight,
				IM_CourseLeft,
				IM_SettingBK,
				IM_SettingEnter,
				IM_SeekBar,
				IM_SeekBarCover,
				IM_SeekBarDot,
				IM_Developers,
				IM_Music,
				IM_EX_BK,
				IM_Welcome;

extern LPFONTS FT_Num,
			   FT_Title,
			   FT_Button,
			   FT_Text;
			  
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
			  
extern struct PoolInformation
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
	int SolidSet;						//当前全色球已摆放球数 
	int StripSet;						//当前花色球已摆放球数 
	int Inhole; 						//当前全局进球球数 
	int Ontable;						//当前全局桌上球数 
	bool isCueInhole;					//母球是否进球 
	bool isBlackInhole;					//判断黑八是否仍在桌面
	int BallState[16];					//记录十六个球的进球状态 
	int NewInhole[16];					//记录新的进球
	int TargetBall;						//记录目标球 
	int isSet;							//未摆放球是否妥当
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

extern HCURSOR CS_Cursor; 
extern char CollideAddress[10][9];
extern float const ShelfGap;
extern float Shelf_Y[15];
extern bool InHoleFinished[22];
extern float Draw_R[22];
extern int WhichHole[22];
extern MODE GameMode;
extern int NUM; 

/*文件版本信息*/
extern char IF_FileTitle[100];
extern char* IF_FileVersion;
bool GetFileVersion(char **version);

/*应用路径*/
extern char IF_FilePath[MAX_PATH];

bool InitSources(){
	/*版本信息*/
	if(!GetFileVersion(&IF_FileVersion)) return false;
	sprintf(IF_FileTitle,"求是台球",IF_FileVersion);
	printf("FilePath: [%s]\n",IF_FilePath); // 获取文件路径
	/*加载图片*/
	LPDATABASE BilliardsData;
	LoadDB("Billiards.db", &BilliardsData);
	LoadPictureFromDB(BilliardsData, 0, &IM_Billiards0, NULL);
	LoadPictureFromDB(BilliardsData, 1, &IM_Billiards1, NULL);
	LoadPictureFromDB(BilliardsData, 2, &IM_Billiards2, NULL);
	LoadPictureFromDB(BilliardsData, 3, &IM_Billiards3, NULL);
	LoadPictureFromDB(BilliardsData, 4, &IM_Billiards4, NULL);
	LoadPictureFromDB(BilliardsData, 5, &IM_Billiards5, NULL);
	LoadPictureFromDB(BilliardsData, 6, &IM_Billiards6, NULL);
	LoadPictureFromDB(BilliardsData, 7, &IM_Billiards7, NULL);
	LoadPictureFromDB(BilliardsData, 8, &IM_Billiards8, NULL);
	LoadPictureFromDB(BilliardsData, 9, &IM_Billiards9, NULL);
	LoadPictureFromDB(BilliardsData, 10, &IM_PoolBilliards0, NULL);
	LoadPictureFromDB(BilliardsData, 11, &IM_PoolBilliards1, NULL);
	LoadPictureFromDB(BilliardsData, 12, &IM_PoolBilliards2, NULL);
	LoadPictureFromDB(BilliardsData, 13, &IM_PoolBilliards3, NULL);
	LoadPictureFromDB(BilliardsData, 14, &IM_PoolBilliards4, NULL);
	LoadPictureFromDB(BilliardsData, 15, &IM_PoolBilliards5, NULL);
	LoadPictureFromDB(BilliardsData, 16, &IM_PoolBilliards6, NULL);
	LoadPictureFromDB(BilliardsData, 17, &IM_PoolBilliards7, NULL);
	LoadPictureFromDB(BilliardsData, 18, &IM_PoolBilliards8, NULL);
	LoadPictureFromDB(BilliardsData, 19, &IM_PoolBilliards9, NULL);
	LoadPictureFromDB(BilliardsData, 20, &IM_PoolBilliards10, NULL);
	LoadPictureFromDB(BilliardsData, 21, &IM_PoolBilliards11, NULL);
	LoadPictureFromDB(BilliardsData, 22, &IM_PoolBilliards12, NULL);
	LoadPictureFromDB(BilliardsData, 23, &IM_PoolBilliards13, NULL);
	LoadPictureFromDB(BilliardsData, 24, &IM_PoolBilliards14, NULL);
	LoadPictureFromDB(BilliardsData, 25, &IM_PoolBilliards15, NULL);
	LoadPictureFromDB(BilliardsData, 26, &IM_BilliardsEmoji0, NULL);
	LoadPictureFromDB(BilliardsData, 27, &IM_BilliardsEmoji1, NULL);
	LoadPictureFromDB(BilliardsData, 28, &IM_BilliardsEmoji2, NULL);
	LoadPictureFromDB(BilliardsData, 29, &IM_BilliardsEmoji3, NULL);
	LoadPictureFromDB(BilliardsData, 30, &IM_BilliardsEmoji4, NULL);
	LoadPictureFromDB(BilliardsData, 31, &IM_BilliardsEmoji5, NULL);
	LoadPictureFromDB(BilliardsData, 32, &IM_BilliardsEmoji6, NULL);
	LoadPictureFromDB(BilliardsData, 33, &IM_BilliardsEmoji7, NULL);
	LoadPictureFromDB(BilliardsData, 34, &IM_BilliardsEmoji8, NULL);
	LoadPictureFromDB(BilliardsData, 35, &IM_BilliardsEmoji9, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 0, &IM_StartMenu_0_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 1, &IM_StartMenu_0_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 2, &IM_StartMenu_0_2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 3, &IM_StartMenu_0_3, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 4, &IM_StartMenu_0_4, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 5, &IM_StartMenu_1_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 6, &IM_StartMenu_1_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 7, &IM_StartMenu_1_2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 8, &IM_StartMenu_1_3, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 9, &IM_StartMenu_1_4, NULL);	
	LoadPictureFromDB(BilliardsData, 36 + 10 + 0, &IM_Close_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 1, &IM_Close_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 2, &IM_Close_2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 3, &IM_Maximize_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 4, &IM_Minimize_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 5, &IM_Minimize_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 6, &IM_Minimize_2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 7, &IM_Info_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 8, &IM_Info_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 9, &IM_Info_2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 10, &IM_Setting_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 11, &IM_Setting_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 12, &IM_Setting_2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 13, &IM_Game_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 14, &IM_Game_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 15, &IM_Game_2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 16, &IM_Split, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 17, &IM_Icon, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 0, &IM_Violation, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 1, &IM_Target, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 2, &IM_Colour, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 3, &IM_BlackSide, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 4, &IM_StripSide, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 5, &IM_SolidSide, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 0, &IM_GM_Pool, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 1, &IM_GM_Pool_Blur, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 2, &IM_GM_Practice, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 3, &IM_GM_Practice_Blur, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 4, &IM_GM_AI, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 5, &IM_GM_AI_Blur, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 6, &IM_GM_Classical, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 7, &IM_GM_Classical_Blur, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 8, &IM_GM_Snooker, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 9, &IM_GM_Snooker_Blur, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 0, &IM_Stick, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 1, &IM_StickArr[0][0], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 2, &IM_StickArr[0][1], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 3, &IM_StickArr[1][0], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 4, &IM_StickArr[1][1], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 5, &IM_StickArr[2][0], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 6, &IM_StickArr[2][1], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 7, &IM_StickArr[3][0], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 8, &IM_StickArr[3][1], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 9, &IM_StickArr[4][0], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 10, &IM_StickArr[4][1], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 11, &IM_StickArr[5][0], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 12, &IM_StickArr[5][1], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 13, &IM_StickArr[6][0], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 14, &IM_StickArr[6][1], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 15, &IM_StickArr[7][0], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 16, &IM_StickArr[7][1], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 17, &IM_StickArr[8][0], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 18, &IM_StickArr[8][1], NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 0, &IM_Table, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 1, &IM_ForceBar, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 2, &IM_RedPoint, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 0, &IM_LoadPage, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 1, &IM_LoadBar, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 2, &IM_Masks_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 3, &IM_Masks_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 4, &IM_Choice, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 5, &IM_Begin_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 6, &IM_Begin_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 7, &IM_Begin_2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 8, &IM_LoadSave_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 9, &IM_LoadSave_2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 10, &IM_LoadSave_3, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 11, &IM_LoadSave_4, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 12, &IM_LoadSave_5, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 13, &IM_Register, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 14, &IM_CourseBK0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 15, &IM_CourseBK1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 16, &IM_CourseBK2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 17, &IM_CourseBK3, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 18, &IM_SettingBK, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 0, &IM_GreyBar, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 1, &IM_WhiteBar, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 2, &IM_Frame, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 3, &IM_StartGame_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 4, &IM_StartGame_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 5, &IM_GM_BK_0_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 6, &IM_GM_BK_1_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 7, &IM_GM_BT_0_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 8, &IM_GM_Esc_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 9, &IM_Pause_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 10, &IM_Pause_Continue, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 11, &IM_Pause_Save, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 12, &IM_Pause_Quit, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 13, &IM_SetTheta, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 14, &IM_ExpBar_0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 15, &IM_ExpBar_1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 16, &IM_TextBar, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 17, &IM_Reg_OK, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 18, &IM_Edit, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 19, &IM_Eagle, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 20, &IM_Cloth, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 21, &IM_ScoreBoard, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 22, &IM_Individual, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 23, &IM_AvatarBack, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 24, &IM_UserButton, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 25, &IM_SavePage_Btn, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 26, &IM_Save_Btn0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 27, &IM_Save_Btn1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 28, &IM_Save_Btn2, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 29, &IM_Save_Del, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 30, &IM_Save_Load, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 31, &IM_Save_Save, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 32, &IM_Crown, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 33, &IM_WinBtn0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 34, &IM_WinBtn1, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 35, &IM_Select, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 36, &IM_UnSelect, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 37, &IM_CoursePlay, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 38, &IM_CoursePause, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 39, &IM_CourseRight, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 40, &IM_CourseLeft, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 41, &IM_SettingEnter, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 42, &IM_SeekBar, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 43, &IM_SeekBarCover, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 44, &IM_SeekBarDot, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 45 + 0, &IM_Player0, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 45 + 1, &IM_AI, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 45 + 2 + 0, &IM_Developers, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 45 + 2 + 1, &IM_Music, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 45 + 2 + 2, &IM_EX_BK, NULL);
	LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 45 + 2 + 3, &IM_Welcome, NULL);
	FreeDB(&BilliardsData);
	/*加载字体设置*/
	LoadFonts(FONTS_NAME,FONTS_PATH,&FT_Num);
	LoadFonts(TITLE_FONTS_NAME,TITLE_FONTS_PATH,&FT_Title);
	LoadFonts(BUTTON_FONTS_NAME,BUTTON_FONTS_PATH,&FT_Button);
	LoadFonts(TEXT_FONTS_NAME,TEXT_FONTS_PATH,&FT_Text);
	/*加载音乐*/
	LPDATABASE SoundData;
	LoadDB("Sound.db", &SoundData);
	LoadSoundFromDB(SoundData, 0, &MU_Begin);
	LoadSoundFromDB(SoundData, 1, &MU_Pause);
	LoadSoundFromDB(SoundData, 2, &MU_Saved);
	LoadSoundFromDB(SoundData, 3, &MU_Recycle);
	LoadSoundFromDB(SoundData, 4, &MU_Collide);
	LoadSoundFromDB(SoundData, 5, &MU_Reflect);
	LoadSoundFromDB(SoundData, 6, &MU_Inhole);
	LoadSoundFromDB(SoundData, 7, &MU_OnButton);
	LoadSoundFromDB(SoundData, 8, &MU_SwitchMenu);
	LoadSoundFromDB(SoundData, 9, &MU_SwitchSide);
	LoadSoundFromDB(SoundData, 10, &MU_Quit1);
	LoadSoundFromDB(SoundData, 11, &MU_Quit2);
	LoadSoundFromDB(SoundData, 12, &MU_Close1);
	LoadSoundFromDB(SoundData, 13, &MU_Close2);
	LoadSoundFromDB(SoundData, 14, &MU_CountDown);
	LoadSoundFromDB(SoundData, 15, &MU_Win);
	FreeDB(&SoundData);
	/*BGM由音频播放窗口自行初始化*/ 
	/*加载鼠标指针图形*/
	CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
	return true;
}

bool InitFunctions()
{
	/*随机加载一套地图*/
	extern int Menu; 
	srand((unsigned)time(NULL));
	Menu = rand() % 2;
	/*页面入口*/	
	StartBeginPage();
	/*显示窗口*/
	extern HWND graphicsWindow;
	ShowWindow(graphicsWindow, SW_SHOWNORMAL);
	UpdateWindow(graphicsWindow);
	return true;
}

extern struct Billiards
{
    float Position_X;								//<需存档变量> 球坐标水平分量 X 
    float Position_Y;								//<需存档变量> 球坐标垂直分量 Y
    float Velocity_X;								//球速度水平分量 X 
    float Velocity_Y;								//球速度垂直分量 Y 
    int Moving;									//球是否在移动 
    int State;									//球是否进洞 
    int IfScored;								//球是否已经计分 
    int Score;									//球的计分情况
    LPIMAGE Address;							//球图像文件的存储路径 
} Ball[22];

extern int ifLoad;

void LoadBalls()
{
	if(GameMode == GM_POOL || GameMode == GM_TRAINNING)
	{
		Ball[0].Address = IM_PoolBilliards0;
		Ball[1].Address = IM_PoolBilliards1;
		Ball[2].Address = IM_PoolBilliards2;
		Ball[3].Address = IM_PoolBilliards3;
		Ball[4].Address = IM_PoolBilliards4;
		Ball[5].Address = IM_PoolBilliards5;
		Ball[6].Address = IM_PoolBilliards6;
		Ball[7].Address = IM_PoolBilliards7;
		Ball[8].Address = IM_PoolBilliards8;
		Ball[9].Address = IM_PoolBilliards9;
		Ball[10].Address = IM_PoolBilliards10;
		Ball[11].Address = IM_PoolBilliards11;			
		Ball[12].Address = IM_PoolBilliards12;
		Ball[13].Address = IM_PoolBilliards13;
		Ball[14].Address = IM_PoolBilliards14;
		Ball[15].Address = IM_PoolBilliards15;
		Ball[16].Address = NULL;
		Ball[17].Address = NULL;
		Ball[18].Address = NULL;
		Ball[19].Address = NULL;
		Ball[20].Address = NULL;
		Ball[21].Address = NULL;
	}
	else if(GameMode == GM_SINGLEPLAYER || GameMode == GM_MULTIPLAYER)
	{
		Ball[0].Address = IM_BilliardsEmoji0;
		Ball[1].Address = IM_BilliardsEmoji1;
		Ball[2].Address = IM_BilliardsEmoji2;
		Ball[3].Address = IM_BilliardsEmoji3;
		Ball[4].Address = IM_BilliardsEmoji4;
		Ball[5].Address = IM_BilliardsEmoji5;
		Ball[6].Address = IM_BilliardsEmoji6;
		Ball[7].Address = IM_BilliardsEmoji7;
		Ball[8].Address = IM_BilliardsEmoji8;
		Ball[9].Address = IM_BilliardsEmoji9;
		Ball[10].Address = NULL;
		Ball[11].Address = NULL;	
		Ball[12].Address = NULL;
		Ball[13].Address = NULL;
		Ball[14].Address = NULL;
		Ball[15].Address = NULL;
		Ball[16].Address = NULL;
		Ball[17].Address = NULL;
		Ball[18].Address = NULL;
		Ball[19].Address = NULL;
		Ball[20].Address = NULL;
		Ball[21].Address = NULL;
	}
	else
	{
		Ball[0].Address = IM_Billiards0;
		Ball[1].Address = IM_Billiards1;
		Ball[2].Address = IM_Billiards6;
		Ball[3].Address = IM_Billiards7;
		Ball[4].Address = IM_Billiards2;
		Ball[5].Address = IM_Billiards4;
		Ball[6].Address = IM_Billiards8;
		Ball[7].Address = IM_Billiards3;
		Ball[8].Address = IM_Billiards3;
		Ball[9].Address = IM_Billiards3;	
		Ball[10].Address = IM_Billiards3;
		Ball[11].Address = IM_Billiards3;
		Ball[12].Address = IM_Billiards3;
		Ball[13].Address = IM_Billiards3;
		Ball[14].Address = IM_Billiards3;
		Ball[15].Address = IM_Billiards3;
		Ball[16].Address = IM_Billiards3;
		Ball[17].Address = IM_Billiards3;
		Ball[18].Address = IM_Billiards3;
		Ball[19].Address = IM_Billiards3;
		Ball[20].Address = IM_Billiards3;
		Ball[21].Address = IM_Billiards3;
	}
}

extern int CueBallPosition_X;
extern int CueBallPosition_Y;

void SetBalls()
{
	if(ifLoad)
	{
		DrawAllBalls();
		return;
	}
	int i; 
	if(GameMode == GM_SINGLEPLAYER || GameMode == GM_MULTIPLAYER)
	{
		Ball[0].Position_X = 331;//WIDTH / 4;									//母球 
		Ball[0].Position_Y = 315 + TITLEHEIGHT;//HEIGHT / 2; 
		Ball[1].Position_X = WIDTH / 2;											//1号球  
		Ball[1].Position_Y = 315 + TITLEHEIGHT;
		Ball[2].Position_X = Ball[1].Position_X + (Ball_R + Gap) * 1.73;		//2号球 
		Ball[2].Position_Y = Ball[1].Position_Y + (Ball_R + Gap);
		Ball[3].Position_X = Ball[1].Position_X + (Ball_R + Gap) * 1.73;		//3号球 
		Ball[3].Position_Y = Ball[1].Position_Y - (Ball_R + Gap);
		Ball[4].Position_X = Ball[3].Position_X + (Ball_R + Gap) * 1.73;		//4号球 
		Ball[4].Position_Y = Ball[1].Position_Y + 2 * (Ball_R + Gap);
		Ball[5].Position_X = Ball[4].Position_X;								//5号球 
		Ball[5].Position_Y = Ball[1].Position_Y;					
		Ball[6].Position_X = Ball[4].Position_X;								//6号球 
		Ball[6].Position_Y = Ball[1].Position_Y - 2 * (Ball_R + Gap);
		Ball[7].Position_X = Ball[6].Position_X + (Ball_R + Gap) * 1.73;		//7号球 
		Ball[7].Position_Y = Ball[1].Position_Y + (Ball_R + Gap);
		Ball[8].Position_X = Ball[7].Position_X;								//8号球 
		Ball[8].Position_Y = Ball[1].Position_Y - (Ball_R + Gap);
		Ball[9].Position_X = Ball[8].Position_X + (Ball_R + Gap) * 1.73;		//9号球 
		Ball[9].Position_Y = Ball[1].Position_Y;
		
		//如果要测试平局情况，添加注释内的内容
		// case 1:
		/* 
		for(i = 1; i <= 8; i ++)
		{
			if(i % 2 != 0)
			{
				Ball[i].Position_X = 33;
				Ball[i].Position_Y = Shelf_Y[Classic.Player1Scored ++];
				Classic.Player1LastScored = i;				
			}
			else
			{
				Ball[i].Position_X = 1247;
				Ball[i].Position_Y = Shelf_Y[Classic.Player2Scored ++];	
				Classic.Player2LastScored = i;	
			}
			Ball[i].State = INHOLE;
		}
		Classic.ScorePlayer1 = Classic.ScorePlayer2 = 10;
		Classic.AgainScore2 = 2;
		*/
		
		// case 2:
		/*
		for(i = 1; i <= 8; i ++)
		{
			Ball[i].Position_X = 1247;
			Ball[i].Position_Y = Shelf_Y[Classic.Player2Scored ++];
			Classic.Player2LastScored = i;				
			Ball[i].State = INHOLE;
		}
		Classic.ScorePlayer2 = 16;
		Classic.AgainScore1 = 18;
		Classic.ScorePlayer1 = 18;
		*/
	}
	else if(GameMode == GM_POOL || GameMode == GM_TRAINNING)
	{
		int i, temp1, temp2, temp;
		int List[15] = {1, 2, 3, 4, 8, 6, 7, 5, 9, 10, 11, 12, 13, 14, 15};
		float Position_X[16] = {0};
		float Position_Y[16] = {0};
   		i = 40;									//随机交换四十次 
		while(i --)
    	{
    		temp1 = rand() % 15;
    		temp2 = rand() % 15;
    		if(temp1 == 4 || temp2 == 4)
    			continue;
    		temp = List[temp1];
    		List[temp1] = List[temp2];
    		List[temp2] = temp;
    	}
		Ball[0].Position_X = 331;									
		Ball[0].Position_Y = 315 + TITLEHEIGHT;
		Position_X[1] = WIDTH / 7 * 5;								
		Position_Y[1] = 315 + TITLEHEIGHT;
		Position_X[2] = Position_X[1] + (Ball_R + Gap) * 1.73;	
		Position_Y[2] = Position_Y[1] + (Ball_R + Gap);
		Position_X[3] = Position_X[1] + (Ball_R + Gap) * 1.73;	
		Position_Y[3] = Position_Y[1] - (Ball_R + Gap);
		Position_X[4] = Position_X[3] + (Ball_R + Gap) * 1.73;	
		Position_Y[4] = Position_Y[1] + 2 * (Ball_R + Gap);
		Position_X[5] = Position_X[4];						
		Position_Y[5] = Position_Y[1];					
		Position_X[6] = Position_X[4];							
		Position_Y[6] = Position_Y[1] - 2 * (Ball_R + Gap);
		Position_X[7] = Position_X[6] + (Ball_R + Gap) * 1.73;	 
		Position_Y[7] = Position_Y[4] + (Ball_R + Gap);
		Position_X[8] = Position_X[7];							
		Position_Y[8] = Position_Y[5] + (Ball_R + Gap);
		Position_X[9] = Position_X[7];
		Position_Y[9] = Position_Y[6] + (Ball_R + Gap);
		Position_X[10] = Position_X[7];
		Position_Y[10] = Position_Y[6] - (Ball_R + Gap);
		Position_X[11] = Position_X[7] + (Ball_R + Gap) * 1.73;
		Position_Y[11] = Position_Y[7] + (Ball_R + Gap);
		Position_X[12] = Position_X[11];
		Position_Y[12] = Position_Y[8] + (Ball_R + Gap);
		Position_X[13] = Position_X[11];
		Position_Y[13] = Position_Y[9] + (Ball_R + Gap);
		Position_X[14] = Position_X[11];
		Position_Y[14] = Position_Y[10] + (Ball_R + Gap);
		Position_X[15] = Position_X[11];
		Position_Y[15] = Position_Y[10] - (Ball_R + Gap);
		for(i = 1; i < 16; i ++)
		{
			Ball[List[i - 1]].Position_X = Position_X[i];
			Ball[List[i - 1]].Position_Y = Position_Y[i];
		}
	}
	else
	{
		int i;
		Ball[0].Position_X = 280;									
		Ball[0].Position_Y = 275 + TITLEHEIGHT;
		float Position_X[16] = {0};
		float Position_Y[16] = {0};
		Position_X[1] = WIDTH / 9 * 6;								
		Position_Y[1] = 315 + TITLEHEIGHT;
		Position_X[2] = Position_X[1] + (Ball_R + Gap) * 1.73;	
		Position_Y[2] = Position_Y[1] + (Ball_R + Gap);
		Position_X[3] = Position_X[1] + (Ball_R + Gap) * 1.73;	
		Position_Y[3] = Position_Y[1] - (Ball_R + Gap);
		Position_X[4] = Position_X[3] + (Ball_R + Gap) * 1.73;	
		Position_Y[4] = Position_Y[1] + 2 * (Ball_R + Gap);
		Position_X[5] = Position_X[4];						
		Position_Y[5] = Position_Y[1];					
		Position_X[6] = Position_X[4];							
		Position_Y[6] = Position_Y[1] - 2 * (Ball_R + Gap);
		Position_X[7] = Position_X[6] + (Ball_R + Gap) * 1.73;	 
		Position_Y[7] = Position_Y[4] + (Ball_R + Gap);
		Position_X[8] = Position_X[7];							
		Position_Y[8] = Position_Y[5] + (Ball_R + Gap);
		Position_X[9] = Position_X[7];
		Position_Y[9] = Position_Y[6] + (Ball_R + Gap);
		Position_X[10] = Position_X[7];
		Position_Y[10] = Position_Y[6] - (Ball_R + Gap);
		Position_X[11] = Position_X[7] + (Ball_R + Gap) * 1.73;
		Position_Y[11] = Position_Y[7] + (Ball_R + Gap);
		Position_X[12] = Position_X[11];
		Position_Y[12] = Position_Y[8] + (Ball_R + Gap);
		Position_X[13] = Position_X[11];
		Position_Y[13] = Position_Y[9] + (Ball_R + Gap);
		Position_X[14] = Position_X[11];
		Position_Y[14] = Position_Y[10] + (Ball_R + Gap);
		Position_X[15] = Position_X[11];
		Position_Y[15] = Position_Y[10] - (Ball_R + Gap);
		for(i = 7; i < 22; i ++)
		{
			Ball[i].Position_X = Position_X[i - 6];
			Ball[i].Position_Y = Position_Y[i - 6];
		}
		Ball[6].Position_X = WIDTH / 6 * 5;								
		Ball[6].Position_Y = 315 + TITLEHEIGHT;
		Ball[5].Position_X = WIDTH / 9 * 6 - 4 * Ball_R;								
		Ball[5].Position_Y = 315 + TITLEHEIGHT;
		Ball[1].Position_X = Ball[2].Position_X = Ball[3].Position_X = 331;
		Ball[4].Position_X = WIDTH / 2;
		Ball[1].Position_Y = Ball[4].Position_Y = 315 + TITLEHEIGHT;
		Ball[2].Position_Y = 226 + TITLEHEIGHT;
		Ball[3].Position_Y = 406 + TITLEHEIGHT;	
		
		
		
		
		/*
		for(i = 7; i <= 21; i ++)
		{
			Ball[i].Position_X = - 10 * Ball_R;
			Ball[i].Position_Y = - Ball_R - i * Ball_R;
			Ball[i].State = INHOLE;
		}
		Snooker.Player1Score = 10;
		Snooker.Player2Score = 17;
		Snooker.Skip = 1;
		*/
		
		
		
		
	}
	//Hereitis
	if(GameMode == GM_SNOOKER || GameMode == GM_POOL)
	{
		Ball[0].Velocity_X = 0;
		Ball[0].Velocity_Y = 0;
		Ball[0].State = INHOLE;
		Ball[0].Position_X = 2560;
		Ball[0].Position_Y = 1440;
		WhichHole[0] = 0;
		InHoleFinished[0] = true;
	}
	else;
	CueBallPosition_X = Ball[0].Position_X;
	CueBallPosition_Y = Ball[0].Position_Y;
	DrawAllBalls();	
}

