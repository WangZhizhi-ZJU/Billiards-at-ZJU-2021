/* 浙江大学《程序设计专题》求是台球设计小组  献上 */
/*
    如何阅读本程序： 
    ·如果您是测试者，那么在编译后，将会自动向您弹出一份说明报告（这份报告也可以在主目录下直接打开“请阅读我.txt”）； 
      如果您还没有阅读过这份报告，强烈建议您先阅读之后再阅读我们的源代码。 
    ·有关评分的所有功能的源代码分布在以下头文件里：
	  1. 游戏界面与图形的绘制：“gui.c”、“gui.h”
      2. 游戏数据初始化：“initial.c”、“initial.h”
      3. 游戏交互响应控制（回调函数）：“callback.c”、“callback.h”
      4. 游戏物理模型、游戏规则控制：“physics.c”、“physics.h”
      5. 为物理模型辅助的数学计算：“vector.c”、“vector.h”
      6. 经典模式的人工智能控制模块：“ai.c”、“ai.h”
      7. 游戏存档、数据、素材等文件的存取：“openfile.c”、“openfile.h”
      8. 游戏关键数据的读取和定义：“set.c”、“set.h”
      9. 游戏经验控制模块：“exp.c”、“exp.h”
    ·实际呈现比代码更直观，赶紧按下“F12”（或者是“F11”）来体验《求是台球》。 
*/ 

#include <gui.h>
#include <text.h> 
#include <stdio.h>
#include <media.h>
#include <physics.h>
#include <pictures.h>
#include <graphics.h>
#include <extgraph.h> 

#include <sets.h>
#include <public.h>
#include <callback.h>

#include <user.h>
#include <timesys.h>
#include <tooltips.h>
#include <newwindow.h>

#include <test.h>

//玩家设置系数
float ForceBarSpeed = 40;						//0 ~ 100, 对应步长1.5 ~ 4, 可以修改 
float ForceBarStep = 1.5;
int RefreshRate = 13;							//刷新频率, 30Hz 设为 33，60 Hz设为 15， 75 Hz设为 13， 
												//120 Hz设为 8， 144 Hz设为 7， 300 Hz设为 3 
/* 全局变量定义 */
double X = 0, Y = 0; 							//屏幕尺寸X, Y 

//游戏内部系数 
MODE GameMode = GM_SNOOKER;
double K_Friction = 0.990;				//阻尼效果系数 
float const MaxVelocity = 45;
float Acceleration_Max = 3;						//高低杆最大加速度 
float SwirlTheta_Max = pi / 20;					//加塞最大偏转角 
float ReflectTheta_Max = pi / 10;				//加塞反射最大偏转角 
float CueTheta = 0;								//球杆角度，决定了是否会造成旋转 
float ImpactOffset = 0.3;						//高低杆球碰撞补偿 
float Power = 0;
float K_Attenuation = 0.9;
float K_Swift = 0;
float K_MoveOut = 18 * 1.414;
int S_Player1_ID = 0;
int S_Player2_ID = 0;
int NUM = 10; 
int Sides = 0;									//<需存档变量> 对战模式 - 回合数（用于判断属于谁的回合） 
float const ShelfGap = 39.5;
float Shelf_Y[15] = {0};
float FixConst = 0;
VECTOR DirectionVec;							//方向向量 

int Violation = 0;								//犯规判定 

int Menu = 0;
int ifLoad = 0;									//是否已经读取 
int LoadState = 0;								//读取前状态 

struct Billiards
{
    float Position_X;							//<需存档变量> 球坐标水平分量 X 
    float Position_Y;							//<需存档变量> 球坐标垂直分量 Y
    float Velocity_X;							//球速度水平分量 X 
    float Velocity_Y;							//球速度垂直分量 Y 
    int Moving;									//球是否在移动 
    int State;									//球是否进洞 
    int IfScored;								//球是否已经计分 
    int Score;									//球的计分情况
    LPIMAGE Address;						   //球图像文件的存储路径 
} Ball[22];

struct PoolInformation
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

struct ClassicInfomation
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

struct TrainningInformation
{
	int Score;
	int NewInhole[16];
	int IfScored[16];
} Trainning;

class SnookerInfomation
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
 
struct Acceleration
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

