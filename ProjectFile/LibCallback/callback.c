#include <graphics.h>
#include <windows.h>
#include <vector.h>
#include <physics.h>
#include <stdio.h>

#include <gui.h>
#include <sets.h>
#include <user.h>
#include <save.h>
#include <public.h>
#include <timesys.h>
#include <pictures.h>
#include <callback.h>

#include <course.h>
#include <setting.h>

extern HCURSOR CS_Cursor; 
extern HWND graphicsWindow;
extern int NUM;
extern int Menu;
extern int ifLoad;
extern MODE GameMode;
 
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

/*游戏时间系统*/
extern LPTIMESYS GameTimeSys; 

/*应用路径*/
extern char IF_FilePath[MAX_PATH];

/*状态机*/
MOUSESTATE MOUSE_STATE = MS_NULL;
KEYBOARDSTATE KEYBOARD_STATE = KB_NULL;

/*客户区控件*/ 
int BT_button=0,BT_update=0;
int BT_ST_start=0,BT_ST_load=0,BT_ST_exit=0,BT_ST_player1=0,BT_ST_player2=0; //StartMenu
int BT_GM_back=0,BT_GM_mode1=0,BT_GM_mode2=0,BT_GM_mode3=0,BT_GM_mode4=0,BT_GM_mode5=0; //GameMode
int BT_LS_back=0,BT_LS_save1=0,BT_LS_save2=0,BT_LS_save3=0,BT_LS_save4=0,
	BT_LS_page1=0,BT_LS_page2=0,BT_LS_page3=0,BT_LS_page4=0,BT_LS_page5=0,
	BT_LS_sav1=0,BT_LS_del1=0,BT_LS_sav2=0,BT_LS_del2=0,BT_LS_sav3=0,BT_LS_del3=0,BT_LS_sav4=0,BT_LS_del4=0; //LoadSave
int BT_Pause_Continue=0,BT_Pause_Save=0,BT_Pause_Quit=0; //Pause
int BT_REG_avatar=0,BT_REG_text=0,BT_REG_ok=0,BT_REG_back=0; //Register
int BT_USER_avatar=0,BT_USER_text=0,BT_USER_background=0,BT_USER_back=0,BT_USER_def=0; //Users
int BT_Win_Back=0,BT_Win_Save=0; //Win
int BT_Stick_back=0,BT_Stick_0=0,BT_Stick_1=0,BT_Stick_2=0,BT_Stick_3=0,
	BT_Stick_4=0,BT_Stick_5=0,BT_Stick_6=0,BT_Stick_7=0,BT_Stick_8=0; //ChooseStick 

/*标题栏控件*/ 
int TBT_minimize=0,TBT_close=0,TBT_info=0,TBT_setting=0,TBT_game=0,TBT_button=0,TBT_update=0,TBT_move=0;

/*清除所有定时器*/
bool EndFlag = false;

void TimeCallback(int id){
	if(EndFlag && id != TM_END_SHADES){
		cancelTimer(id);
		return;
	}
	switch(id){
		case TM_FORCEBAR_CHANGE:
			DrawBKGround();
			DrawForceBar();
			UpdateHitPos();
			TargetPoints();
			DrawAllBalls();
			UpdtStickBar();
			DrawWndTitle();
			DrawHintMsge();
		break;
		case TM_HIT_POSITION_CHANGE:
			DrawBKGround();
			UpdateHitPos();
			TargetPoints();
			DrawAllBalls();
			DrawStickBar();
			DrawThetaBar();
			DrawWndTitle();
			DrawHintMsge();
		break;
		case TM_START_HIT:
			OverallDraw();
			DrawBKGround();
			UpdtForceBar();
			UpdateHitPos();
			DrawAllBalls();
			DrawHintMsge();
		break;
		case TM_FREE_RESET_CUEBALL:
			DrawBKGround();
			DrawAllBalls();
			UpdateHitPos();
			DrawACueBall();
			DrawHintMsge();
		break;
		case TM_SHAKE_START_MENU:
			RsetShakeMnu();
		break;
		case TM_SHAKE:
			AddShakePage();
			DrawShakeMnu();
			DrawStartBtn();
		break;
		case TM_LOAD_PAGE:
			AddLoadPages();
			DrawLoadPage();
		break;
		case TM_LOAD_TEXT:
			AddLoadTexts();
		break;
		case TM_SHADES:
			AddMaskPages();
			DrawBlkMasks();
		break;
		case TM_END_SHADES:
			AddMaskPages();
			DrawBlkMasks();
		break;
		case TM_BEGIN_PAGE:
			AddBeginPage();
			DrawBeginPag();
		break;
		case TM_CHOOSE_GAMEMODE:
			cancelTimer(TM_CHOOSE_GAMEMODE);
			DrawGmodeBtn();
		break;
		case TM_PAUSE_MENU:
			cancelTimer(TM_PAUSE_MENU);
			DrawPauseBtn();
		break;
		case TM_PAUSE_MENU_CD:
			cancelTimer(TM_PAUSE_MENU_CD);
			EnableCDTime();
		break;
		case TM_LOAD_SAVE:
			cancelTimer(TM_LOAD_SAVE);
			DrawLdSavBtn();
		break;
		case TM_GET_THETA:
			HideThetaBar();
		break;
		case TM_START_SIDE:
			//if(GameMode == GM_TRAINNING) break;
			OriginalSide();
		break; 
		case TM_REGISTER:
			cancelTimer(TM_REGISTER); 
			DrawRegstBtn();
		break; 
		case TM_AI_WAIT:
			cancelTimer(TM_AI_WAIT);	
			cancelTimer(TM_HIT_POSITION_CHANGE);
			EndCountDown();
			SetPrepareMode(1);
			StartPrepare();
		break;
		case TM_AI_SET_CUEBALL:
			cancelTimer(TM_AI_SET_CUEBALL);
			AIFreeResetCueBall();
			startTimer(TM_AI_PLAY,2000);
		break;
		case TM_AI_PLAY:
			cancelTimer(TM_AI_PLAY);
			AI_Play();
			startTimer(TM_AI_WAIT,2000);
		break;
		case TM_USER:
			cancelTimer(TM_USER); 
			DrawUsersBtn();
		break;
		case TM_PREPARE_TO_HIT:
			DrawBKGround();
			UpdtForceBar();
			UpdateHitPos();
			DrawAllBalls();
			PrepareStick();
			DrawWndTitle();
		break;
		case TM_WIN:
			cancelTimer(TM_WIN);
			DrawWinerBtn();
		break;
		case TM_CHOOSE_STICK:
			cancelTimer(TM_CHOOSE_STICK);
			DrawStickBtn();
		break;
		case TM_HINT:
			cancelTimer(TM_HINT);
			extern bool f_PlayMsg;
			f_PlayMsg=false;
		break;
	}
}

float const DirectionBall_X = 639;
float const DirectionBall_Y = 665 + TITLEHEIGHT;
float const DirectionBall_R = 37;
extern int const Dot_R;
extern bool InHoleFinished[16];
static int MouseButtonDown = 0;
extern int HitPosition_X;
extern int HitPosition_Y;
extern int CurrentPosition_X;
extern int CurrentPosition_Y;
extern int CueBallPosition_X;
extern int CueBallPosition_Y;
extern struct Billiards
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

int CourseAndSetting=0; 

