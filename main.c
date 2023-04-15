/* �㽭��ѧ���������ר�⡷����̨�����С��  ���� */
/*
    ����Ķ������� 
    ��������ǲ����ߣ���ô�ڱ���󣬽����Զ���������һ��˵�����棨��ݱ���Ҳ��������Ŀ¼��ֱ�Ӵ򿪡����Ķ���.txt������ 
      �������û���Ķ�����ݱ��棬ǿ�ҽ��������Ķ�֮�����Ķ����ǵ�Դ���롣 
    ���й����ֵ����й��ܵ�Դ����ֲ�������ͷ�ļ��
	  1. ��Ϸ������ͼ�εĻ��ƣ���gui.c������gui.h��
      2. ��Ϸ���ݳ�ʼ������initial.c������initial.h��
      3. ��Ϸ������Ӧ���ƣ��ص�����������callback.c������callback.h��
      4. ��Ϸ����ģ�͡���Ϸ������ƣ���physics.c������physics.h��
      5. Ϊ����ģ�͸�������ѧ���㣺��vector.c������vector.h��
      6. ����ģʽ���˹����ܿ���ģ�飺��ai.c������ai.h��
      7. ��Ϸ�浵�����ݡ��زĵ��ļ��Ĵ�ȡ����openfile.c������openfile.h��
      8. ��Ϸ�ؼ����ݵĶ�ȡ�Ͷ��壺��set.c������set.h��
      9. ��Ϸ�������ģ�飺��exp.c������exp.h��
    ��ʵ�ʳ��ֱȴ����ֱ�ۣ��Ͻ����¡�F12���������ǡ�F11���������顶����̨�򡷡� 
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

//�������ϵ��
float ForceBarSpeed = 40;						//0 ~ 100, ��Ӧ����1.5 ~ 4, �����޸� 
float ForceBarStep = 1.5;
int RefreshRate = 13;							//ˢ��Ƶ��, 30Hz ��Ϊ 33��60 Hz��Ϊ 15�� 75 Hz��Ϊ 13�� 
												//120 Hz��Ϊ 8�� 144 Hz��Ϊ 7�� 300 Hz��Ϊ 3 
/* ȫ�ֱ������� */
double X = 0, Y = 0; 							//��Ļ�ߴ�X, Y 

//��Ϸ�ڲ�ϵ�� 
MODE GameMode = GM_SNOOKER;
double K_Friction = 0.990;				//����Ч��ϵ�� 
float const MaxVelocity = 45;
float Acceleration_Max = 3;						//�ߵ͸������ٶ� 
float SwirlTheta_Max = pi / 20;					//�������ƫת�� 
float ReflectTheta_Max = pi / 10;				//�����������ƫת�� 
float CueTheta = 0;								//��˽Ƕȣ��������Ƿ�������ת 
float ImpactOffset = 0.3;						//�ߵ͸�����ײ���� 
float Power = 0;
float K_Attenuation = 0.9;
float K_Swift = 0;
float K_MoveOut = 18 * 1.414;
int S_Player1_ID = 0;
int S_Player2_ID = 0;
int NUM = 10; 
int Sides = 0;									//<��浵����> ��սģʽ - �غ����������ж�����˭�Ļغϣ� 
float const ShelfGap = 39.5;
float Shelf_Y[15] = {0};
float FixConst = 0;
VECTOR DirectionVec;							//�������� 

int Violation = 0;								//�����ж� 

int Menu = 0;
int ifLoad = 0;									//�Ƿ��Ѿ���ȡ 
int LoadState = 0;								//��ȡǰ״̬ 

struct Billiards
{
    float Position_X;							//<��浵����> ������ˮƽ���� X 
    float Position_Y;							//<��浵����> �����괹ֱ���� Y
    float Velocity_X;							//���ٶ�ˮƽ���� X 
    float Velocity_Y;							//���ٶȴ�ֱ���� Y 
    int Moving;									//���Ƿ����ƶ� 
    int State;									//���Ƿ���� 
    int IfScored;								//���Ƿ��Ѿ��Ʒ� 
    int Score;									//��ļƷ����
    LPIMAGE Address;						   //��ͼ���ļ��Ĵ洢·�� 
} Ball[22];

struct PoolInformation
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

/*��Ϸʱ��ϵͳ*/
LPTIMESYS GameTimeSys;

/*ͼƬ��Դ*/
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
		/*�˵�*/ 
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
		/*������*/
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
		/*��ʾ*/
		IM_Violation,
		IM_Target,
		IM_Colour,
		IM_BlackSide,
		IM_SolidSide,
		IM_StripSide,
		/*��Ϸģʽ*/
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
		/*����ز�*/
		IM_Stick,
		IM_StickArr[9][2],
		/*����*/
		IM_Table,
		IM_ForceBar,
		IM_RedPoint,
		/*�˵�*/
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
		/*��ť*/
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
		/*����ͷ��*/
		IM_Player0, 
		IM_AI,
		/*���������*/
		IM_Developers,
		IM_Music,
		IM_EX_BK,
		IM_Welcome;

/*������Դ*/
LPFONTS FT_Num,
	    FT_Title,
	    FT_Button,
	    FT_Text; 

/*��Ƶ��Դ*/
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

/*�����Ϣ*/
LPUSER User1 = NULL, User2 = NULL;

/*�����Դ*/
HCURSOR CS_Cursor; 

/*�ļ��汾��Ϣ*/
char IF_FileTitle[100];
char* IF_FileVersion;

/*Ӧ��·��*/
char IF_FilePath[MAX_PATH];

/*��ʾ��Ϣ*/
LPTOOLTIPS TT_Title,
		   TT_Client;

void Main(){
	//WriteBilliardsDB("Billiards.db");
	//WriteMusicDB("Sound.db");
	
	/*��ʼ��ͼ�ο�*/ 
	//InitConsole();
	InitGraphics();
	SetWindowTitle("Billiards");
	X = GetWindowWidth();
	Y = GetWindowHeight();
	printf("WindowWidth: %f\nWindowHeight: %f\n", X, Y);
	/*��ӭ����*/
	WelcomeWindow();
	/*��ʼ����Ϸ����*/ 
	InitSets();
	InitUsers();
	InitSources();
	InitCallback(); 
	InitWndTitle();
	InitFunctions();
	InitMusicWindow();
	return;
}