float RealHole_X[6] = {Hole1_X - 23, Hole2_X, Hole3_X + 23, Hole4_X - 23, Hole5_X, Hole6_X + 23};
float RealHole_Y[6] = {Hole1_Y - 21, Hole2_Y - 36, Hole3_Y - 21, Hole4_Y + 21, Hole5_Y + 36, Hole6_Y + 21};
float Hole_X[6] = {Hole1_X, Hole2_X, Hole3_X, Hole4_X, Hole5_X, Hole6_X};
float Hole_Y[6] = {Hole1_Y, Hole2_Y, Hole3_Y, Hole4_Y, Hole5_Y, Hole6_Y};
bool InHoleFinished[22] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
float Draw_R[22] = {16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0};
int WhichHole[22] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int ViolationShelf_X[2][5] = {{505, 524, 543, 562, 581}, {757, 738, 719, 700, 681}};

/*游戏时间系统*/
LPTIMESYS GameTimeSys;

/*图片资源*/
LPIMAGE IM_Billiards0,
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
		/*菜单*/ 
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
		/*标题栏*/
		IM_Close_0,
		IM_Close_1,
		IM_Close_2,
		IM_Maximize_0,
		IM_Minimize_0,
		IM_Minimize_1,
		IM_Minimize_2,
		IM_Info_0,
		IM_Info_1,
		IM_Info_2,
		IM_Setting_0,
		IM_Setting_1,
		IM_Setting_2,
		IM_Game_0,
		IM_Game_1,
		IM_Game_2,
		IM_Split,
		IM_Icon,
		/*提示*/
		IM_Violation,
		IM_Target,
		IM_Colour,
		IM_BlackSide,
		IM_SolidSide,
		IM_StripSide,
		/*游戏模式*/
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
		/*球杆素材*/
		IM_Stick,
		IM_StickArr[9][2],
		/*球桌*/
		IM_Table,
		IM_ForceBar,
		IM_RedPoint,
		/*菜单*/
		IM_LoadPage,
		IM_LoadBar,
		IM_Masks_0,
		IM_Masks_1,
		IM_Choice,
		IM_Begin_0,
		IM_Begin_1,
		IM_Begin_2,
		IM_LoadSave_1,
		IM_LoadSave_2,
		IM_LoadSave_3,
		IM_LoadSave_4,
		IM_LoadSave_5,
		IM_Register,
		IM_CourseBK0,
		IM_CourseBK1,
		IM_CourseBK2,
		IM_CourseBK3,
		IM_SettingBK,
		/*按钮*/
		IM_GreyBar,
		IM_WhiteBar,
		IM_Frame,
		IM_StartGame_0,
		IM_StartGame_1,
		IM_GM_BK_0_0,
		IM_GM_BK_1_0,
		IM_GM_BT_0_0,
		IM_GM_Esc_0,
		IM_Pause_0,
		IM_Pause_Continue,
		IM_Pause_Save,
		IM_Pause_Quit,
		IM_SetTheta,
		IM_ExpBar_0,
		IM_ExpBar_1,
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
		IM_Select,
		IM_UnSelect,
		IM_CoursePlay,
		IM_CoursePause,
		IM_CourseRight,
		IM_CourseLeft,
		IM_SettingEnter,
		IM_SeekBar,
		IM_SeekBarCover,
		IM_SeekBarDot,
		/*内置头像*/
		IM_Player0, 
		IM_AI,
		/*开发者相关*/
		IM_Developers,
		IM_Music,
		IM_EX_BK,
		IM_Welcome;

/*字体资源*/
LPFONTS FT_Num,
	    FT_Title,
	    FT_Button,
	    FT_Text; 

/*音频资源*/
LPMUSIC MU_Begin,
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

/*玩家信息*/
LPUSER User1 = NULL, User2 = NULL;

/*光标资源*/
HCURSOR CS_Cursor; 

/*文件版本信息*/
char IF_FileTitle[100];
char* IF_FileVersion;

/*应用路径*/
char IF_FilePath[MAX_PATH];

/*提示信息*/
LPTOOLTIPS TT_Title,
		   TT_Client;

void Main(){
	//WriteBilliardsDB("Billiards.db");
	//WriteMusicDB("Sound.db");
	
	/*初始化图形库*/ 
	//InitConsole();
	InitGraphics();
	SetWindowTitle("Billiards");
	X = GetWindowWidth();
	Y = GetWindowHeight();
	printf("WindowWidth: %f\nWindowHeight: %f\n", X, Y);
	/*欢迎窗口*/
	WelcomeWindow();
	/*初始化游戏界面*/ 
	InitSets();
	InitUsers();
	InitSources();
	InitCallback(); 
	InitWndTitle();
	InitFunctions();
	InitMusicWindow();
	return;
}