#define SC_DRAGMOVE HTCAPTION|SC_MOVE

void MouseCallback(int x, int y, int button, int event){
	/*文本框*/
	if(InitCheck() && y > TITLEHEIGHT)  
		TextBoxMouse(x,y,button,event);
	/*标题栏区域判断*/
{
	if((x>1238 && x<1266 
	 || x>1200 && x<1228 
	 || x>1162 && x<1190 
	 || x>1104 && x<1132 && CourseAndSetting
	 || x>1066 && x<1094 && CourseAndSetting
	 || x>1028 && x<1056 && CourseAndSetting)
	 && y>13 && y<41){
	 	if(button == LEFT_BUTTON && event == BUTTON_UP) TBT_button=0;
		DestroyCursor(CS_Cursor);
		CS_Cursor = LoadCursor(NULL, IDC_HAND);
		SetCursor(CS_Cursor);
		TBT_minimize=0,TBT_close=0,TBT_info=0,TBT_setting=0,TBT_game=0,TBT_update=1;
		if(x>1238 && x<1266){
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				TBT_button=1;
			if(!TBT_button) TBT_close=1; else TBT_close=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				ResetCursors();
				ExitWindow();//关闭确认 
				TBT_close=0;
			}
		}
		else if(x>1200 && x<1228){
			DestroyCursor(CS_Cursor);
			CS_Cursor = LoadCursor(NULL, IDC_NO);
			SetCursor(CS_Cursor);
		}
		else if(x>1162 && x<1190){
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				TBT_button=1;
			if(!TBT_button) TBT_minimize=1; else TBT_minimize=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				PostMessage(graphicsWindow,WM_SYSCOMMAND,SC_MINIMIZE,0);//最小化
				TBT_minimize=0;
			}
		}
		else if(x>1104 && x<1132){
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				TBT_button=1;
			if(!TBT_button) TBT_info=1; else TBT_info=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				extern LPIMAGE IM_Developers;
				LoadPictureOutB(IM_Developers,"AboutWindow","关于我们",640,360);// 关于 
				TBT_info=0;
			}
		}
		else if(x>1066 && x<1094){
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				TBT_button=1;
			if(!TBT_button) TBT_setting=1; else TBT_setting=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				// 设置 
				TBT_setting=0;
				DrawWndTitle();
				ResetCursors();
				if(MOUSE_STATE == MS_GET_FORCE
				|| MOUSE_STATE == MS_GET_HIT_POSITION
				|| MOUSE_STATE == MS_FREE_RESET_CUEBALL
				|| MOUSE_STATE == MS_PREPARE_TO_HIT
				|| MOUSE_STATE == MS_START_HIT){
					PostMessage(graphicsWindow,WM_KEYDOWN,VK_ESCAPE,KEY_DOWN);
					PostMessage(graphicsWindow,WM_KEYDOWN,VK_ESCAPE,KEY_UP);
				}
				ShowSettingWindow();
			}
		}
		else if(x>1028 && x<1056){
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				TBT_button=1;
			if(!TBT_button) TBT_game=1; else TBT_game=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				// 游戏教程
				TBT_game=0;
				DrawWndTitle();
				ResetCursors();
				if(MOUSE_STATE == MS_GET_FORCE
				|| MOUSE_STATE == MS_GET_HIT_POSITION
				|| MOUSE_STATE == MS_FREE_RESET_CUEBALL
				|| MOUSE_STATE == MS_PREPARE_TO_HIT
				|| MOUSE_STATE == MS_START_HIT){
					PostMessage(graphicsWindow,WM_KEYDOWN,VK_ESCAPE,KEY_DOWN);
					PostMessage(graphicsWindow,WM_KEYDOWN,VK_ESCAPE,KEY_UP);
				}
				ShowCourseWindow();
			}
		}
		DrawWndTitle();
	}else if(TBT_update){
		ResetCursors();
		TBT_minimize=0,TBT_close=0,TBT_info=0,TBT_setting=0,TBT_game=0;
		DrawWndTitle();
		TBT_update=0;
	}
	if(x>0 && x<1155 && y>13 && y<53 && !TBT_button){
		if(CourseAndSetting){
			if(x>0 && x<1028){
				if(button == LEFT_BUTTON && event == BUTTON_DOWN)
					TBT_move=1;
				if(TBT_move){
					PostMessage(graphicsWindow,WM_SYSCOMMAND,SC_DRAGMOVE,0);
					TBT_move=0;
				}
			}else;
		}else{
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				TBT_move=1;
			if(TBT_move){
				PostMessage(graphicsWindow,WM_SYSCOMMAND,SC_DRAGMOVE,0);
				TBT_move=0;
			}
		}
	}
}
	/*客户区域判断*/ 
	switch(MOUSE_STATE){
		case MS_GET_FORCE:
		{/*力度获取*/
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				EndGetPower();
				PowerControl();
				GetSwirl(HitPosition_X, HitPosition_Y);
				InHoleFinished[0] = false;
				HideHintMsge();
				StartPrepare();
			}else if(button == RIGHT_BUTTON && event == BUTTON_DOWN) {
				EndGetPower();
				StartGetHit();
			}
		}
		break;
		case MS_GET_HIT_POSITION:
		{/*击球点获取*/
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
			{
				if(Distance(x, y, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R)
				{
					MouseButtonDown = 1;
					HitPosition_X = x;
					HitPosition_Y = y;
				}
				else if(x>LeftBroad && x<RightBroad && y>UpBroad && y<DownBroad)
				{
					CurrentPosition_X = x;
					CurrentPosition_Y = y;
					EndGetHit();
					StartGetPower();
				}
			}
			if(button == LEFT_BUTTON && event == BUTTON_UP)
			{
				MouseButtonDown = 0;
			}
			if(MouseButtonDown)
				if(Distance(x, y, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R)
				{
					HitPosition_X = x;
					HitPosition_Y = y;
				}
				else
				{
					VECTOR Direction = {x - DirectionBall_X, y - DirectionBall_Y};
					UnitVec(Direction);
					HitPosition_X = DirectionBall_X + (DirectionBall_R - 6) * Direction[0];
					HitPosition_Y = DirectionBall_Y + (DirectionBall_R - 6) * Direction[1];
				}
			if(event == MOUSEMOVE)
			{
				if(x>LeftBroad && x<RightBroad && y>UpBroad && y<DownBroad)
				{
					if(CS_Cursor){
						DestroyCursor(CS_Cursor);
						CS_Cursor = NULL;
						SetCursor(CS_Cursor);
					}
					CurrentPosition_X = x;
					CurrentPosition_Y = y;
				}
				else{
					if(!CS_Cursor)
						ResetCursors();
				}
			}
			if(button == MIDDLE_BUTTON)
			{
				switch(event)
				{
					case ROLL_UP:
						AddTheta();
					break;
					case ROLL_DOWN:
						CutTheta();
					break;
				}
			}
		}
		break;
		case MS_FREE_RESET_CUEBALL:
		{/*自由放置母球*/ 
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
			{
				int i = 0;
				float Distance0;
				//先判断重置位置的合法性
				if(GameMode != GM_SNOOKER && GameMode != GM_POOL)
				{
					if(x + Ball_R <= RightBroad && x - Ball_R >= LeftBroad && y + Ball_R <= DownBroad && y - Ball_R >= UpBroad)
					{
						for(i = 0; i < NUM; i ++)
						{
							Distance0 = Distance(x, y, Ball[i].Position_X, Ball[i].Position_Y);
							if(Distance0 <= 2 * Ball_R)
							{
																	//返回放置位置违法提示
								break; 
							}
						}
						if(i >= NUM)
						{
							Ball[0].Moving = STOP;
							Ball[0].State = ONTABLE;
							Ball[0].Position_X = x;
							Ball[0].Position_Y = y;
							EndFreeResetCueBall();
							StartGetHit();
							//设置出口	
						}
						else;										//不合法位置重新调用 
					} 
					else;											//不合法位置重新调用					
				}
				else if(GameMode == GM_SNOOKER)
				{
					if(CueBallPosition_X + Ball_R <= 331 && (Distance(CueBallPosition_X, CueBallPosition_Y, 331, 317 + TITLEHEIGHT) <= 90 - Ball_R) &&
					   x + Ball_R <= RightBroad && x - Ball_R >= LeftBroad && y + Ball_R <= DownBroad && y - Ball_R >= UpBroad)
					{
						for(i = 0; i < NUM; i ++)
						{
							Distance0 = Distance(CueBallPosition_X, CueBallPosition_Y, Ball[i].Position_X, Ball[i].Position_Y);
							if(Distance0 <= 2 * Ball_R)
							{
																	//返回放置位置违法提示
								break; 
							}
						}
						if(i >= NUM)
						{
							Ball[0].Moving = STOP;
							Ball[0].State = ONTABLE;
							Ball[0].Position_X = CueBallPosition_X;
							Ball[0].Position_Y = CueBallPosition_Y;
							EndFreeResetCueBall();
							StartGetHit();
							//设置出口	
						}
						else;										//不合法位置重新调用 
					} 
					else;											//不合法位置重新调用						
				}
				else if(GameMode == GM_POOL)
				{
					if(!Pool.isSuccessfullyBegin)
					{
						if(CueBallPosition_X + Ball_R <= 331 && CueBallPosition_X - Ball_R >= LeftBroad && CueBallPosition_Y + Ball_R <= DownBroad && CueBallPosition_Y - Ball_R >= UpBroad
						&& x + Ball_R <= RightBroad && x - Ball_R >= LeftBroad && y + Ball_R <= DownBroad && y - Ball_R >= UpBroad)
						{
							for(i = 0; i < NUM; i ++)
							{
								Distance0 = Distance(CueBallPosition_X, CueBallPosition_Y, Ball[i].Position_X, Ball[i].Position_Y);
								if(Distance0 <= 2 * Ball_R)
								{
																		//返回放置位置违法提示
									break; 
								}
							}
							if(i >= NUM)
							{
								Ball[0].Moving = STOP;
								Ball[0].State = ONTABLE;
								Ball[0].Position_X = CueBallPosition_X;
								Ball[0].Position_Y = CueBallPosition_Y;
								EndFreeResetCueBall();
								StartGetHit();
								//设置出口	
							}
							else;										//不合法位置重新调用 
						} 
						else;	
					}
					else
					{
						if(x + Ball_R <= RightBroad && x - Ball_R >= LeftBroad && y + Ball_R <= DownBroad && y - Ball_R >= UpBroad)
						{
							for(i = 0; i < NUM; i ++)
							{
								Distance0 = Distance(x, y, Ball[i].Position_X, Ball[i].Position_Y);
								if(Distance0 <= 2 * Ball_R)
								{
																		//返回放置位置违法提示
									break; 
								}
							}
							if(i >= NUM)
							{
								Ball[0].Moving = STOP;
								Ball[0].State = ONTABLE;
								Ball[0].Position_X = x;
								Ball[0].Position_Y = y;
								EndFreeResetCueBall();
								StartGetHit();
								//设置出口	
							}
							else;										//不合法位置重新调用 
						} 
						else;
					}	
				}
			}
			if(event == MOUSEMOVE)
			{
				if(GameMode != GM_SNOOKER && GameMode != GM_POOL)
				{
					if(x + Ball_R < RightBroad && x - Ball_R > LeftBroad && y + Ball_R < DownBroad && y - Ball_R > UpBroad)
					{
						if(CS_Cursor)
						{
							DestroyCursor(CS_Cursor);
							CS_Cursor = NULL;
							SetCursor(CS_Cursor);
						}
						CueBallPosition_X = x;
						CueBallPosition_Y = y;
					}
					else{
						if(!CS_Cursor)
							ResetCursors();
					}				
				}
				else if(GameMode == GM_SNOOKER)
				{
					if(x + Ball_R <= 331 && (Distance(x, y, 331, 317 + TITLEHEIGHT) <= 90 - Ball_R))
					{
						if(CS_Cursor)
						{
							DestroyCursor(CS_Cursor);
							CS_Cursor = NULL;
							SetCursor(CS_Cursor);
						}
					}
					else
					{
						if(!CS_Cursor)
							ResetCursors();
					}
					if(x + Ball_R <= 331)
					{
						VECTOR MouseDirect = {x - 331, y - (317 + TITLEHEIGHT)};
						if(Mod(MouseDirect) <= 90 - Ball_R)
						{
							CueBallPosition_X = x;
							CueBallPosition_Y = y;
						}
						else
						{
							UnitVec(MouseDirect);
							MouseDirect[0] *= (89 - Ball_R);
							MouseDirect[1] *= (89 - Ball_R);
							CueBallPosition_X = MouseDirect[0] + 331;
							CueBallPosition_Y = MouseDirect[1] + 317 + TITLEHEIGHT;	
						}
					}
					else if(x + Ball_R > 331)
					{
						//Hereitis2
						CueBallPosition_X = 330 - Ball_R;
						int InstantY = 0;
						if(y <= (317 + TITLEHEIGHT - Ball_R + 90) && y >= (317 + TITLEHEIGHT + Ball_R - 90))
						{
							InstantY = y;
						}
						else if(y > (317 + TITLEHEIGHT - Ball_R + 90))
						{
							InstantY = (317 + TITLEHEIGHT - Ball_R + 90);
							while(Distance(CueBallPosition_X, InstantY, 331, 317 + TITLEHEIGHT) >= 90 - Ball_R)
							{
								InstantY -= 1;
							}
						}
						else if(y < (317 + TITLEHEIGHT + Ball_R - 90))
						{
							InstantY = (317 + TITLEHEIGHT + Ball_R - 90);
							while(Distance(CueBallPosition_X, InstantY, 331, 317 + TITLEHEIGHT) >= 90 - Ball_R)
							{
								InstantY += 1;
							}
						}
						CueBallPosition_Y = InstantY;
					}
					/* orgin
					if(x + Ball_R <= 331 && (Distance(x, y, 331, 317 + TITLEHEIGHT) <= 90 - Ball_R))
					{
						if(CS_Cursor){
							DestroyCursor(CS_Cursor);
							CS_Cursor = NULL;
							SetCursor(CS_Cursor);
						}
						CueBallPosition_X = x;
						CueBallPosition_Y = y;
					}
					*/
					else{
						if(!CS_Cursor)
							ResetCursors();
					}
				}
				else if(GameMode == GM_POOL)
				{
					if(!Pool.isSuccessfullyBegin)
					{	
						if(x + Ball_R < 331 && x - Ball_R > LeftBroad && y + Ball_R < DownBroad && y - Ball_R > UpBroad)
						{
							if(CS_Cursor){
								DestroyCursor(CS_Cursor);
								CS_Cursor = NULL;
								SetCursor(CS_Cursor);
							}
							CueBallPosition_X = x;
							CueBallPosition_Y = y;
						}
						else if(x + Ball_R >= 331 && y + Ball_R < DownBroad && y - Ball_R > UpBroad)
						{
							if(!CS_Cursor)
								ResetCursors();
							CueBallPosition_X = 330 - Ball_R;
							if(y + Ball_R < DownBroad && y - Ball_R > UpBroad)
							{
								CueBallPosition_Y = y;
							}
							else if(y + Ball_R >= DownBroad)
							{
								CueBallPosition_Y = DownBroad - Ball_R;
							}
							else if(y - Ball_R > UpBroad)
							{
								CueBallPosition_Y = UpBroad + Ball_R;
							}
						}
						else
						{
							if(!CS_Cursor)
								ResetCursors();
						}	
					}
					else
					{
						if(x + Ball_R < RightBroad && x - Ball_R > LeftBroad && y + Ball_R < DownBroad && y - Ball_R > UpBroad)
						{
							if(CS_Cursor){
								DestroyCursor(CS_Cursor);
								CS_Cursor = NULL;
								SetCursor(CS_Cursor);
							}
							CueBallPosition_X = x;
							CueBallPosition_Y = y;
						}
						else{
							if(!CS_Cursor)
								ResetCursors();
						}
					}
				}
			}
		}
		break;
		case MS_START_MENU:
		{/*开始菜单*/ 
			extern int savemode;
			extern LPUSER WhichUser;
			extern LPUSER User1, User2;
			if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
			if( (x>503 && x<773 && y>355+TITLEHEIGHT && y<435+TITLEHEIGHT)
			 || (x>534 && x<741 && y>464+TITLEHEIGHT && y<524+TITLEHEIGHT)
			 || (x>534 && x<741 && y>555+TITLEHEIGHT && y<615+TITLEHEIGHT)
			 || (x>808 && x<1158 && (y>88+TITLEHEIGHT && y<168+TITLEHEIGHT || y>188+TITLEHEIGHT && y<268+TITLEHEIGHT))){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(CS_Cursor);
				BT_ST_start=0,BT_ST_load=0,BT_ST_exit=0,BT_ST_player1=0,BT_ST_player2=0,BT_update=1;
				if(y>355+TITLEHEIGHT && y<435+TITLEHEIGHT){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_ST_start=1; else BT_ST_start=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_ST_start=0;
						ResetCursors();
						EndStartMenu();
						ifLoad = 0;		//是否加载重置回 0 
						StartMasks(StartGmodeMenu);
					}
				}
				else if(y>464+TITLEHEIGHT && y<524+TITLEHEIGHT){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_ST_load=1; else BT_ST_load=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_ST_load=0;
						savemode=LS_LOAD;
						ResetCursors();
						EndStartMenu();
						StartMasks(StartLdSavMenu);
					}
				}
				else if(y>555+TITLEHEIGHT && y<615+TITLEHEIGHT){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_ST_exit=1; else BT_ST_exit=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_ST_exit=0;
						PostMessage(graphicsWindow,WM_SYSCOMMAND,SC_CLOSE,0);//退出游戏
					}
				}
				else if(y>88+TITLEHEIGHT && y<168+TITLEHEIGHT){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_ST_player1=1; else BT_ST_player1=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_ST_player1=0;
						ResetCursors();
						EndStartMenu();
						WhichUser = User1;
						StartMasks(StartUserMenu);
					}
				}
				else if(y>188+TITLEHEIGHT && y<268+TITLEHEIGHT){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_ST_player2=1; else BT_ST_player2=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_ST_player2=0;
						ResetCursors();
						EndStartMenu();
						WhichUser = User2;
						StartMasks(StartUserMenu);
					}
				}
				DrawShakeMnu();
				DrawStartBtn();
			}else if(BT_update){
				ResetCursors();
				BT_ST_start=0,BT_ST_load=0,BT_ST_exit=0,BT_ST_player1=0,BT_ST_player2=0;
				DrawShakeMnu();
				DrawStartBtn();
				BT_update=0;
			}
		}
		break;
		case MS_CHOOSE_GAMEMODE:
		{/*选择模式*/ 
			if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
			if( (x>120 && x<410) && (y>160+TITLEHEIGHT && y<567+TITLEHEIGHT)
			 || (x>495 && x<785) && (
					   (y>160+TITLEHEIGHT && y<347+TITLEHEIGHT)
					|| (y>380+TITLEHEIGHT && y<567+TITLEHEIGHT))
			 || (x>870 && x<1160) && (
					   (y>160+TITLEHEIGHT && y<347+TITLEHEIGHT)
					|| (y>380+TITLEHEIGHT && y<567+TITLEHEIGHT))
			 || (x>95 && x<300) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(CS_Cursor);
				BT_GM_back=0,BT_GM_mode1=0,BT_GM_mode2=0,BT_GM_mode3=0,BT_GM_mode4=0,BT_GM_mode5=0,BT_update=1;
				if((x>120 && x<410) && (y>160+TITLEHEIGHT && y<567+TITLEHEIGHT)){//GM1 
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_GM_mode1=1; else BT_GM_mode1=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_GM_mode1=0;
						ResetCursors();
						GameMode=GM_POOL;
						EndGmodeMenu();
						StartMasks(StartLoad);
					}
				}
				else if((x>495 && x<785)&&(y>160+TITLEHEIGHT && y<347+TITLEHEIGHT)){//GM2
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_GM_mode2=1; else BT_GM_mode2=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_GM_mode2=0;
						ResetCursors();
						GameMode=GM_TRAINNING;
						EndGmodeMenu();
						StartMasks(StartLoad);
					}
				}
				else if((x>870 && x<1160)&&(y>160+TITLEHEIGHT && y<347+TITLEHEIGHT)){//GM3
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_GM_mode3=1; else BT_GM_mode3=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_GM_mode3=0;
						ResetCursors();
						GameMode=GM_SINGLEPLAYER;
						EndGmodeMenu();
						StartMasks(StartLoad);
					}
				}
				else if((x>495 && x<785)&&(y>380+TITLEHEIGHT && y<567+TITLEHEIGHT)){//GM4
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_GM_mode4=1; else BT_GM_mode4=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_GM_mode4=0;
						ResetCursors();
						GameMode=GM_MULTIPLAYER;
						EndGmodeMenu();
						StartMasks(StartLoad);
					}
				}
				else if((x>870 && x<1160)&&(y>380+TITLEHEIGHT && y<567+TITLEHEIGHT)){//GM5
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_GM_mode5=1; else BT_GM_mode5=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_GM_mode5=0;
						ResetCursors();
						GameMode=GM_SNOOKER;
						EndGmodeMenu();
						StartMasks(StartLoad);
					}
				}
				else if((x>95 && x<300) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){//Esc
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_GM_back=1; else BT_GM_back=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_GM_back=0;
						ResetCursors();
						EndGmodeMenu();
						StartMasks(StartStartMenu);
					}
				}
				DrawGmodeBtn();
			}else if(BT_update){
				ResetCursors();
				BT_GM_back=0,BT_GM_mode1=0,BT_GM_mode2=0,BT_GM_mode3=0,BT_GM_mode4=0,BT_GM_mode5=0;
				DrawGmodeBtn();
				BT_update=0;
			}
		}
		break;
		case MS_LOAD_SAVE:
		{/*选择存档*/ 
			extern int savemode;
			extern int savepage;
			extern SAVE LS_Save_Data[4];
			if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
			if( (y>59+TITLEHEIGHT && y<109+TITLEHEIGHT) &&((x>857 && x<907) && (savepage != 1)
														|| (x>917 && x<967)	&& (savepage != 2)
														|| (x>977 && x<1027) && (savepage != 3)	
														|| (x>1037 && x<1087) && (savepage != 4)
														|| (x>1097 && x<1147) && (savepage != 5))
			 || (x>95 && x<300) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)
			 || (x>130 && x<590) && (y>166+TITLEHEIGHT && y<316+TITLEHEIGHT) && (LS_Save_Data[0].flag || savemode==LS_SAVE)
			 || (x>689 && x<1149) && (y>166+TITLEHEIGHT && y<316+TITLEHEIGHT) && (LS_Save_Data[1].flag || savemode==LS_SAVE)
			 || (x>130 && x<590) && (y>405+TITLEHEIGHT && y<555+TITLEHEIGHT) && (LS_Save_Data[2].flag || savemode==LS_SAVE)
			 || (x>689 && x<1149) && (y>405+TITLEHEIGHT && y<555+TITLEHEIGHT) && (LS_Save_Data[3].flag || savemode==LS_SAVE)){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(CS_Cursor);
				BT_LS_back=0,BT_LS_save1=0,BT_LS_save2=0,BT_LS_save3=0,BT_LS_save4=0,
				BT_LS_page1=0,BT_LS_page2=0,BT_LS_page3=0,BT_LS_page4=0,BT_LS_page5=0,
				BT_LS_sav1=0,BT_LS_del1=0,BT_LS_sav2=0,BT_LS_del2=0,BT_LS_sav3=0,BT_LS_del3=0,BT_LS_sav4=0,BT_LS_del4=0,BT_update=1;
				if(y>59+TITLEHEIGHT && y<109+TITLEHEIGHT){
					if(x>857 && x<907){//page 1
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_LS_page1=1; else BT_LS_page1=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							savepage=1;
							BT_LS_page1=0;
							UpdtLdSavMsg();
							ResetCursors();
						}
					}else if(x>917 && x<967){//page 2
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_LS_page2=1; else BT_LS_page2=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							savepage=2;
							BT_LS_page2=0;
							UpdtLdSavMsg();
							ResetCursors();
						}
					}else if(x>977 && x<1027){//page 3
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_LS_page3=1; else BT_LS_page3=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							savepage=3;
							BT_LS_page3=0;
							UpdtLdSavMsg();
							ResetCursors();
						}
					}else if(x>1037 && x<1087){//page 4
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_LS_page4=1; else BT_LS_page4=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							savepage=4;
							BT_LS_page4=0;
							UpdtLdSavMsg();
							ResetCursors();
						}
					}else if(x>1097 && x<1147){//page 5
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_LS_page5=1; else BT_LS_page5=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							savepage=5;
							BT_LS_page5=0;
							UpdtLdSavMsg();
							ResetCursors();
						}
					}	
				}
				else if((x>95 && x<300) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){//Esc
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_LS_back=1; else BT_LS_back=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_LS_back=0;
						ResetCursors();
						switch(savemode){
							case LS_SAVE:
								EndLdSavMenu();
								StartMasks(StartPauseMenuState);
							break;
							case LS_LOAD:
								EndLdSavMenu();
								StartMasks(StartStartMenu);
							break;
						}
					}
				}
				else if((x>130 && x<590) && (y>166+TITLEHEIGHT && y<316+TITLEHEIGHT)){//NO.1
					if((x>392 && x<482) && (y>284+TITLEHEIGHT && y<309+TITLEHEIGHT)){//DEL
						if(LS_Save_Data[0].flag){
							if(button == LEFT_BUTTON && event == BUTTON_DOWN)
								BT_button=1;
							if(!BT_button) BT_LS_del1=1; else BT_LS_del1=2;
							if(button == LEFT_BUTTON && event == BUTTON_UP){
								BT_LS_del1=0;
								ResetCursors();
								DlteSave(4*(savepage-1) + 1);
								PlayDlteMusic();
								UpdtLdSavMsg();
							}
						}
						else{
							DestroyCursor(CS_Cursor);
							CS_Cursor = LoadCursor(NULL, IDC_NO);
							SetCursor(CS_Cursor);
						}
					}
					else{//SAVE|LOAD
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_LS_sav1=1; else BT_LS_sav1=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_LS_sav1=0;
							ResetCursors();
							switch(savemode){
								case LS_SAVE:
									SaveSave(4*(savepage-1) + 1);
									PlaySaveMusic();
									UpdtLdSavMsg();
								break;
								case LS_LOAD:
									EndLdSavMenu();
									ReadSave(4*(savepage-1) + 1);
									StartMasks(StartLoad);
								break;
							}
						}
					}
				}
				else if((x>689 && x<1149) && (y>166+TITLEHEIGHT && y<316+TITLEHEIGHT)){//NO.2
					if((x>951 && x<1041) && (y>284+TITLEHEIGHT && y<309+TITLEHEIGHT)){//DEL
						if(LS_Save_Data[1].flag){
							if(button == LEFT_BUTTON && event == BUTTON_DOWN)
								BT_button=1;
							if(!BT_button) BT_LS_del2=1; else BT_LS_del2=2;
							if(button == LEFT_BUTTON && event == BUTTON_UP){
								BT_LS_del2=0;
								ResetCursors();
								DlteSave(4*(savepage-1) + 2);
								PlayDlteMusic();
								UpdtLdSavMsg();
							}
						}
						else{
							DestroyCursor(CS_Cursor);
							CS_Cursor = LoadCursor(NULL, IDC_NO);
							SetCursor(CS_Cursor);
						}
					}
					else{//SAVE|LOAD
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_LS_sav2=1; else BT_LS_sav2=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_LS_sav2=0;
							ResetCursors();
							switch(savemode){
								case LS_SAVE:
									SaveSave(4*(savepage-1) + 2);
									PlaySaveMusic();
									UpdtLdSavMsg();
								break;
								case LS_LOAD:
									EndLdSavMenu();
									ReadSave(4*(savepage-1) + 2);
									StartMasks(StartLoad);
								break;
							}
						}
					}
				}
				else if((x>130 && x<590) && (y>405+TITLEHEIGHT && y<555+TITLEHEIGHT)){//NO.3
					if((x>392 && x<482) && (y>523+TITLEHEIGHT && y<548+TITLEHEIGHT)){//DEL
						if(LS_Save_Data[2].flag){
							if(button == LEFT_BUTTON && event == BUTTON_DOWN)
								BT_button=1;
							if(!BT_button) BT_LS_del3=1; else BT_LS_del3=2;
							if(button == LEFT_BUTTON && event == BUTTON_UP){
								BT_LS_del3=0;
								ResetCursors();
								DlteSave(4*(savepage-1) + 3);
								PlayDlteMusic();
								UpdtLdSavMsg();
							}
						}
						else{
							DestroyCursor(CS_Cursor);
							CS_Cursor = LoadCursor(NULL, IDC_NO);
							SetCursor(CS_Cursor);
						}
					}
					else{//SAVE|LOAD
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_LS_sav3=1; else BT_LS_sav3=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_LS_sav3=0;
							ResetCursors();
							switch(savemode){
								case LS_SAVE:
									SaveSave(4*(savepage-1) + 3);
									PlaySaveMusic();
									UpdtLdSavMsg();
								break;
								case LS_LOAD:
									EndLdSavMenu();
									ReadSave(4*(savepage-1) + 3);
									StartMasks(StartLoad);
								break;
							}
						}
					}
				}
				else if((x>689 && x<1149) && (y>405+TITLEHEIGHT && y<555+TITLEHEIGHT)){//NO.4
					if((x>951 && x<1041) && (y>523+TITLEHEIGHT && y<548+TITLEHEIGHT)){//DEL
						if(LS_Save_Data[3].flag){
							if(button == LEFT_BUTTON && event == BUTTON_DOWN)
								BT_button=1;
							if(!BT_button) BT_LS_del4=1; else BT_LS_del4=2;
							if(button == LEFT_BUTTON && event == BUTTON_UP){
								BT_LS_del4=0;
								ResetCursors();
								DlteSave(4*(savepage-1) + 4);
								PlayDlteMusic();
								UpdtLdSavMsg();
							}
						}
						else{
							DestroyCursor(CS_Cursor);
							CS_Cursor = LoadCursor(NULL, IDC_NO);
							SetCursor(CS_Cursor);
						}
					}
					else{//SAVE|LOAD
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_LS_sav4=1; else BT_LS_sav4=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_LS_sav4=0;
							ResetCursors();
							switch(savemode){
								case LS_SAVE:
									SaveSave(4*(savepage-1) + 4);
									PlaySaveMusic();
									UpdtLdSavMsg();
								break;
								case LS_LOAD:
									EndLdSavMenu();
									ReadSave(4*(savepage-1) + 4);
									StartMasks(StartLoad);
								break;
							}
						}
					}
				}
				DrawLdSavBtn();
			}else if((x>130 && x<590) && (y>166+TITLEHEIGHT && y<316+TITLEHEIGHT)
				  || (x>689 && x<1149) && (y>166+TITLEHEIGHT && y<316+TITLEHEIGHT)
				  || (x>130 && x<590) && (y>405+TITLEHEIGHT && y<555+TITLEHEIGHT)
				  || (x>689 && x<1149) && (y>405+TITLEHEIGHT && y<555+TITLEHEIGHT)){  	
				BT_update=1;
			 	DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_NO);
				SetCursor(CS_Cursor);
			}else if(BT_update){
				ResetCursors();
				BT_LS_back=0,BT_LS_save1=0,BT_LS_save2=0,BT_LS_save3=0,BT_LS_save4=0,
				BT_LS_page1=0,BT_LS_page2=0,BT_LS_page3=0,BT_LS_page4=0,BT_LS_page5=0,
				BT_LS_sav1=0,BT_LS_del1=0,BT_LS_sav2=0,BT_LS_del2=0,BT_LS_sav3=0,BT_LS_del3=0,BT_LS_sav4=0,BT_LS_del4=0;
				DrawLdSavBtn();
				BT_update=0;
			}
		}
		break;
		case MS_REGISTER:
		{/*注册账号*/ 
			extern bool f_register;
			if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
			if((x>527 && x<752) && (y>153+TITLEHEIGHT && y<378+TITLEHEIGHT)
			|| (x>430 && x<850) && (y>455+TITLEHEIGHT && y<525+TITLEHEIGHT) && !InitCheck()
			|| (x>980 && x<1185) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(CS_Cursor);
				BT_REG_avatar=0,BT_REG_text=0,BT_REG_ok=0,BT_update=1;
				if((x>527 && x<752) && (y>153+TITLEHEIGHT && y<378+TITLEHEIGHT)){//头像 
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_REG_avatar=1; else BT_REG_avatar=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_REG_avatar=0;
						ResetCursors();
						char szFile[MAX_PATH] = {0};
						EnableWindow(graphicsWindow,false);
						if(FileDialog(szFile,"请选择自定义头像文件","图片文件 (*.BMP;*.JPG)\0*.BMP;*.JPG\0")){
							puts(szFile);
							NewAvarar(szFile);
						}
						EnableWindow(graphicsWindow,true);
					}
				}
				else if((x>430 && x<850) && (y>455+TITLEHEIGHT && y<525+TITLEHEIGHT) && !InitCheck()){//昵称
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_REG_text=1; else BT_REG_text=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_REG_text=0;
						ResetCursors();
						ActiveRegText();
					}
				}
				else if((x>980 && x<1185) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){//ok
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_REG_ok=1; else BT_REG_ok=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_REG_ok=0;
						ResetCursors();
						FinishRegist();
						EndRegstMenu();
					}
				}
				DrawRegstBtn();
			}else if((x>95 && x<300) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT) && f_register){//Esc
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(CS_Cursor);
				BT_REG_back=0,BT_update=1;
				if(button == LEFT_BUTTON && event == BUTTON_DOWN)
					BT_button=1;
				if(!BT_button) BT_REG_back=1; else BT_REG_back=2;
				if(button == LEFT_BUTTON && event == BUTTON_UP){
					BT_REG_back=0;
					ResetCursors();
					EndRegstMenu();
				}
				DrawRegstBtn();
			}else if(BT_update){
				ResetCursors();
				BT_REG_avatar=0,BT_REG_text=0,BT_REG_ok=0,BT_REG_back=0;
				DrawRegstBtn();
				BT_update=0;
			}
		}
		break;
		case MS_USER:
		{/*用户界面*/ 
			extern bool editing;
			if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
			if((x>135 && x<310) && (y>60+TITLEHEIGHT && y<235+TITLEHEIGHT)
			|| (x>790 && x<830) && (y>95+TITLEHEIGHT && y<135+TITLEHEIGHT) && !editing
			|| (x>860 && x<900) && (y>95+TITLEHEIGHT && y<135+TITLEHEIGHT)
			|| (x>980 && x<1185) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)
			|| (x>95 && x<300) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(CS_Cursor);
				BT_USER_avatar=0,BT_USER_text=0,BT_USER_background=0,BT_USER_back=0,BT_USER_def=0,BT_update=1;
				if((x>135 && x<310) && (y>60+TITLEHEIGHT && y<235+TITLEHEIGHT)){//头像 
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_USER_avatar=1; else BT_USER_avatar=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_USER_avatar=0;
						ResetCursors();
						char szFile[MAX_PATH] = {0};
						EnableWindow(graphicsWindow,false);
						if(FileDialog(szFile,"请选择自定义头像文件","图片文件 (*.BMP;*.JPG)\0*.BMP;*.JPG\0")){
							puts(szFile);
							NewUserAvarar(szFile);
						}
						EnableWindow(graphicsWindow,true);
					}
				}
				else if((x>790 && x<830) && (y>95+TITLEHEIGHT && y<135+TITLEHEIGHT)){//昵称
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_USER_text=1; else BT_USER_text=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_USER_text=0;
						ResetCursors();
						ActiveUsrText();
					}
				}
				else if((x>860 && x<900) && (y>95+TITLEHEIGHT && y<135+TITLEHEIGHT)){//背景 
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_USER_background=1; else BT_USER_background=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_USER_background=0;
						ResetCursors();
						char szFile[MAX_PATH] = {0};
						EnableWindow(graphicsWindow,false);
						if(FileDialog(szFile,"请选择自定义背景文件","图片文件 (*.BMP;*.JPG)\0*.BMP;*.JPG\0")){
							puts(szFile);
							NewUserBackground(szFile);
						}
						EnableWindow(graphicsWindow,true);
					}
				}
				else if((x>980 && x<1185) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){//def
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_USER_def=1; else BT_USER_def=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_USER_def=0;
						ResetCursors();
						EndUserMenu();
						StartMasks(StartChooseStick);
					}
				}else if((x>95 && x<300) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){//Esc
					DestroyCursor(CS_Cursor);
					CS_Cursor = LoadCursor(NULL, IDC_HAND);
					SetCursor(CS_Cursor);
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_USER_back=1; else BT_USER_back=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_USER_back=0;
						ResetCursors();
						EndUserMenu();
						StartMasks(StartStartMenu);
					}
				}
				DrawUsersBtn();
			}else if(BT_update){
				ResetCursors();
				BT_USER_avatar=0,BT_USER_text=0,BT_USER_background=0,BT_USER_back=0,BT_USER_def=0;
				DrawUsersBtn();
				BT_update=0;
			}
		}
		break;
		case MS_PAUSE_MENU:
		{/*暂停菜单*/ 
			extern int savemode;
			if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
			if( (y>250+TITLEHEIGHT && y<470+TITLEHEIGHT)
			 && ((x>155 && x<415)
			 ||  (x>510 && x<780)
			 ||  (x>865 && x<1125) ) ){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(CS_Cursor);
				BT_Pause_Continue=0,BT_Pause_Save=0,BT_Pause_Quit=0,BT_update=1;
				if(x>155 && x<415){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_Pause_Continue=1; else BT_Pause_Continue=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_Pause_Continue=0;
						ResetCursors();
						EndPauseMenu();
						break;
					}
				}
				else if(x>510 && x<780){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_Pause_Save=1; else BT_Pause_Save=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_Pause_Save=0;
						savemode=LS_SAVE;
						ResetCursors();
						EndPauseMenuState();
						StartMasks(StartLdSavMenu);
					}
				}
				else if(x>865 && x<1125){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_Pause_Quit=1; else BT_Pause_Quit=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_Pause_Quit=0;
						ResetCursors();
						if(Pool.End != 1 && Classic.End != 1 && Snooker.End != 1)
						{
							GameTimeSys->GetTime(TS_ED);
							GameTimeSys->GetTime(TS_AL);
						}
						EndPauseMenuWithoutCallBack();
						StartMasks(StartStartMenu);
						break;
					}
				}
				DrawPauseBtn();
			}else if(BT_update){
				ResetCursors();
				BT_Pause_Continue=0,BT_Pause_Save=0,BT_Pause_Quit=0;
				DrawPauseBtn();
				BT_update=0;
			}
		}
		break;
		case MS_WIN:
		{/*游戏结束*/ 
			if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
			if( (y>625+TITLEHEIGHT && y<690+TITLEHEIGHT)
			 && ((x>30 && x<240)
			 ||  (x>1040 && x<1250)) ){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(CS_Cursor);
				BT_Win_Back=0,BT_Win_Save=0,BT_update=1;
				if((x>30 && x<240)){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_Win_Back=1; else BT_Win_Back=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_Win_Back=0;
						ResetCursors();
						EndWinMenu();
						StartMasks(StartStartMenu);
					}
				}
				else if(x>1040 && x<1250){
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_Win_Save=1; else BT_Win_Save=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_Win_Save=0;
						ResetCursors();
						SaveWinPage();
						PlaySaveMusic();
					}
				}
				DrawWinerBtn();
			}else if(BT_update){
				ResetCursors();
				BT_Win_Back=0,BT_Win_Save=0;
				DrawWinerBtn();
				BT_update=0;
			}
		}
		break;
		case MS_CHOOSE_STICK:
		{/*选择球杆*/ 
			extern int Stick_Choice;
			if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
			if( (y>160+TITLEHEIGHT && y<270+TITLEHEIGHT) && ((x>120 && x<440)
														 ||  (x>480 && x<800)
														 ||  (x>840 && x<1160))
			 || (y>310+TITLEHEIGHT && y<420+TITLEHEIGHT) && ((x>120 && x<440)
														 ||  (x>480 && x<800)
														 ||  (x>840 && x<1160))
			 || (y>460+TITLEHEIGHT && y<570+TITLEHEIGHT) && ((x>120 && x<440)
														 ||  (x>480 && x<800)
														 ||  (x>840 && x<1160))
			 || (x>95 && x<300) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(CS_Cursor);
				BT_Stick_back=0,BT_Stick_0=0,BT_Stick_1=0,BT_Stick_2=0,BT_Stick_3=0,
				BT_Stick_4=0,BT_Stick_5=0,BT_Stick_6=0,BT_Stick_7=0,BT_Stick_8=0,BT_update=1;
				if(y>160+TITLEHEIGHT && y<270+TITLEHEIGHT){//1-3
					if(x>120 && x<440){
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_Stick_0=1; else BT_Stick_0=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_Stick_0=0;
							ResetCursors();
							Stick_Choice = 0;
						}
					}
					else if(x>480 && x<800){
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_Stick_1=1; else BT_Stick_1=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_Stick_1=0;
							ResetCursors();
							Stick_Choice = 1;
						}
					}
					else if(x>840 && x<1160){
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_Stick_2=1; else BT_Stick_2=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_Stick_2=0;
							ResetCursors();
							Stick_Choice = 2;
						}
					}
				}
				else if(y>310+TITLEHEIGHT && y<420+TITLEHEIGHT){//4-6
					if(x>120 && x<440){
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_Stick_3=1; else BT_Stick_3=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_Stick_3=0;
							ResetCursors();
							Stick_Choice = 3;
						}
					}
					else if(x>480 && x<800){
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_Stick_4=1; else BT_Stick_4=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_Stick_4=0;
							ResetCursors();
							Stick_Choice = 4;
						}
					}
					else if(x>840 && x<1160){
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_Stick_5=1; else BT_Stick_5=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_Stick_5=0;
							ResetCursors();
							Stick_Choice = 5;
						}
					}
				}
				else if(y>460+TITLEHEIGHT && y<570+TITLEHEIGHT){//7-9
					if(x>120 && x<440){
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_Stick_6=1; else BT_Stick_6=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_Stick_6=0;
							ResetCursors();
							Stick_Choice = 6;
						}
					}
					else if(x>480 && x<800){
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_Stick_7=1; else BT_Stick_7=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_Stick_7=0;
							ResetCursors();
							Stick_Choice = 7;
						}
					}
					else if(x>840 && x<1160){
						if(button == LEFT_BUTTON && event == BUTTON_DOWN)
							BT_button=1;
						if(!BT_button) BT_Stick_8=1; else BT_Stick_8=2;
						if(button == LEFT_BUTTON && event == BUTTON_UP){
							BT_Stick_8=0;
							ResetCursors();
							Stick_Choice = 8;
						}
					}
				}
				else if((x>95 && x<300) && (y>625+TITLEHEIGHT && y<670+TITLEHEIGHT)){//Esc
					if(button == LEFT_BUTTON && event == BUTTON_DOWN)
						BT_button=1;
					if(!BT_button) BT_Stick_back=1; else BT_Stick_back=2;
					if(button == LEFT_BUTTON && event == BUTTON_UP){
						BT_Stick_back=0;
						ResetCursors();
						EndChooseStick();
						StartMasks(StartUserMenu);
					}
				}
				DrawStickBtn();
			}else if(BT_update){
				ResetCursors();
				BT_Stick_back=0,BT_Stick_0=0,BT_Stick_1=0,BT_Stick_2=0,BT_Stick_3=0,
				BT_Stick_4=0,BT_Stick_5=0,BT_Stick_6=0,BT_Stick_7=0,BT_Stick_8=0;
				DrawStickBtn();
				BT_update=0;
			}
		}
		break;
	}
}

static int LEFT = 0;
static int RIGHT = 0;
static int UP = 0;
static int DOWN = 0;
static int ESC = 0;

extern int LoadState;

void KeyboardCallback(int key,int event){
	//if(key == VK_RWIN || key == VK_LWIN) PostMessage(graphicsWindow,WM_CLOSE,0,0); //强制退出游戏 
	/*文本框*/
	if(InitCheck()) 
		TextBoxKeyboard(key,event);
	switch(KEYBOARD_STATE){
		case KB_PREPARE_TO_HIT:
		{/*准备击球*/
			if(!ESC && key == VK_ESCAPE && event == KEY_DOWN){
				ESC=!ESC;
				BT_button=0;
				LoadState=MOUSE_STATE;
				ResetCursors();
				EndPrepare();
				StartPauseMenu(StartPrepare);
				break;
			}
			if(ESC && key == VK_ESCAPE && event == KEY_UP){
				ESC=!ESC;
				break;
			}
		}
		break;
		case KB_START_HIT:
		{/*多球碰撞开始*/
			if(!ESC && key == VK_ESCAPE && event == KEY_DOWN){
				ESC=!ESC;
				BT_button=0;
				LoadState=MOUSE_STATE;
				EndHit();
				StartPauseMenu(StartHit);
				break;
			}
			if(ESC && key == VK_ESCAPE && event == KEY_UP){
				ESC=!ESC;
				break;
			}
		}
		break;
		case MS_FREE_RESET_CUEBALL:
		{/*自由放置母球*/
			if(!ESC && key == VK_ESCAPE && event == KEY_DOWN){
				ESC=!ESC;
				BT_button=0;
				LoadState=MOUSE_STATE;
				EndFreeResetCueBall();
				StartPauseMenu(StartFreeResetCueBall);
				break;
			}
			if(ESC && key == VK_ESCAPE && event == KEY_UP){
				ESC=!ESC;
				break;
			}
		}
		break;
		case KB_PAUSE_MENU:
		{/*暂停界面*/
			if(!ESC && key == VK_ESCAPE && event == KEY_DOWN){
				ESC=!ESC;
				BT_button=0;
				ResetCursors();
				EndPauseMenu();
				break;
			}
			if(ESC && key == VK_ESCAPE && event == KEY_UP){
				ESC=!ESC;
				break;
			}
		}
		break;
		case KB_GET_HIT_POSITION:
		{/*获取击球位置*/
			if(!ESC && key == VK_ESCAPE && event == KEY_DOWN){
				ESC=!ESC;
				BT_button=0;
				LoadState=MOUSE_STATE;
				EndGetHit();
				StartPauseMenu(StartGetHit);
				break;
			}
			if(ESC && key == VK_ESCAPE && event == KEY_UP){
				ESC=!ESC;
				break;
			}
			if(key == VK_LEFT)
				if(event == KEY_DOWN) LEFT=1;  else LEFT=0;
			if(key == VK_RIGHT)
				if(event == KEY_DOWN) RIGHT=1; else RIGHT=0;
			if(key == VK_UP)
				if(event == KEY_DOWN) UP=1;    else UP=0;
			if(key == VK_DOWN)
				if(event == KEY_DOWN) DOWN=1;  else DOWN=0;
			
			if(LEFT)
				if(Distance(HitPosition_X, HitPosition_Y, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R
				 && Distance(HitPosition_X - 3, HitPosition_Y, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R)
					HitPosition_X -= 2;
			if(RIGHT)
				if(Distance(HitPosition_X, HitPosition_Y, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R
				 && Distance(HitPosition_X + 3, HitPosition_Y, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R)
					HitPosition_X += 2;
			if(UP)
				if(Distance(HitPosition_X, HitPosition_Y, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R
				 && Distance(HitPosition_X, HitPosition_Y - 3, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R)
					HitPosition_Y -= 2;
			if(DOWN)
				if(Distance(HitPosition_X, HitPosition_Y, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R
				 && Distance(HitPosition_X, HitPosition_Y + 3, DirectionBall_X, DirectionBall_Y) < DirectionBall_R - Dot_R)
					HitPosition_Y += 2;			
			if((key == VK_RETURN || key == VK_SPACE) && event == KEY_DOWN)
			{
				//HitPosition_X = CurrentPosition_X;
				//HitPosition_Y = CurrentPosition_Y;
				//设置出口 
			}
		}
		break;
		case KB_CHOOSE_GAMEMODE:
		{/*选择模式*/
			if(key == VK_ESCAPE && event == KEY_DOWN){
				ResetCursors();
				EndGmodeMenu();
				StartMasks(StartStartMenu);
			} 
		}
		break;
		case KB_LOAD_SAVE:
		{/*选择存档*/
			extern int savemode;
			if(key == VK_ESCAPE && event == KEY_DOWN){
				ResetCursors();
				switch(savemode){
					case LS_SAVE:
						EndLdSavMenu();
						StartMasks(StartPauseMenuState);
					break;
					case LS_LOAD:
						EndLdSavMenu();
						StartMasks(StartStartMenu);
					break;
				}
			} 
		}
		break;
		case KB_REGISTER:
		{/*注册账号*/
			extern bool f_register;
			if(key == VK_ESCAPE && event == KEY_DOWN && f_register){
				ResetCursors();
				EndRegstMenu();
			} 
		}
		break;
		case KB_USER:
		{/*用户界面*/
			if(key == VK_ESCAPE && event == KEY_DOWN){
				ResetCursors();
				EndUserMenu();
				StartMasks(StartStartMenu);
			} 
		}
		break;
		case KB_WIN:
		{/*游戏结束*/
			if(key == VK_ESCAPE && event == KEY_DOWN){
				ResetCursors();
				EndWinMenu();
				StartMasks(StartStartMenu);
			} 
		}
		break;
		case KB_CHOOSE_STICK:
		{/*选择球杆*/
			if(key == VK_ESCAPE && event == KEY_DOWN){
				ResetCursors();
				EndChooseStick();
				StartMasks(StartUserMenu);
			} 
		}
		break;
	}
}

void CharCallback(char c){
	/*文本框*/
	if(InitCheck()) 
		TextBoxChar(c);
}

void InitCallback(){
	registerCharEvent(CharCallback);
	registerMouseEvent(MouseCallback);
	registerTimerEvent(TimeCallback);
	registerKeyboardEvent(KeyboardCallback);
}
