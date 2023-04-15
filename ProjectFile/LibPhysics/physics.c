#include <math.h>
#include <text.h>
#include <stdio.h>
#include <media.h> 
#include <stdlib.h>
#include <vector.h>
#include <physics.h>
#include <pictures.h>
#include <graphics.h>
#include <extgraph.h> 

#include <gui.h>
#include <sets.h>
#include <test.h>
#include <public.h>
#include <callback.h>
#include <timesys.h>
#include <ai.h>

extern float ForceBarSpeed;
extern float ForceBarStep;

extern float FixConst;									//专门用于让开球时临时未处理球产生位置差距的计数变量 

extern HWND MenuMusicWindow;
extern HWND CollisionMusicWindow;
extern HWND BackGroundMusicWindow;

extern int MenuMusicID;
extern int CollisionMusicID;
extern int BackGroundMusicID;

extern int NUM;
extern double X, Y;
extern float const MaxVelocity;
extern float Power;
extern int Sides;									//<需存档变量> 对战模式 - 回合数（用于判断属于谁的回合） 
extern int Violation;								//犯规判定 
extern float CueTheta;
extern int Menu;
extern VECTOR DirectionVec;							//方向向量 
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

extern struct Acceleration
{
	float AccelerationMax;
	float Acceleration;
	float SwirlTheta;							//旋转角，逆时针转取正号 
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

extern float Acceleration_Max;
extern float SwirlTheta_Max;
extern float ReflectTheta_Max;
extern float ImpactOffset;

extern char CollideAddress[10][9];
extern int ViolationShelf_X[2][5];
extern float RealHole_X[6];
extern float RealHole_Y[6];
extern float Hole_X[6];
extern float Hole_Y[6];
extern bool InHoleFinished[22];
extern int WhichHole[22];
extern float Draw_R[22];
extern float const ShelfGap;
extern float Shelf_Y[15];
extern MODE GameMode;
extern float K_Attenuation;
extern int ifLoad;
extern float K_Swift;
int UpdateTimes = 0;

/*音频资源*/
extern LPMUSIC 	MU_Begin,
				MU_Pause,
				MU_Saved,
				MU_Collide,
				MU_Reflect,
				MU_Inhole,
				MU_OnButton,
				MU_SwitchMenu,
				MU_SwitchSide; 

double Distance(double DotA_X, double DotA_Y, double DotB_X, double DotB_Y)
{
	return sqrt((DotA_X - DotB_X) * (DotA_X - DotB_X) + (DotA_Y - DotB_Y) * (DotA_Y - DotB_Y));
}

void MoveUpdate(int i)
{
	if(UpdateTimes == 0) { PreJudge(); UpdateTimes ++; }
	Move(i); 
	if(i == 0) Swirl();
	Collision(i);	
	Friction(i);
	Stop(i);		
} 

void PreJudge()
{
	int i, index = -1, isCollision = 0;
	float Distance_Min = 99999;
	VECTOR NextVec = {K_VXSwitch * K_Friction * Ball[0].Velocity_X, K_VXSwitch * K_Friction * Ball[0].Velocity_Y};
	for(i = 1; i < NUM; i ++)
	{
		float Distance0 = Distance(Ball[0].Position_X, Ball[0].Position_Y, Ball[i].Position_X, Ball[i].Position_Y);
		if(Distance0 < Distance_Min)
		{
			index = i;
			Distance_Min = Distance0;
		}
	}
	int ifNeedFix = 0, MaxTimes = 50, Times;
	float PositionCur_X, PositionCur_Y;
	for(Times = 1; Times < MaxTimes + 1; Times ++)
	{
		PositionCur_X = Ball[0].Position_X + 0.02 * Times * K_VXSwitch * K_Friction * Ball[0].Velocity_X;
		PositionCur_Y = Ball[0].Position_Y + 0.02 * Times * K_VXSwitch * K_Friction * Ball[0].Velocity_Y;
		float Distance_Cur = Distance(PositionCur_X, PositionCur_Y, Ball[index].Position_X, Ball[index].Position_Y);
		if(Distance_Cur < 2 * Ball_R)
		{	
			Ball[0].Position_X = PositionCur_X;
			Ball[0].Position_Y = PositionCur_Y;
			ifNeedFix = 1;
			break;
		}
	}
	if(ifNeedFix)
	{
		//printf("Prejudge End!\n");
		while(Distance(Ball[index].Position_X, Ball[index].Position_Y, Ball[0].Position_X, Ball[0].Position_Y) < 2 * Ball_R)
		{
			Rotation.Reflects ++;
			isCollision = 1;
			Ball[0].Position_X -= 0.02 * Ball[0].Velocity_X;
			Ball[0].Position_Y -= 0.02 * Ball[0].Velocity_Y;
		}
		//printf("Fixed!\n");						
	}
	if(!isCollision)
		return;
	else
	{
		PostThreadMessage(CollisionMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_Collide); 
		isCollision = 0;
	} 
	int j = index;
	i = 0;
	float Distance1 = Ball[i].Position_X - Ball[j].Position_X; 
	float Distance2 = Ball[i].Position_Y - Ball[j].Position_Y;
	VECTOR UnitDistance = {Distance1, Distance2};			//两球球心连线单位向量，同时确定了水平投影方向
	VECTOR UnitDistance_Rot = {- Distance2, Distance1};		//两球球心连线旋转向量，同时确定了垂直投影方向
	//将两个向量单位化 
	UnitVec(UnitDistance);
	UnitVec(UnitDistance_Rot); 
	//建立投影速度向量 
	VECTOR Velocity1 = {Ball[i].Velocity_X * UnitDistance[0] + Ball[i].Velocity_Y * UnitDistance[1], Ball[i].Velocity_X * UnitDistance_Rot[0] + Ball[i].Velocity_Y * UnitDistance_Rot[1]};			//投影化速度向量 
	VECTOR Velocity2 = {Ball[j].Velocity_X * UnitDistance[0] + Ball[j].Velocity_Y * UnitDistance[1], Ball[j].Velocity_X * UnitDistance_Rot[0] + Ball[j].Velocity_Y * UnitDistance_Rot[1]}; 			//投影化速度向量
	//水平投影方向速度互换 
	float Temp = Velocity1[0];
	Velocity1[0] = Velocity2[0];
	Velocity2[0] = Temp;		
	if(i == 0 && j != 0 && Ball[i].Moving == MOVING || i != 0 && j == 0 && Ball[j].Moving == MOVING)
		if(Rotation.FrontRotation)
			Velocity1[0] += ImpactOffset * Velocity2[0] * Rotation.MoveRatio;
		else if(Rotation.BackRotation)
			Velocity1[0] -= ImpactOffset * Velocity2[0] * Rotation.MoveRatio;
	//建立分配后速度向量 
	VECTOR UnitVelocity1 = {Velocity1[0] * UnitDistance[0], Velocity1[0] * UnitDistance[1]};					//分配后水平投影速度向量 1
	VECTOR UnitVelocity1_Rot = {Velocity1[1] * UnitDistance_Rot[0], Velocity1[1] * UnitDistance_Rot[1]}; 		//分配后垂直投影速度向量 1
	VECTOR UnitVelocity2 = {Velocity2[0] * UnitDistance[0], Velocity2[0] * UnitDistance[1]};					//分配后水平投影速度向量 2
	VECTOR UnitVelocity2_Rot = {Velocity2[1] * UnitDistance_Rot[0], Velocity2[1] * UnitDistance_Rot[1]}; 		//分配后垂直投影速度向量 2
	//速度重新赋值 
	Ball[i].Velocity_X = UnitVelocity1[0] + UnitVelocity1_Rot[0]; 
	Ball[i].Velocity_Y = UnitVelocity1[1] + UnitVelocity1_Rot[1];
	Ball[j].Velocity_X = UnitVelocity2[0] + UnitVelocity2_Rot[0]; 
	Ball[j].Velocity_Y = UnitVelocity2[1] + UnitVelocity2_Rot[1];
	Ball[j].Moving = MOVING;
}

float K_AvoidFix = 0;
extern float K_MoveOut;
void Collision(int i)
{
	if(Ball[i].State == INHOLE)
		return;
	int j, isCollision = 0, isReflect = 0;
	ScoreJudge(); 
	if(Rotation.RotationRatio <= 0.1)
		Rotation.RotationRatio = 0;
	/* 与球桌边沿的碰撞 */
	if(Ball[i].State == ONTABLE && Ball[i].Moving == MOVING)		//如果球体在桌面上且在运动 
	{
		if(Ball[i].Position_X - Ball_R <= LeftBroad)  				//是否与左边沿发生碰撞 	 
		{	
			isReflect = 1;
			if(i != 0)
				Pool.Reflect ++; 
			else
				Rotation.Reflects ++; 	
			Ball[i].Velocity_X = - Ball[i].Velocity_X;
			if(Ball[i].Position_X - K_VXSwitch * Ball[i].Velocity_X - Ball_R < LeftBroad)
				Ball[i].Position_X = LeftBroad + Ball_R;
			else;
		}	
		if(Ball[i].Position_X + Ball_R >= RightBroad)
		{	
			isReflect = 1;
			if(i != 0)
				Pool.Reflect ++; 
			else
				Rotation.Reflects ++; 
			Ball[i].Velocity_X = - Ball[i].Velocity_X;
			if(Ball[i].Position_X - K_VXSwitch * Ball[i].Velocity_X + Ball_R > RightBroad)
				Ball[i].Position_X = RightBroad - Ball_R;
			else;  	
		}
		//if(Ball[i].Position_Y - Ball_R <= UpBroad && !(Ball[i].Position_X <= 664 && Ball[i].Position_X >= 616))
		if(Ball[i].Position_Y - Ball_R <= UpBroad && !(Ball[i].Position_X <= UpBroadNearHoleR_X && Ball[i].Position_X >= UpBroadNearHoleL_X))
		{	
			isReflect = 1;
			if(i != 0)
				Pool.Reflect ++; 
			else
				Rotation.Reflects ++; 
			Ball[i].Velocity_Y = - Ball[i].Velocity_Y;
			if(Ball[i].Position_Y - K_VXSwitch * Ball[i].Velocity_Y - Ball_R < UpBroad)
				Ball[i].Position_Y = UpBroad + Ball_R;
			else;	
		}
		if(Ball[i].Position_Y + Ball_R >= DownBroad && !(Ball[i].Position_X <= DownBroadNearHoleR_X && Ball[i].Position_X >= DownBroadNearHoleL_X))
		{	
			isReflect = 1;
			if(i != 0)
				Pool.Reflect ++; 
			else
				Rotation.Reflects ++; 
			Ball[i].Velocity_Y = - Ball[i].Velocity_Y;
			if(Ball[i].Position_Y - K_VXSwitch * Ball[i].Velocity_Y + Ball_R > DownBroad)
				Ball[i].Position_Y = DownBroad - Ball_R;
			else;	
		}
		if((Ball[i].Position_X <= 616 + Ball_R && Ball[i].Position_X >= UpBroadNearHoleL_X) && (Ball[i].Position_Y >= UpBroadNearHoleU_Y && Ball[i].Position_Y <= UpBroadNearHoleD_Y + 8 * 1.414))
		{
			if(Ball[i].Position_Y + Ball[i].Position_X <= UpBroadNearHoleL_X + UpBroadNearHoleD_Y + K_MoveOut)
			{
				isReflect = 1;
				if(i != 0)
					Pool.Reflect ++; 
				else
					Rotation.Reflects ++;
				if((Ball[i].Velocity_X * Ball[i].Velocity_X + Ball[i].Velocity_Y * Ball[i].Velocity_Y) >= K_AvoidFix)
				{	
					while(Ball[i].Position_Y + Ball[i].Position_X <= UpBroadNearHoleL_X + UpBroadNearHoleD_Y + K_MoveOut)
					{
						Ball[i].Position_X += 0.01;
						Ball[i].Position_Y += 0.01;
					}
				} 
				float temp = Ball[i].Velocity_Y;
				Ball[i].Velocity_Y = - Ball[i].Velocity_X;
				Ball[i].Velocity_X = - temp;
				Ball[i].Velocity_X *= K_Attenuation;
				Ball[i].Velocity_Y *= K_Attenuation; 
				Ball[i].Position_X -= Ball[i].Velocity_X * K_Swift; 
				Ball[i].Position_Y -= Ball[i].Velocity_Y * K_Swift; 
			}
			else
			{
				float Next_X = Ball[i].Position_X + Ball[i].Velocity_X * K_VXSwitch;
				float Next_Y = Ball[i].Position_Y +	Ball[i].Velocity_Y * K_VXSwitch;
				if(Next_X + Next_Y <= UpBroadNearHoleL_X + UpBroadNearHoleD_Y + K_MoveOut)
				{
					while(Next_X + Next_Y <= UpBroadNearHoleL_X + UpBroadNearHoleD_Y + K_MoveOut)
					{
						Next_X += 0.01;
						Next_Y += 0.01;
					}
					isReflect = 1;
					if(i != 0)
						Pool.Reflect ++; 
					else
						Rotation.Reflects ++; 
					Ball[i].Position_X = Next_X;
					Ball[i].Position_Y = Next_Y;
					float temp = Ball[i].Velocity_Y;
					Ball[i].Velocity_Y = - Ball[i].Velocity_X;
					Ball[i].Velocity_X = - temp;
					Ball[i].Velocity_X *= K_Attenuation;
					Ball[i].Velocity_Y *= K_Attenuation; 										
				}
				else;
			} 
		}
		if((Ball[i].Position_X <= UpBroadNearHoleR_X && Ball[i].Position_X >= 664 - Ball_R) && (Ball[i].Position_Y >= UpBroadNearHoleU_Y && Ball[i].Position_Y <= UpBroadNearHoleD_Y + 8 * 1.414))
		{
			if(Ball[i].Position_Y - Ball[i].Position_X <= - UpBroadNearHoleR_X + UpBroadNearHoleD_Y + K_MoveOut)
			{
				isReflect = 1;
				if(i != 0)
					Pool.Reflect ++; 
				else
					Rotation.Reflects ++;
				if((Ball[i].Velocity_X * Ball[i].Velocity_X + Ball[i].Velocity_Y * Ball[i].Velocity_Y) >= K_AvoidFix)	 
				{ 
					while(Ball[i].Position_Y - Ball[i].Position_X <= - UpBroadNearHoleR_X + UpBroadNearHoleD_Y + K_MoveOut)
					{
						Ball[i].Position_X -= 0.01;
						Ball[i].Position_Y += 0.01;
					}
				} 
				float temp = Ball[i].Velocity_Y;
				Ball[i].Velocity_Y = Ball[i].Velocity_X;
				Ball[i].Velocity_X = temp;
				Ball[i].Velocity_X *= K_Attenuation;
				Ball[i].Velocity_Y *= K_Attenuation;
				Ball[i].Position_X -= Ball[i].Velocity_X * K_Swift; 
				Ball[i].Position_Y -= Ball[i].Velocity_Y * K_Swift; 
			}
			else
			{
				float Next_X = Ball[i].Position_X + Ball[i].Velocity_X * K_VXSwitch;
				float Next_Y = Ball[i].Position_Y +	Ball[i].Velocity_Y * K_VXSwitch;
				if(Next_X + Next_Y <= - UpBroadNearHoleR_X + UpBroadNearHoleD_Y + K_MoveOut)
				{
					while(Next_X + Next_Y <= - UpBroadNearHoleR_X + UpBroadNearHoleD_Y + K_MoveOut)
					{
						Next_X -= 0.01;
						Next_Y += 0.01;
					}
					isReflect = 1;
					if(i != 0)
						Pool.Reflect ++; 
					else
						Rotation.Reflects ++; 
					Ball[i].Position_X = Next_X;
					Ball[i].Position_Y = Next_Y;
					float temp = Ball[i].Velocity_Y;
					Ball[i].Velocity_Y = Ball[i].Velocity_X;
					Ball[i].Velocity_X = temp;
					Ball[i].Velocity_X *= K_Attenuation;
					Ball[i].Velocity_Y *= K_Attenuation; 										
				}
				else;
			} 
		}
		if((Ball[i].Position_X >= DownBroadNearHoleL_X && Ball[i].Position_X <= 616 + Ball_R) && (Ball[i].Position_Y >= DownBroadNearHoleU_Y - 8 * 1.414 && Ball[i].Position_Y <= DownBroadNearHoleD_Y))
		{
			if(Ball[i].Position_Y - Ball[i].Position_X >= - DownBroadNearHoleL_X + DownBroadNearHoleU_Y - K_MoveOut)
			{
				isReflect = 1;
				if(i != 0)
					Pool.Reflect ++; 
				else
					Rotation.Reflects ++; 
				if((Ball[i].Velocity_X * Ball[i].Velocity_X + Ball[i].Velocity_Y * Ball[i].Velocity_Y) >= K_AvoidFix)
				{ 
					while(Ball[i].Position_Y - Ball[i].Position_X >= - DownBroadNearHoleL_X + DownBroadNearHoleU_Y - K_MoveOut)
					{
						Ball[i].Position_X += 0.01;
						Ball[i].Position_Y -= 0.01;
					}
				} 
				float temp = Ball[i].Velocity_Y;
				Ball[i].Velocity_Y = Ball[i].Velocity_X;
				Ball[i].Velocity_X = temp;
				Ball[i].Velocity_X *= K_Attenuation;
				Ball[i].Velocity_Y *= K_Attenuation; 
				Ball[i].Position_X -= Ball[i].Velocity_X * K_Swift; 
				Ball[i].Position_Y -= Ball[i].Velocity_Y * K_Swift; 
			}
			else
			{
				float Next_X = Ball[i].Position_X + Ball[i].Velocity_X * K_VXSwitch;
				float Next_Y = Ball[i].Position_Y +	Ball[i].Velocity_Y * K_VXSwitch;
				if(Next_Y - Next_X >= - DownBroadNearHoleL_X + DownBroadNearHoleU_Y - K_MoveOut)
				{
					while(Next_Y - Next_X >= - DownBroadNearHoleL_X + DownBroadNearHoleU_Y - K_MoveOut)
					{
						Next_X += 0.01;
						Next_Y -= 0.01;
					}
					isReflect = 1;
					if(i != 0)
						Pool.Reflect ++; 
					else
						Rotation.Reflects ++; 
					Ball[i].Position_X = Next_X;
					Ball[i].Position_Y = Next_Y;
					float temp = Ball[i].Velocity_Y;
					Ball[i].Velocity_Y = Ball[i].Velocity_X;
					Ball[i].Velocity_X = temp;
					Ball[i].Velocity_X *= K_Attenuation;
					Ball[i].Velocity_Y *= K_Attenuation; 										
				}
				else;
			}
		}
		if((Ball[i].Position_X <= DownBroadNearHoleR_X && Ball[i].Position_X >= 664 - Ball_R) && (Ball[i].Position_Y >= DownBroadNearHoleU_Y - 8 * 1.414 && Ball[i].Position_Y <= DownBroadNearHoleD_Y))
		{
			if(Ball[i].Position_Y + Ball[i].Position_X >= DownBroadNearHoleR_X + DownBroadNearHoleU_Y - K_MoveOut)
			{
				isReflect = 1;
				if(i != 0)
					Pool.Reflect ++; 
				else
					Rotation.Reflects ++; 
				if((Ball[i].Velocity_X * Ball[i].Velocity_X + Ball[i].Velocity_Y * Ball[i].Velocity_Y) >= K_AvoidFix)
				{ 
					while(Ball[i].Position_Y + Ball[i].Position_X >= DownBroadNearHoleR_X + DownBroadNearHoleU_Y - K_MoveOut)
					{
						Ball[i].Position_X -= 0.01;
						Ball[i].Position_Y -= 0.01;
					}
				} 
				float temp = Ball[i].Velocity_Y;
				Ball[i].Velocity_Y = - Ball[i].Velocity_X;
				Ball[i].Velocity_X = - temp;
				Ball[i].Velocity_X *= K_Attenuation;
				Ball[i].Velocity_Y *= K_Attenuation; 
				Ball[i].Position_X -= Ball[i].Velocity_X * K_Swift; 
				Ball[i].Position_Y -= Ball[i].Velocity_Y * K_Swift; 
			}
			else
			{
				float Next_X = Ball[i].Position_X + Ball[i].Velocity_X * K_VXSwitch;
				float Next_Y = Ball[i].Position_Y +	Ball[i].Velocity_Y * K_VXSwitch;
				if(Next_Y + Next_X >= DownBroadNearHoleR_X + DownBroadNearHoleU_Y - K_MoveOut)
				{
					while(Next_Y + Next_X >= DownBroadNearHoleR_X + DownBroadNearHoleU_Y - K_MoveOut)
					{
						Next_X -= 0.01;
						Next_Y -= 0.01;
					}
					isReflect = 1;
					if(i != 0)
						Pool.Reflect ++; 
					else
						Rotation.Reflects ++; 
					Ball[i].Position_X = Next_X;
					Ball[i].Position_Y = Next_Y;
					float temp = Ball[i].Velocity_Y;
					Ball[i].Velocity_Y = - Ball[i].Velocity_X;
					Ball[i].Velocity_X = - temp;
					Ball[i].Velocity_X *= K_Attenuation;
					Ball[i].Velocity_Y *= K_Attenuation; 										
				}
				else;
			}
		}
		if(i == 0 && isReflect)
		{
			//printf("%.2f, %.2f\n", Ball[0].Velocity_X, Ball[0].Velocity_Y);
			if(Rotation.LeftRotation == 1)
			{
				VECTOR Velocity = {Ball[i].Velocity_X, Ball[i].Velocity_Y};
				ReflectRotation(Velocity, ReflectTheta_Max, Rotation.RotationRatio);
				Ball[i].Velocity_X = Velocity[0];
				Ball[i].Velocity_Y = Velocity[1];
				Rotation.RotationRatio *= 0.5; 
			} 
			else if(Rotation.RightRotation == 1)
			{
				VECTOR Velocity = {Ball[i].Velocity_X, Ball[i].Velocity_Y};
				ReflectRotation(Velocity, - ReflectTheta_Max, Rotation.RotationRatio);
				Ball[i].Velocity_X = Velocity[0];
				Ball[i].Velocity_Y = Velocity[1];
				Rotation.RotationRatio *= 0.5;
			}
			//printf("%.2f, %.2f\n", Ball[0].Velocity_X, Ball[0].Velocity_Y);
		}
	}
	if(isReflect)
		PostThreadMessage(CollisionMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_Reflect); 
	isReflect = 0;
	
	/* 与其他球体的碰撞 */
	float Distance_Min[NUM][2];				//记录与球体距离最近的球的距离和下标
	float Distance1, Distance2;
	float Cosine, Sine;
	float Distance_Re;						//两球碰撞后应当被直接补偿的距离，防止黏着反复判定 
	for(i = 0; i < NUM; i ++)				//初始化每个球对应的球的最小距离和下标 
	{
		Distance_Min[i][0] = 99999;
		Distance_Min[i][1] = -1;
	}
	for(i = 0; i < NUM - 1; i ++) 			//记录每个球对应的球的最小距离和下标【可以优化成遍历一半】 
	{
		if(Ball[i].State == INHOLE)
			continue;
		for(j = i + 1; j < NUM; j++)
		{
			if(Ball[j].State == INHOLE)
				continue;
			float Distance0;
			Distance0 = Distance(Ball[i].Position_X, Ball[i].Position_Y, Ball[j].Position_X, Ball[j].Position_Y);
			if(Distance_Min[i][0] > Distance0)
			{
				Distance_Min[i][0] = Distance0;
				Distance_Min[i][1] = j;
			}
			if(Distance_Min[j][0] > Distance0)
			{
				Distance_Min[j][0] = Distance0;
				Distance_Min[j][1] = i;
			}
		}
	}
	
	for(i = 0; i < NUM; i ++)
	{
		if(Distance_Min[i][0] <= 2 * Ball_R)
		{
			isCollision = 1;
			j = Distance_Min[i][1];
			if(Ball[j].State == INHOLE)
				continue;
			Distance_Re = 2 * Ball_R - Distance_Min[i][0] + 2;		//计算碰撞后应当补偿的距离
			Distance1 = Ball[i].Position_X - Ball[j].Position_X; 
			Distance2 = Ball[i].Position_Y - Ball[j].Position_Y;
			Cosine = sqrt(Distance1 * Distance1 / (Distance1 * Distance1 + Distance2 * Distance2));
			Sine = sqrt(Distance2 * Distance2 / (Distance1 * Distance1 + Distance2 * Distance2));
			if(Distance1 > 0)
				Cosine = - Cosine;
			if(Distance2 > 0)
				Sine = - Sine;
			Ball[j].Position_X += Distance_Re * Cosine;				//给球 j补偿距离防止黏着反复判定 
			Ball[j].Position_Y += Distance_Re * Sine;				
		}	
		else
			if(i == 0)
			{
				j = Distance_Min[i][1];
				if((Ball[i].Velocity_X || Ball[i].Velocity_Y) && Ball[j].Velocity_X == 0 && Ball[j].Velocity_Y == 0)
				{
					VECTOR MoveVec = {Ball[i].Velocity_X, Ball[i].Velocity_Y};
					VECTOR NextPos = {Ball[i].Position_X + K_VXSwitch * K_Friction * Ball[i].Velocity_X, Ball[i].Position_Y + K_VXSwitch * K_Friction * Ball[i].Velocity_Y};
					if(Distance(Ball[j].Position_X, Ball[j].Position_Y, NextPos[0], NextPos[1]) < 2 * Ball_R)
					{
						//printf("Fix - Case1\n");
						Rotation.Reflects ++;
						isCollision = 1;
						Ball[i].Position_X = NextPos[0];
						Ball[i].Position_Y = NextPos[1];
						while(Distance(Ball[j].Position_X, Ball[j].Position_Y, Ball[i].Position_X, Ball[i].Position_Y) <= 2 * Ball_R)
						{
							Ball[i].Position_X -= 0.02 * Ball[i].Velocity_X;
							Ball[i].Position_Y -= 0.02 * Ball[i].Velocity_Y;
						}
						Distance_Min[0][0] = 2 * Ball_R;
					}
					else			//特别判定，如果中途会略过球体 
					{
						int ifNeedFix = 0, MaxTimes = 50, Times;
						float PositionCur_X, PositionCur_Y;
						for(Times = 1; Times < MaxTimes + 1; Times ++)
						{
							PositionCur_X = Ball[i].Position_X + 0.02 * Times * K_VXSwitch * K_Friction * Ball[i].Velocity_X;
							PositionCur_Y = Ball[i].Position_Y + 0.02 * Times * K_VXSwitch * K_Friction * Ball[i].Velocity_Y;
							float Distance_Cur = Distance(PositionCur_X, PositionCur_Y, Ball[j].Position_X, Ball[j].Position_Y);
							if(Distance_Cur < 2 * Ball_R)
							{	
								Ball[i].Position_X = PositionCur_X;
								Ball[i].Position_Y = PositionCur_Y;
								ifNeedFix = 1;
								break;
							}
						}
						if(ifNeedFix)
						{
							//printf("Fix - Case2\n");
							while(Distance(Ball[j].Position_X, Ball[j].Position_Y, Ball[i].Position_X, Ball[i].Position_Y) < 2 * Ball_R)
							{
								Rotation.Reflects ++;
								isCollision = 1;
								Ball[i].Position_X -= 0.02 * Ball[i].Velocity_X;
								Ball[i].Position_Y -= 0.02 * Ball[i].Velocity_Y;
							}
							//printf("Distance Now: %.2f\n", Distance(Ball[j].Position_X, Ball[j].Position_Y, Ball[i].Position_X, Ball[i].Position_Y));
							//system("Pause");
							Distance_Min[0][0] = 2 * Ball_R;							
						}
					}
				}				
			}
	}
	if(isCollision) 
		PostThreadMessage(CollisionMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_Collide); 
	isCollision = 0;
	
	for(i = 0; i < NUM; i ++)
	{
		float Temp;
		if(Distance_Min[i][0] <= 2 * Ball_R && Ball[i].Moving == MOVING)
		{
			j = Distance_Min[i][1];
			Distance1 = Ball[i].Position_X - Ball[j].Position_X; 
			Distance2 = Ball[i].Position_Y - Ball[j].Position_Y;
			VECTOR UnitDistance = {Distance1, Distance2};			//两球球心连线单位向量，同时确定了水平投影方向
			VECTOR UnitDistance_Rot = {- Distance2, Distance1};		//两球球心连线旋转向量，同时确定了垂直投影方向
			//将两个向量单位化 
			UnitVec(UnitDistance);
			UnitVec(UnitDistance_Rot); 
			//建立投影速度向量 
			VECTOR Velocity1 = {Ball[i].Velocity_X * UnitDistance[0] + Ball[i].Velocity_Y * UnitDistance[1], Ball[i].Velocity_X * UnitDistance_Rot[0] + Ball[i].Velocity_Y * UnitDistance_Rot[1]};			//投影化速度向量 
			VECTOR Velocity2 = {Ball[j].Velocity_X * UnitDistance[0] + Ball[j].Velocity_Y * UnitDistance[1], Ball[j].Velocity_X * UnitDistance_Rot[0] + Ball[j].Velocity_Y * UnitDistance_Rot[1]}; 			//投影化速度向量
			//水平投影方向速度互换 
			Temp = Velocity1[0];
			Velocity1[0] = Velocity2[0];
			Velocity2[0] = Temp;		
			if(i == 0 && j != 0 && Ball[i].Moving == MOVING || i != 0 && j == 0 && Ball[j].Moving == MOVING)
				if(Rotation.FrontRotation)
					Velocity1[0] += ImpactOffset * Velocity2[0] * Rotation.MoveRatio;
				else if(Rotation.BackRotation)
					Velocity1[0] -= ImpactOffset * Velocity2[0] * Rotation.MoveRatio;
			//建立分配后速度向量 
			VECTOR UnitVelocity1 = {Velocity1[0] * UnitDistance[0], Velocity1[0] * UnitDistance[1]};					//分配后水平投影速度向量 1
			VECTOR UnitVelocity1_Rot = {Velocity1[1] * UnitDistance_Rot[0], Velocity1[1] * UnitDistance_Rot[1]}; 		//分配后垂直投影速度向量 1
			VECTOR UnitVelocity2 = {Velocity2[0] * UnitDistance[0], Velocity2[0] * UnitDistance[1]};					//分配后水平投影速度向量 2
			VECTOR UnitVelocity2_Rot = {Velocity2[1] * UnitDistance_Rot[0], Velocity2[1] * UnitDistance_Rot[1]}; 		//分配后垂直投影速度向量 2
			//速度重新赋值 
			Ball[i].Velocity_X = UnitVelocity1[0] + UnitVelocity1_Rot[0]; 
			Ball[i].Velocity_Y = UnitVelocity1[1] + UnitVelocity1_Rot[1];
			Ball[j].Velocity_X = UnitVelocity2[0] + UnitVelocity2_Rot[0]; 
			Ball[j].Velocity_Y = UnitVelocity2[1] + UnitVelocity2_Rot[1];
			Ball[j].Moving = MOVING;
			//避免二次交换 
			Distance_Min[j][0] = 99999;
			Distance_Min[j][1] = -1;
		}
	} 
}

void Move(int i)
{
	if(GameMode == GM_POOL)
	{
		if(Pool.TargetBall == 0 && i != 0 && (Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0))
			Pool.TargetBall = i;
		if(Pool.SolidMoved == 0 && (1 <= i && i <= 7) && (Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0))
		{
			Pool.SolidMoved = 1;
			//printf("Pool: SolidMoved = %d\n", Pool.SolidMoved);
		}
			
		if(Pool.StripMoved == 0 && (9 <= i && i <= 15) && (Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0))
		{
			Pool.StripMoved = 1;
			//printf("Pool: StripMoved = %d\n", Pool.StripMoved);
		}	
	}
	if(GameMode == GM_SNOOKER)
		if(Snooker.TargetBall == 0 && i != 0 && (Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0))
			Snooker.TargetBall = i;
	
	if(i != 0 && Ball[i].Velocity_X == 0 && Ball[i].Velocity_Y == 0)
	{
		Ball[i].Moving = STOP;
		return; 
	}	
	else if(i == 0)
		if(Ball[i].Velocity_X == 0 && Ball[i].Velocity_Y == 0 && Rotation.Acceleration == 0)
		{
			Ball[0].Moving = STOP;
			return; 
		}		
	Ball[i].Moving = MOVING;
	Ball[i].Position_X += K_VXSwitch * Ball[i].Velocity_X;
	Ball[i].Position_Y += K_VXSwitch * Ball[i].Velocity_Y;
} 

void Friction(int i)
{
	Ball[i].Velocity_X *= K_Friction;
	Ball[i].Velocity_Y *= K_Friction;
	K_Friction -= 0.000002;
	//printf("%.5f\n", K_Friction); 
}

void Stop(int i)
{
	if(i != 0)
	{
		if(sqrt(Ball[i].Velocity_X * Ball[i].Velocity_X + Ball[i].Velocity_Y * Ball[i].Velocity_Y) <= K_StopV)
		{
			Ball[i].Velocity_X = 0;
			Ball[i].Velocity_Y = 0;
			Ball[i].Moving = STOP;
		}
	}
	else
	{
		if(Rotation.Acceleration == 0 && sqrt(Ball[i].Velocity_X * Ball[i].Velocity_X + Ball[i].Velocity_Y * Ball[i].Velocity_Y) <= K_StopV)
		{
			Ball[i].Velocity_X = 0;
			Ball[i].Velocity_Y = 0;
			Ball[i].Moving = STOP;
		}
	}
}

void ScoreJudge()
{ 
	int i, j, k, Random_X, Random_Y;
	int isGoal = 0, isBallInhole = 0;
	double Distance = 0;
	for(i = 0; i < NUM; i ++)
	{
		for(j = 0; j < 6; j ++)
		{
			isBallInhole = 0;
			Distance = sqrt((Ball[i].Position_X - Hole_X[j]) * (Ball[i].Position_X - Hole_X[j]) + (Ball[i].Position_Y - Hole_Y[j]) * (Ball[i].Position_Y - Hole_Y[j]));
			if(j != 1 && j != 4 && Distance + Ball_R <= Hole_R)
				isBallInhole = 1;
			else if(j == 1)
			{
				if(Ball[i].Position_X <= Hole2_C && Ball[i].Position_X >= Hole2_A && Ball[i].Position_Y <= Hole2_D && Ball[i].Position_Y >= Hole2_D - 120)
					isBallInhole = 1;	
			}
			else if(j == 4)
			{
				if(Ball[i].Position_X <= Hole5_C && Ball[i].Position_X >= Hole5_A && Ball[i].Position_Y >= Hole5_B && Ball[i].Position_Y <= Hole5_B + 120)
					isBallInhole = 1;
			}
			if(!isBallInhole)
				continue;
			else
			{ 
				WhichHole[i] = j;
				Ball[i].Velocity_X = 0;
				Ball[i].Velocity_Y = 0;
				Ball[i].State = INHOLE;
				Draw_R[i] -= 0.2;
				if(i != 0)
				{
					isGoal = 1;
					if(GameMode == GM_TRAINNING)
					{
						Trainning.IfScored[i] = 0;
						Trainning.NewInhole[i] = 1;
					}
					else if(GameMode == GM_SINGLEPLAYER || GameMode == GM_MULTIPLAYER)
					{
						Classic.IfScored[i] = 0;
						Classic.NewInhole[i] = 1;
						if(Classic.AgainTimes >= 1)
						{
							if(!Sides)
								Classic.AgainScore1 += Classic.AgainTimes;
							else
								Classic.AgainScore2 += Classic.AgainTimes;
						}
						Classic.AgainTimes ++;
						//printf("AgainTimes: %d\n", Classic.AgainTimes);
					}
				
					if(GameMode != GM_POOL && GameMode != GM_SNOOKER)
					{
						if(!Sides)
						{
							Classic.Player1LastScored = i;
							printf("Player1LastScored = %d\n", Classic.Player1LastScored);
							Ball[i].Position_X = 33;
							Ball[i].Position_Y = Shelf_Y[Classic.Player1Scored ++];
						}
						else
						{
							Classic.Player2LastScored = i;
							printf("Player2LastScored = %d\n", Classic.Player2LastScored);
							Ball[i].Position_X = 1247;
							Ball[i].Position_Y = Shelf_Y[Classic.Player2Scored ++];
						}						
					}
					else if(GameMode == GM_POOL)
					{
						if(Pool.isSideConfirmed && Pool.isSuccessfullyBegin)
						{
							if(1 <= i && i <= 7)
							{
								if(Pool.SideSolid == 0)
									Ball[i].Position_X = 33;
								else
									Ball[i].Position_X = 1247;
								Ball[i].Position_Y = Shelf_Y[Pool.SolidSet ++];					//采用SolidSet而不用SolidInhole的原因：因为SolidInhole不是实时刷新的，需要GetPoolInfo读入 
							}
							if(9 <= i && i <= 15)
							{
								if(Pool.SideStrip == 0)
									Ball[i].Position_X = 33;
								else
									Ball[i].Position_X = 1247;
								Ball[i].Position_Y = Shelf_Y[Pool.StripSet ++];								
							}
							if(i == 8)
							{
								Ball[i].Position_X = - Ball_R;
								Ball[i].Position_Y = - Ball_R;
								/*if(Pool.SideClear[Sides])
								{
									if(Sides == 0)
										Ball[i].Position_X = ViolationShelf_X[0][0];
									else
										Ball[i].Position_X = ViolationShelf_X[1][0];
									Ball[i].Position_Y = 698 + TITLEHEIGHT;
								}*/
							}							
						}
						else
						{
							if(Pool.isSet == 0)						//这一段对应还未确定花色时进的球，暂且都移到界外，在花色确定程序内完成摆放，摆放完成后 isSet 改为 1  
							{
								if(i == 8)
								{
									Ball[i].Position_X = - Ball_R;
									Ball[i].Position_Y = - Ball_R;
									/*if(Pool.SideClear[Sides])
									{
										if(Sides == 0)
											Ball[i].Position_X = ViolationShelf_X[0][0];
										else
											Ball[i].Position_X = ViolationShelf_X[1][0];
										Ball[i].Position_Y = 698 + TITLEHEIGHT;
									}*/
								}
								//printf("Detected Once - %.0f\n", FixConst);	
								Ball[i].Position_X = - Ball_R;
								Ball[i].Position_Y = - Ball_R - FixConst * 2 * Ball_R;
								FixConst += 1;
							}
						}
					}
					else if(GameMode == GM_SNOOKER)
					{
						if(i >= 7)
						{
							if(!Sides)
							{
								Ball[i].Position_X = 33;
								Ball[i].Position_Y = Shelf_Y[Snooker.Player1Scored ++];
							}
							else
							{
								Ball[i].Position_X = 1247;
								Ball[i].Position_Y = Shelf_Y[Snooker.Player2Scored ++];
							}							
						}
						else
						{
							if(!Snooker.isRedClear || Snooker.WrongGoal)
							{
								Ball[i].Position_X = - Ball_R;
								Ball[i].Position_Y = - Ball_R - FixConst * 2 * Ball_R;
								FixConst ++;						
							}
							else
							{
								if(Snooker.Score == 0)
									Snooker.Score = i;
								else if(Snooker.Score == Snooker.NowTime)
								{
									if(!Sides)
									{
										Ball[i].Position_X = 33;
										Ball[i].Position_Y = Shelf_Y[Snooker.Player1Scored ++];
									}
									else
									{
										Ball[i].Position_X = 1247;
										Ball[i].Position_Y = Shelf_Y[Snooker.Player2Scored ++];
									}
								}
								else if(Snooker.Score != Snooker.NowTime)
								{
									Ball[i].Position_X = - Ball_R;
									Ball[i].Position_Y = - Ball_R - FixConst * 2 * Ball_R;
									FixConst ++;									
								}
							}
						}
												
					}
				}
				else
				{
					isGoal = 1;
					Violation = 1;
					if(GameMode == GM_POOL)
						Pool.isCueInhole = 1;
					else if(GameMode == GM_SNOOKER)
					Ball[i].Position_X = 1280 * 2;
					Ball[i].Position_Y = 720 * 2;
					if(GameMode == GM_SINGLEPLAYER || GameMode == GM_MULTIPLAYER)
					{
						if(!Sides)
							Classic.Player1Violation ++;
						else
							Classic.Player2Violation ++;					
					}
				}
			}
		}
		if(isGoal)
		{
			//播放声音
			PostThreadMessage(CollisionMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_Inhole);
			isGoal = 0;
		} 	
	}
}

void PowerToVelocity(float Power, VECTOR UnitVector)
{
	float Velocity = MaxVelocity * Power / 100;
	Ball[0].Velocity_X = Velocity * UnitVector[0];
	Ball[0].Velocity_Y = Velocity * UnitVector[1]; 
}

int const Dot_R = 5;
int HitPosition_X = 639;
int HitPosition_Y = 665 + TITLEHEIGHT;
int CurrentPosition_X = -1;
int CurrentPosition_Y = -1;
int CueBallPosition_X = -1;
int CueBallPosition_Y = -1;
int HitStopKey = 0;

void PowerControl()
{
	//printf("%d %d %f %f\n",CurrentPosition_X,CurrentPosition_Y,Ball[0].Position_X,Ball[0].Position_Y);
	DirectionVec[0] = CurrentPosition_X - Ball[0].Position_X;
	DirectionVec[1] = CurrentPosition_Y - Ball[0].Position_Y;								//建立射击方向向量 
	UnitVec(DirectionVec);										//将方向向量单位化 
	//printf("[%.2f, %.2f]\n", DirectionVec[0], DirectionVec[1]); 
	PowerToVelocity(Power, DirectionVec);						//将射击力度和射击方向转换为母球的速度 
}

void InitParameter()
{
	ForceBarStep = 1.5 + ForceBarSpeed / 40;
	int i;
	for(i = 0; i < 15; i ++)
		Shelf_Y[i] = 32 + TITLEHEIGHT + i * ShelfGap;
	if(ifLoad){
		UpdtSticks();
		return;
	}
	if(GameMode == GM_POOL || GameMode == GM_TRAINNING)
		NUM = 16;
	else if(GameMode == GM_SINGLEPLAYER || GameMode == GM_MULTIPLAYER)
		NUM = 10;
	else
		NUM = 22;
	MOUSESTATE MOUSE_STATE = MS_NULL;
	KEYBOARDSTATE KEYBOARD_STATE = KB_NULL; 
	if(GameMode != GM_SINGLEPLAYER)
		Sides = rand() % 2;					//随机指定开球方
	else
		Sides = 0;
	UpdtSticks();
	InitRotation();						//加塞常量初始化
	Violation = 0;						//<要存档> 所有模式共用的犯规标记 
	InitBallVelocity();
	switch(GameMode)
	{
		case GM_SINGLEPLAYER: case GM_MULTIPLAYER: InitClassic(); break;
		case GM_POOL: InitPool(); break;
		case GM_TRAINNING: InitTrainning(); break;
		case GM_SNOOKER: InitSnooker(); break; 
	}
}

void InitBallVelocity()
{
	int i;
	for(i = 0; i < 22; i ++)
	{
		Ball[i].Velocity_X = 0;
		Ball[i].Velocity_Y = 0;
		Ball[i].Moving = STOP;
		Ball[i].State = ONTABLE;
		Ball[i].Score = 0;
		Ball[i].IfScored = 0;
		InHoleFinished[i] = false;
		Draw_R[i] = 16.0;
		WhichHole[i] = -1;
	}
}

void InitSnooker()
{
	int i;
	Snooker.isRedClear = 0;
	Snooker.isRedTurn = 1;
	Snooker.TargetBall = 0;
	Snooker.Score = 0;
	Snooker.Player1Score = 0;
	Snooker.Player2Score = 0;
	Snooker.Player1Scored = 0;
	Snooker.Player2Scored = 0;
	Snooker.isCueInhole = 0;
	Snooker.isColourScored = 0;
	Snooker.isRedScored = 0;
	Snooker.NowTime = 0;
	Snooker.WrongGoal = 0;
	Snooker.Skip = 0;
	Snooker.End = 0;
	Snooker.Winner = 0;
	for(i = 0; i < 22; i ++)
		Snooker.NewInhole[i] = Snooker.BallState[i] = 0;
}

void InitPool()
{
	int i;
	Pool.Collide = 0;
	Pool.SolidMoved = 0;
	Pool.StripMoved = 0;
	Pool.DoubleScored = false;
	Pool.End = 0;
	Pool.Inhole = 0;
	Pool.isBlackInhole = false;
	Pool.isCueInhole = false;
	Pool.SideClear[0] = false;
	Pool.SideClear[1] = false;
	Pool.isSideConfirmed = 0;
	Pool.isSuccessfullyBegin = 0;
	Pool.Ontable = 15;
	Pool.Reflect = 0;
	Pool.Scored = 0;
	Pool.SideSolid = -1;
	Pool.SideStrip = -1;
	Pool.SolidInhole = 0;
	Pool.StripInhole = 0;
	Pool.TargetBall = 0;
	Pool.Turns = 0;
	Pool.SolidSet = 0;
	Pool.StripSet = 0;
	Pool.isSet = 0;
	Pool.Winner = 0; 
	for(i = 0; i < 16; i ++)
		Pool.NewInhole[i] = Pool.BallState[i] = 0;	
}

void InitClassic()
{
	int i;
	Classic.Player1Scored = 0;
	Classic.Player2Scored = 0;
	Classic.Player1LastScored = 0;
	Classic.Player2LastScored = 0;
	Classic.Player1Violation = 0;
	Classic.Player2Violation = 0;
	Classic.End = 0;
	Classic.Winner = -1;
	//如果要测试平局情况，注释掉以下四行 
	Classic.ScorePlayer1 = 0;
	Classic.ScorePlayer2 = 0;
	Classic.AgainScore1 = 0;
	Classic.AgainScore2 = 0;
	Classic.AgainTimes = 0;
	for(i = 0; i < 10; i ++)
		Classic.NewInhole[i] = Classic.IfScored[i] = 0;
	if(GameMode == GM_SINGLEPLAYER)
		AI_Init();
}

void InitTrainning()
{
	int i;
	Trainning.Score = 0;
	for(i = 0; i < 16; i ++)
		Trainning.NewInhole[i] = Trainning.IfScored[i] = 0;
}

void InitRotation()
{	
	Rotation.Reflects = 0;
	Rotation.AccelerationMax = Acceleration_Max;
}

void OverallDraw()
{
	int i;
	switch(GameMode)
	{
		case GM_SINGLEPLAYER:
		{
			for(i = 0; i < NUM; i ++)
			{
				if(i != 0 && Classic.NewInhole[i] == 1 && Classic.IfScored[i] == 0)
				{
					if(!Sides)
						Classic.ScorePlayer1 = 2 * Classic.Player1Scored - (Classic.Player1Violation * Classic.Player1Violation + 5 * Classic.Player1Violation) / 2 + Classic.AgainScore1; 
					else
						Classic.ScorePlayer2 = 2 * Classic.Player2Scored - (Classic.Player2Violation * Classic.Player2Violation + 5 * Classic.Player2Violation) / 2 + Classic.AgainScore2;
					Classic.PlayerScored = true;	
					Classic.IfScored[i] = 1;
					Classic.NewInhole[i] = 0;
				}
				if(Violation)
				{
					if(!Sides)
						Classic.ScorePlayer1 = 2 * Classic.Player1Scored - (Classic.Player1Violation * Classic.Player1Violation + 5 * Classic.Player1Violation) / 2 + Classic.AgainScore1; 
					else
						Classic.ScorePlayer2 = 2 * Classic.Player2Scored - (Classic.Player2Violation * Classic.Player2Violation + 5 * Classic.Player2Violation) / 2 + Classic.AgainScore2;			
				}
				MoveUpdate(i);
			}	
			for(i = 0; i < NUM; i ++)
				if(Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0 || Draw_R[i] != 16) 
					break;
			if(i == NUM && Draw_R[0] == 16)
			{	
				K_Friction = 0.99;
				ClassicRules();
				if(Violation)
					Violation = 0;
				/*
				printf("NextStep\n");
				printf("Sides = %d\n", Sides); 
				*/
				Power = 0;
				HitPosition_X = 639;
				HitPosition_Y = 665 + TITLEHEIGHT;
				Classic.PlayerScored = false;
				UpdateTimes = 0;
				if(Classic.End == 1)
				{
					EndHit();
					LevelUpdate();
					StartWinMasks(StartWinMenu);
					return;
				}
				if(Ball[0].State == INHOLE)
				{
					EndHit();
					if(!Sides){
						ShowHintMsge("母球进洞：请选择合适位置摆放母球"); 
						StartFreeResetCueBall();
					} 
					else
					{
						StartCountDown();
						//停滞 1 秒 
						extern int RefreshRate;
						startTimer(TM_HIT_POSITION_CHANGE,RefreshRate);
						startTimer(TM_AI_SET_CUEBALL,1000);
					}
				}
				else
				{
					EndHit();
					if(!Sides)
						StartGetHit();
					else
					{
						StartCountDown();
						//停滞 1 秒 
						extern int RefreshRate;
						startTimer(TM_HIT_POSITION_CHANGE,RefreshRate);
						startTimer(TM_AI_PLAY,2000);
					}
				}
			}
		} break;
		
		case GM_MULTIPLAYER:
		{
			for(i = 0; i < NUM; i ++)
			{
				if(i != 0 && Classic.NewInhole[i] == 1 && Classic.IfScored[i] == 0)
				{
					if(!Sides)
						Classic.ScorePlayer1 = 2 * Classic.Player1Scored - (Classic.Player1Violation * Classic.Player1Violation + 5 * Classic.Player1Violation) / 2 + Classic.AgainScore1; 
					else
						Classic.ScorePlayer2 = 2 * Classic.Player2Scored - (Classic.Player2Violation * Classic.Player2Violation + 5 * Classic.Player2Violation) / 2 + Classic.AgainScore2;
					Classic.PlayerScored = true;	
					Classic.IfScored[i] = 1;
					Classic.NewInhole[i] = 0;
				}
				if(Violation)
				{
					if(!Sides)
						Classic.ScorePlayer1 = 2 * Classic.Player1Scored - (Classic.Player1Violation * Classic.Player1Violation + 5 * Classic.Player1Violation) / 2 + Classic.AgainScore1; 
					else
						Classic.ScorePlayer2 = 2 * Classic.Player2Scored - (Classic.Player2Violation * Classic.Player2Violation + 5 * Classic.Player2Violation) / 2 + Classic.AgainScore2;		
				}
				MoveUpdate(i);
			}	
			for(i = 0; i < NUM; i ++)
				if(Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0 || Draw_R[i] != 16) 
					break;
			if(i == NUM && Draw_R[0] == 16)
			{	
				K_Friction = 0.99;
				ClassicRules();
				if(Violation)
					Violation = 0;
				/*
				printf("NextStep\n");
				printf("Sides = %d\n", Sides); 
				*/
				Power = 0;
				HitPosition_X = 639;
				HitPosition_Y = 665 + TITLEHEIGHT;
				Classic.PlayerScored = false;
				UpdateTimes = 0;
				if(Classic.End == 1)
				{
					EndHit();
					LevelUpdate();
					StartWinMasks(StartWinMenu);
					return;
				}
				if(Ball[0].State == INHOLE)
				{
					EndHit();
					ShowHintMsge("母球进洞：请选择合适位置摆放母球"); 
					StartFreeResetCueBall();
				}
				else{
					EndHit();
					StartGetHit();
				}
			}
		} break;
		
		case GM_TRAINNING:
		{
			for(i = 0; i < NUM; i ++)
			{
				if(i != 0 && Trainning.NewInhole[i] && Trainning.IfScored[i] == 0)
				{
					Trainning.Score ++;
					Trainning.IfScored[i] = 1;
					Trainning.NewInhole[i] = 0;
				}
				MoveUpdate(i);
			}
			for(i = 0; i < NUM; i ++)
				if(Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0 || Draw_R[i] != 16) 
					break;
			if(i == NUM && Draw_R[0] == 16)
			{
				K_Friction = 0.99;
				Violation = 0;
				ExerciseBallReset();
				Power = 0;
				HitPosition_X = 639;
				HitPosition_Y = 665 + TITLEHEIGHT;
				UpdateTimes = 0;
				if(Ball[0].State == INHOLE){
					EndHit();
					ShowHintMsge("母球进洞：请选择合适位置摆放母球"); 
					StartFreeResetCueBall();
				}
				else{
					EndHit();
					StartGetHit();
				}
			}
		} break;
		
		case GM_POOL:
		{
			for(i = 0; i < 16; i ++)
			{
				MoveUpdate(i);	
			}
			for(i = 0; i < 16; i ++)
				if(Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0) 
					break;
			if(i == NUM && Draw_R[0] == 16)
			{	
				K_Friction = 0.99;
				int PrevisSideConfirmed = Pool.isSideConfirmed;
				PoolRules();
				/*
				printf("--------------------------\n");
				printf("isSideConfirmed: %d\n", Pool.isSideConfirmed);
				printf("SideSolid: %d\n", Pool.SideSolid);
				printf("SideStrip: %d\n", Pool.SideStrip);
				printf("ifEnd: %d\n", Pool.End);
				printf("isBlackInhole: %d\n", Pool.isBlackInhole);
				printf("Score: %d\n", Pool.Scored);
				printf("DoubleScore: %d\n", Pool.DoubleScored);
				*/
				if(Pool.End == 1)
				{
					EndHit();
					LevelUpdate();
					StartWinMasks(StartWinMenu);
					return;
				}
				if(!Violation);
				else
				{
					Sides = !Sides;
					UpdtSticks();
					PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_SwitchSide); 
				}
				if(Violation && Pool.Reflect == 0 && Pool.Scored == 0)
				{
					Violation = 0;
					Pool.Turns ++; 
					Pool.isCueInhole = 0;
					Power = 0;
					HitPosition_X = 639;
					HitPosition_Y = 665 + TITLEHEIGHT;
					Pool.Reflect = 0;
					Pool.TargetBall = 0;
					Pool.Scored = 0;
					UpdateTimes = 0;
					Ball[0].Position_X = -1000;
					Ball[0].Position_Y = -1000;
					EndHit();
					if(!Pool.isSuccessfullyBegin)
						ShowHintMsge("开球失败：请在左半区域内摆放母球");
					else
						ShowHintMsge("无效击球：请选择合适位置摆放母球"); 
					StartFreeResetCueBall();
					return;
				}
				if(PrevisSideConfirmed){
					if((Sides == Pool.SideSolid && !Violation)
					|| ((!Sides) == Pool.SideSolid && Violation))
					{
						if(Pool.SolidInhole >= 7)
						{
							if(Pool.TargetBall != 8)
							{
								Violation = 0;
								Pool.Turns ++; 
								Pool.isCueInhole = 0;
								Power = 0;
								HitPosition_X = 639;
								HitPosition_Y = 665 + TITLEHEIGHT;
								Pool.Reflect = 0;
								Pool.TargetBall = 0;
								Pool.Scored = 0;
								UpdateTimes = 0;
								Ball[0].Position_X = -1000;
								Ball[0].Position_Y = -1000;
								EndHit();
								ShowHintMsge("目标球错误：请选择合适位置摆放母球"); 
								StartFreeResetCueBall();
								return;
							}
						}
						else if(Pool.TargetBall >= 8)
						{
							Violation = 0;
							Pool.Turns ++; 
							Pool.isCueInhole = 0;
							Power = 0;
							HitPosition_X = 639;
							HitPosition_Y = 665 + TITLEHEIGHT;
							Pool.Reflect = 0;
							Pool.TargetBall = 0;
							Pool.Scored = 0;
							UpdateTimes = 0;
							Ball[0].Position_X = -1000;
							Ball[0].Position_Y = -1000;
							EndHit();
							ShowHintMsge("目标球错误：请选择合适位置摆放母球"); 
							StartFreeResetCueBall();
							return;
						}
					}
					else
					{
						if(Pool.StripInhole >= 7)
						{
							if(Pool.TargetBall != 8)
							{
								Violation = 0;
								Pool.Turns ++; 
								Pool.isCueInhole = 0;
								Power = 0;
								HitPosition_X = 639;
								HitPosition_Y = 665 + TITLEHEIGHT;
								Pool.Reflect = 0;
								Pool.TargetBall = 0;
								Pool.Scored = 0;
								UpdateTimes = 0;
								Ball[0].Position_X = -1000;
								Ball[0].Position_Y = -1000;
								EndHit();
								ShowHintMsge("目标球错误：请选择合适位置摆放母球"); 
								StartFreeResetCueBall();
								return;
							}
						}
						else if(Pool.TargetBall <= 8 && Pool.TargetBall >= 1)
						{
							Violation = 0;
							Pool.Turns ++; 
							Pool.isCueInhole = 0;
							Power = 0;
							HitPosition_X = 639;
							HitPosition_Y = 665 + TITLEHEIGHT;
							Pool.Reflect = 0;
							Pool.TargetBall = 0;
							Pool.Scored = 0;
							UpdateTimes = 0;
							Ball[0].Position_X = -1000;
							Ball[0].Position_Y = -1000;
							EndHit();
							ShowHintMsge("目标球错误：请选择合适位置摆放母球"); 
							StartFreeResetCueBall();
							return;
						}
					}
				}
				if(Violation)
					Violation = 0;
				Pool.Turns ++; 
				Pool.isCueInhole = 0;
				Pool.SolidMoved = 0;
				Pool.StripMoved = 0;
				//printf("NextStep\n");
				//printf("Sides = %d\n", Sides); 
				Power = 0;
				HitPosition_X = 639;
				HitPosition_Y = 665 + TITLEHEIGHT;
				Pool.Reflect = 0;
				Pool.TargetBall = 0;
				Pool.Scored = 0;
				UpdateTimes = 0;
				if(Ball[0].State == INHOLE || !Pool.isSuccessfullyBegin)
				{
					EndHit();
					if(!Pool.isSuccessfullyBegin)
						ShowHintMsge("开球失败：请在左半区域内摆放母球"); 
					else 
						ShowHintMsge("母球进洞：请选择合适位置摆放母球"); 
					StartFreeResetCueBall();
				}
				else{
					EndHit();
					StartGetHit();
				}
			}
		} break;	

		case GM_SNOOKER:
		{
			for(i = 0; i < 22; i ++)
			{
				MoveUpdate(i);	
			}
			for(i = 0; i < 22; i ++)
				if(Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0) 
					break;
			if(i == NUM && Draw_R[0] == 16)
			{
				GameJudger();	
				ResetBalls();
				if(Snooker.isRedClear)
				{
					for(i = 1; i < 22; i ++)
					{
						if(Ball[i].State == ONTABLE)
							break;	
					}	
					if(i >= 22)
					{
						if(Snooker.Player1Score != Snooker.Player2Score)
						{
							Snooker.End = 1;
							if(Snooker.Player1Score > Snooker.Player2Score)
								Snooker.Winner = 0;
							else
								Snooker.Winner = 1;				
						}
						else
						{
							ResetBlackBall();
							Snooker.NowTime --;
						}
					}
				}
				K_Friction = 0.99;
				/*
				printf("------------------------\n");
				printf("isRedTurn: %s\n", Snooker.isRedTurn?"Yes":"No");
				printf("TargetBall: %d\n", Snooker.TargetBall);
				printf("isRedInhole: %s\n", Snooker.isRedScored?"Yes":"No");
				printf("isColourInhole: %s\n", Snooker.isColourScored?"Yes":"No");
				printf("isRedClear: %s\n", Snooker.isRedClear?"Yes":"No");
				printf("NowScore: %d\n", Snooker.Score);
				printf("NowTime: %d\n", Snooker.NowTime);
				printf("isWrong: %s\n", Snooker.WrongGoal?"Yes":"No");	
				printf("ifEnd: %d\n", Snooker.End);
				printf("Winner: %d\n", Snooker.Winner);
				printf("------------------------\n");
				printf("NextStep\n");
				printf("Sides = %d\n", Sides); 
				*/
				Power = 0;
				HitPosition_X = 639;
				HitPosition_Y = 665 + TITLEHEIGHT;
				Snooker.isCueInhole = 0;
				Snooker.TargetBall = 0;
				Snooker.Score = 0;
				Snooker.WrongGoal = 0;
				UpdateTimes = 0;
				if(Snooker.End == 1)
				{
					EndHit();
					LevelUpdate();
					StartWinMasks(StartWinMenu);
					return;
				}
				if(Ball[0].State == INHOLE)
				{
					EndHit();
					ShowHintMsge("母球进洞：请在半圆区域内摆放母球"); 
					StartFreeResetCueBall();
				}
				else{
					EndHit();
					StartGetHit();
				}
			}
		} break;
	}
}

void AIFreeResetCueBall()
{
	int j;
	if(Ball[0].State == INHOLE)
	{
		float Random_X, Random_Y;			
		while(1)
		{
			Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
			Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
			if(!Ontable(Random_X, Random_Y))
				continue;
			for(j = 1; j < NUM; j ++)
				if(Distance(Random_X, Random_Y, Ball[j].Position_X, Ball[j].Position_Y) < 2 * Ball_R)
					break;
			if(j >= NUM)
			{
				Ball[0].Position_X = Random_X;
				Ball[0].Position_Y = Random_Y;
				Ball[0].State = ONTABLE;
				Ball[0].IfScored = 0;
				Ball[0].Score = 0;
				InHoleFinished[0] = false;
				break;
			}
		}
	}
}

void ClassicRules()
{
	int i;
	//犯规 5次判比赛结束 
	if(Classic.Player1Violation == 5)
	{
		Classic.End = 1;
		Classic.Winner = 1; 
	}
	else if(Classic.Player2Violation == 5)
	{
		Classic.End = 1;
		Classic.Winner = 0;
	}
	//进完所有球分高者获胜
	for(i = 1; i <= 9; i ++)
		if(Ball[i].State != INHOLE)
			break;	
	if(i >= 10)
	{
		if(Classic.ScorePlayer1 != Classic.ScorePlayer2)
		{
			Classic.End = 1;
			if(Classic.ScorePlayer1 > Classic.ScorePlayer2)
				Classic.Winner = 0;
			else if(Classic.ScorePlayer2 > Classic.ScorePlayer1)
				Classic.Winner = 1;				
		}
		else
		{
			ResetTwoBalls();
		}
	}
	//犯规或者未进球都需交换方向 
	if(Classic.PlayerScored && (!Violation))
	{
		//printf("AgainTimes: %d\n", Classic.AgainTimes);
	}
	else
	{
		Sides = !Sides;
		UpdtSticks();
		PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_SwitchSide); 
		//播放换边音效 
		Classic.AgainTimes = 0;	
		//printf("AgainTimes: %d\n", Classic.AgainTimes);	
	}
}

void ResetTwoBalls()
{
	if(Classic.Player1Scored != 0 && Classic.Player2Scored != 0)
	{
		Classic.Player1Scored --;
		Classic.Player2Scored --;
		Classic.AgainScore1 += 2;
		Classic.AgainScore2 += 2;
		float Random_X, Random_Y;
		int i;
		i = Classic.Player1LastScored;
		while(1)
		{
			Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
			Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
			if(!Ontable(Random_X, Random_Y))
				continue;
			if(Distance(Random_X, Random_Y, Ball[0].Position_X, Ball[0].Position_Y) < 2 * Ball_R)
				continue;
			else
			{
				Ball[i].Position_X = Random_X;
				Ball[i].Position_Y = Random_Y;
				break;
			}				
		}
		Ball[i].State = ONTABLE;
		Ball[i].IfScored = 0;
		Ball[i].Score = 0;
		WhichHole[i] = -1;
		Draw_R[i] = 16.0;
		InHoleFinished[i] = false;
		
		i = Classic.Player2LastScored;
		while(1)
		{
			Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
			Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
			if(!Ontable(Random_X, Random_Y))
				continue;
			if(Distance(Random_X, Random_Y, Ball[0].Position_X, Ball[0].Position_Y) < 2 * Ball_R)
				continue;
			else
			{
				if(Distance(Random_X, Random_Y, Ball[8].Position_X, Ball[8].Position_Y) < 2 * Ball_R)
					continue;
				else
				{
					Ball[i].Position_X = Random_X;
					Ball[i].Position_Y = Random_Y;
					break;					
				}
			}				
		}
		Ball[i].State = ONTABLE;
		Ball[i].IfScored = 0;
		Ball[i].Score = 0;
		WhichHole[i] = -1;
		Draw_R[i] = 16.0;
		InHoleFinished[i] = false;		
	}
	else if(Classic.Player1Scored != 0 && Classic.Player2Scored == 0)
	{
		Classic.Player1Scored --;
		Classic.AgainScore1 += 2;
		float Random_X, Random_Y;
		int i;
		i = Classic.Player1LastScored;
		while(1)
		{
			Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
			Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
			if(!Ontable(Random_X, Random_Y))
				continue;
			if(Distance(Random_X, Random_Y, Ball[0].Position_X, Ball[0].Position_Y) < 2 * Ball_R)
				continue;
			else
			{
				Ball[i].Position_X = Random_X;
				Ball[i].Position_Y = Random_Y;
				break;
			}				
		}
		Ball[i].State = ONTABLE;
		Ball[i].IfScored = 0;
		Ball[i].Score = 0;
		WhichHole[i] = -1;
		Draw_R[i] = 16.0;
		InHoleFinished[i] = false;
	}
	else if(Classic.Player1Scored == 0 && Classic.Player2Scored != 0)
	{
		Classic.Player2Scored --;
		Classic.AgainScore2 += 2;
		float Random_X, Random_Y;
		int i;
		i = Classic.Player2LastScored;
		while(1)
		{
			Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
			Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
			if(!Ontable(Random_X, Random_Y))
				continue;
			if(Distance(Random_X, Random_Y, Ball[0].Position_X, Ball[0].Position_Y) < 2 * Ball_R)
				continue;
			else
			{
				Ball[i].Position_X = Random_X;
				Ball[i].Position_Y = Random_Y;
				break;
			}				
		}
		Ball[i].State = ONTABLE;
		Ball[i].IfScored = 0;
		Ball[i].Score = 0;
		WhichHole[i] = -1;
		Draw_R[i] = 16.0;
		InHoleFinished[i] = false;
	}
}

void ExerciseBallReset()
{
	int i, j;
	for(i = 1; i < NUM; i ++)
	{
		if(Ball[i].State == INHOLE)
		{
			float Random_X, Random_Y;			
			while(1)
			{
				Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
				Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
				if(!Ontable(Random_X, Random_Y))
					continue;
				for(j = 0; j < NUM; j ++)
				{
					if(i == j)
						continue;
					else
						if(Distance(Random_X, Random_Y, Ball[j].Position_X, Ball[j].Position_Y) < 2 * Ball_R)
							break;
				}
				if(j >= NUM)
				{
					Ball[i].Position_X = Random_X;
					Ball[i].Position_Y = Random_Y;
					Ball[i].State = ONTABLE;
					Ball[i].IfScored = 0;
					Ball[i].Score = 0;
					InHoleFinished[i] = false;
					break;
				}
			}
		}
	}
}

bool Ontable(float X, float Y)
{
	if(((Y + Ball_R <= DownBroad) && (Y - Ball_R >= UpBroad) && (X + Ball_R <= RightBroad) &&(X - Ball_R >= LeftBroad)))
		return true;
	else
		return false;
}

void GetPoolInfo()													//获取当前进球状况（排除白球） 
{
	int i, j;
	int isSolidScored = 0, isStripScored = 0;					//分别记录一回合内是否存在全色球、花色球进球 
	int Temp_BallState[16] = {0};
	Pool.Inhole = 0;
	Pool.SolidInhole = 0;
	Pool.StripInhole = 0;
	Pool.isBlackInhole = false;
	Pool.DoubleScored = false; 
	Pool.Scored = 0;
	//重置记录新进球数组 
	for(i = 0; i < 16; i ++)									
		Pool.NewInhole[i] = 0;
	//复制进球状态数组
	for(i = 1; i < 16; i ++)
		Temp_BallState[i] = Pool.BallState[i];				 
	for(i = 1; i < 16; i ++)
	{
		if(Ball[i].State == INHOLE)
		{
			if(1 <= i && i <= 7)
				Pool.SolidInhole ++;
			else if(9 <= i && i <= 15)
				Pool.StripInhole ++;
			Pool.BallState[i] = 1;								//标记进球 
		}
	}
	for(i = 1; i < 16; i ++)
	{
		if(Pool.BallState[i] == 1 && Temp_BallState[i] == 0)	//判断前后进球状况差异
		{
			Pool.NewInhole[i] = 1; 
			if(1 <= i && i <= 7)
				isSolidScored ++;
			else if(i == 8)
				Pool.isBlackInhole = true;
			else if(9 <= i && i <= 15)
				isStripScored ++;
			if(Pool.Scored == 0)								//如果是第一例进球 
				Pool.Scored = i;
		} 
	}
	//判断初次进球是否存在双花色进球 
	if(isSolidScored && isStripScored)
		Pool.DoubleScored = true;
		
	//计算全场的进球数和场上球数 
	Pool.Inhole = Pool.SolidInhole + Pool.StripInhole; 
	Pool.Ontable = 15 - Pool.Inhole;
	//获取两方是否进完其他所有球 
	if(Pool.SolidInhole >= 7)
		Pool.SideClear[Pool.SideSolid] = true;
	else
		Pool.SideClear[Pool.SideSolid] = false;
	if(Pool.StripInhole >= 7)
		Pool.SideClear[Pool.SideStrip] = true;
	else
		Pool.SideClear[Pool.SideStrip] = false;
	//判断母球是否进洞
	if(Ball[0].State == INHOLE)
		Pool.isCueInhole = true;
	else
		Pool.isCueInhole = false; 
} 

void GetTargetBall()
{
	int i;
	if(Pool.TargetBall == 0)
		for(i = 1; i < 16; i ++)
			if(Ball[i].Velocity_X != 0 || Ball[i].Velocity_Y != 0)
			{
				Pool.TargetBall = i;
				return;	
			}	
} 

bool isTargetValid()
{
	if(Pool.isSideConfirmed == 0)
		return true;
	else
	{
		if(1 <= Pool.TargetBall && Pool.TargetBall <= 7)
		{	
			if(Sides == Pool.SideSolid)
				return true;
			else
				return false;
		}
		else if(9 <= Pool.TargetBall && Pool.TargetBall <= 15)
		{
			if(Sides == Pool.SideStrip)
				return true;
			else
				return false;
		}		
	}
}

bool isScoreValid()					//判断进球是否有效，有效条件：进球中必须包含己方球，否则无效 
{
	int i;
	if(Pool.SideSolid == Sides)			//如果当前击球方是全色球 
	{
		for(i = 1; i <= 7; i ++)
		{
			if(Pool.NewInhole[i] != 0)
				break;
		}
		if(i > 7)
			return false;
		else
			return true;
	}
	else if(Pool.SideStrip == Sides)
	{
		for(i = 9; i <= 15; i ++)
		{
			if(Pool.NewInhole[i] != 0)
				break;
		}
		if(i > 15)
			return false;
		else
			return true;
	}
} 

void PoolRules()
{
	if(Pool.isSuccessfullyBegin == 0 && Pool.isSideConfirmed == 0)	//开球 
	{
		GetPoolInfo();
		//规则一：开球必须有四次与球台边沿的碰撞或者一次进球，否则开球失败
		if(Pool.Reflect <= 4 && Pool.Scored == 0)
		{
			int NowSide = Sides;
			GameMode = GM_POOL;
			InitParameter();
			Sides = !NowSide;
			UpdtSticks();
			PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_SwitchSide); 
			SetBalls();
			Pool.isSuccessfullyBegin = 0;
			return;			
		}
		if(Pool.Reflect >= 4 || Pool.Scored)
		{				 
			if(Pool.isCueInhole|| Ball[0].State == INHOLE || Pool.isBlackInhole)	//如果开球黑八进洞、母球进洞 
			{ 
				int NowSide = Sides;
				GameMode = GM_POOL;
				InitParameter();
				Sides = !NowSide;
				UpdtSticks();
				PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_SwitchSide); 
				SetBalls();
				Pool.isSuccessfullyBegin = 0;
				return;
			} 
			if(Pool.Scored)												//如果得分
			{
				if(!Pool.DoubleScored)									//如果只击落了一种花色 
				{
					if(1 <= Pool.Scored && Pool.Scored <= 7)			//如果第一颗球是全色球
					{
						Pool.SideSolid = Sides;
						Pool.SideStrip = !Sides;
						Pool.isSideConfirmed = 1;
						Pool.isSuccessfullyBegin = 1;
					}
					else if(9 <= Pool.Scored && Pool.Scored <= 15)		//如果第一颗球是花色球
					{
						Pool.SideStrip = Sides;
						Pool.SideSolid = !Sides;
						Pool.isSideConfirmed = 1;
						Pool.isSuccessfullyBegin = 1;
					}
				}
				else													//如果击落了两种花色 
				{
					Pool.isSideConfirmed = 0;
					Pool.isSuccessfullyBegin = 1;
					ShowHintMsge("击落双种花色：暂时无法确定花色"); 
				}	
			}
			else														//如果没有得分只是发生了四次以上的碰撞 
			{
				Sides = !Sides;
				UpdtSticks();
				//播放换边音效 
				PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_SwitchSide); 
				Pool.isSideConfirmed = 0; 
				Pool.isSuccessfullyBegin = 1;
				Pool.DoubleScored = false;
			}
			if(Pool.isSideConfirmed)
				SetInholeBallsAfterConfirmed();								//摆放暂时未摆放的进球 
		} 
		else
		{
			Sides = !Sides;
			UpdtSticks();
			//播放换边音效 
			PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_SwitchSide); 
			Pool.isSideConfirmed = 0; 
			Pool.isSuccessfullyBegin = 1;		
		}
	}
	else if(Pool.isSuccessfullyBegin && !Pool.isSideConfirmed)
	{
		GetPoolInfo();
		if(Pool.Scored)
		{
			if(!Pool.DoubleScored)									//如果只击落了一种花色 
			{
				if(1 <= Pool.Scored && Pool.Scored <= 7)			//如果第一颗球是全色球
				{
					Pool.SideSolid = Sides;
					Pool.SideStrip = !Sides;
					Pool.isSideConfirmed = 1;
				}
				else if(9 <= Pool.Scored && Pool.Scored <= 15)		//如果第一颗球是花色球
				{
					Pool.SideStrip = Sides;
					Pool.SideSolid = !Sides;
					Pool.isSideConfirmed = 1;
				}
			}
			else													//如果击落了两种花色 
			{
				Pool.isSideConfirmed = 0;
				ShowHintMsge("击落双种花色：暂时无法确定花色"); 
			}
			if(Pool.isSideConfirmed)
				SetInholeBallsAfterConfirmed();								//摆放暂时未摆放的进球	
			if(Pool.isBlackInhole)
			{
				Pool.End = 1;
				Pool.Winner = !Sides;
			}
			if(Pool.isCueInhole)
			{
				Violation = 1; 
			}			
		}
		else
		{
			Violation = 1; 
		}
	} 
	else if(Pool.isSuccessfullyBegin && Pool.isSideConfirmed)			//开球成功 
	{
		GetPoolInfo();													//获取当前全局状态 
		//优先判定黑八 
		if(Pool.isBlackInhole && !Pool.SideClear[Sides] && Pool.End == 0)	//规则三：开球后，如果该方其他球未全进黑八进洞直接判负 
		{
			Pool.Winner = !Sides;
			Pool.End = 1;													//如果未清台的情况下进黑八直接判负 
		}
		else if(Pool.isBlackInhole && Pool.SideClear[Sides] && Pool.End == 0)
		{
			if(Pool.isCueInhole)
			{
				Pool.Winner = !Sides;
				Pool.End = 1;
			}
			else
			{
				Pool.Winner = Sides;
				Pool.End = 1;
				if(Pool.SideSolid == Sides)
					Pool.SolidInhole ++;
				else
					Pool.StripInhole ++;
			}
		}
		//规则四：犯规：目标球错误、进球不包含己方球、母球进洞、一次击球内即未进球也无与球台的碰撞 
		if(!isTargetValid()) 				Violation = 1;		
		if(Pool.isCueInhole) 				Violation = 1;		
		if(!isScoreValid())	 				Violation = 1;
		if(!Pool.Scored && !Pool.Reflect) 	Violation = 1;
		else
			return; 
	}
} 

void SetInholeBallsAfterConfirmed()							//专门用来处理还未确定花色前的进球 
{
	int i;
	int Solid = 0;
	int Strip = 0;
	//先摆放第一个进球 
	if(1 <= Pool.Scored && Pool.Scored <= 7)
	{
		if(Pool.SideSolid == 0)
			Ball[Pool.Scored].Position_X = 33;
		else
			Ball[Pool.Scored].Position_X = 1247;
		Ball[Pool.Scored].Position_Y = Shelf_Y[Solid ++];	
	}
	else if(9 <= Pool.Scored && Pool.Scored <= 15)
	{
		if(Pool.SideStrip == 0)
			Ball[Pool.Scored].Position_X = 33;
		else
			Ball[Pool.Scored].Position_X = 1247;
		Ball[Pool.Scored].Position_Y = Shelf_Y[Strip ++];			
	}
	for(i = 1; i < 16; i ++)
	{
		if(Ball[i].State == ONTABLE || i == Pool.Scored)
			continue;
		else
		{
			if(1 <= i && i <= 7)
			{
				if(Pool.SideSolid == 0)
					Ball[i].Position_X = 33;
				else
					Ball[i].Position_X = 1247;
				Ball[i].Position_Y = Shelf_Y[Solid ++];	
			}
			else if(9 <= i && i <= 15)
			{
				if(Pool.SideStrip == 0)
					Ball[i].Position_X = 33;
				else
					Ball[i].Position_X = 1247;
				Ball[i].Position_Y = Shelf_Y[Strip ++];								
			}			
		}
	} 
	Pool.SolidSet = Solid;
	Pool.StripSet = Strip;
	Pool.isSet = 1; 
}

//int HitPosition_X = 639;
//int HitPosition_Y = 665 + TITLEHEIGHT;
extern float const DirectionBall_X;
extern float const DirectionBall_Y;
extern float const DirectionBall_R;
//extern VECTOR Direction;

void GetSwirl(int HitPosition_X, int HitPosition_Y)
{
	Rotation.Reflects = 0;
	Rotation.LeftRotation = 0;
	Rotation.RightRotation = 0;
	Rotation.FrontRotation = 0;
	Rotation.BackRotation = 0;
	Rotation.RotationRatio = 0;			//左右塞比例系数 
	Rotation.MoveRatio = 0;				//高低杆比例系数 
	if(HitPosition_X - DirectionBall_X < 0)				//左塞 
	{
		Rotation.SwirlTheta = - SwirlTheta_Max * pow((DirectionBall_X - HitPosition_X) / DirectionBall_R, 3 / 2);
		Rotation.RotationRatio = - (HitPosition_X - DirectionBall_X) / DirectionBall_R;
		Rotation.LeftRotation = 1;
	}
	else if(HitPosition_X - DirectionBall_X > 0)		//右塞 
	{
		Rotation.SwirlTheta = SwirlTheta_Max * pow((HitPosition_X - DirectionBall_X) / DirectionBall_R, 3 / 2);
		Rotation.RotationRatio = (HitPosition_X - DirectionBall_X) / DirectionBall_R;
		Rotation.RightRotation = 1;
	}
	else												//不加塞 
		Rotation.SwirlTheta = 0;
	if(HitPosition_Y - DirectionBall_Y < 0)				//高杆
	{
		Rotation.Acceleration = Rotation.AccelerationMax * pow((DirectionBall_Y - HitPosition_Y) / DirectionBall_R, 3 / 2);	
		Rotation.MoveRatio = - (HitPosition_Y - DirectionBall_Y) / DirectionBall_R;
		Rotation.FrontRotation = 1;
	}
	else if(HitPosition_Y - DirectionBall_Y > 0)		//低杆 
	{
		Rotation.Acceleration = - Rotation.AccelerationMax * pow((HitPosition_Y - DirectionBall_Y) / DirectionBall_R, 3 / 2);
		Rotation.MoveRatio = (HitPosition_Y - DirectionBall_Y) / DirectionBall_R;
		Rotation.BackRotation = 1;
	}
	else												//击中心点 
		Rotation.Acceleration = 0;
	Rotation.SwirlTheta *= pow((CueTheta / (pi / 2)), 3 / 2);	
}

void Swirl()
{
	if(Rotation.Acceleration == 0 && Rotation.SwirlTheta == 0)
		return;
	MATRIX Rot_Matrix = {{0, 0}, {0, 0}};
	if(Rotation.Reflects != 0)
		Rotation.SwirlTheta = 0;
	if(fabs(Rotation.SwirlTheta) <= pi / 1024)
		Rotation.SwirlTheta = 0;
	Rotation.SwirlTheta *= 0.95;
	VECTOR Velocity = {Ball[0].Velocity_X, Ball[0].Velocity_Y};
	if(Rotation.SwirlTheta != 0)
	{
		RotationMatrix(Rot_Matrix, Rotation.SwirlTheta);
		MatTimesVec(Rot_Matrix, Velocity);		
	}
	VECTOR DirectionVector = {Velocity[0], Velocity[1]};
	UnitVec(DirectionVector);
	Velocity[0] += DirectionVec[0] * Rotation.Acceleration;
	Velocity[1] += DirectionVec[1] * Rotation.Acceleration;
	Rotation.Acceleration *= 0.6;
	if(fabs(Rotation.Acceleration) <= 0.1)
		Rotation.Acceleration = 0;
	Ball[0].Velocity_X = Velocity[0];
	Ball[0].Velocity_Y = Velocity[1];
	Rotation.Reflects = 0;
}


//0 Cue Ball, 1 ~ 6 Colour, 7 ~ 21 Object Balls
/*
	0: White
	1: Yellow
	2: Green
	3: Brown
	4: Blue
	5: Pink
	6: Black
	7 ~ 21: Red
*/
int SnookerScore[22] = {0, 2, 3, 4, 5, 6, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
void RedClearJudge()
{
	int i;
	for(i = 7; i < 22; i ++)
	if(Ball[i].State == ONTABLE)
		break; 
	if(i >= 22)
	{
		Snooker.isRedClear = 1;	
		if(Snooker.NowTime == 0)
		{
			Snooker.Skip = 1;
			Snooker.NowTime = 1;
			int tempSum = 0;
			for(i = 1; i < 22; i ++)
			{
				if(Snooker.NewInhole[i])
					tempSum += SnookerScore[i];
			}
			if(!Sides)
				Snooker.Player1Score += tempSum;
			else
				Snooker.Player2Score += tempSum;
			for(i = 7; i < 22; i ++)
			{
				Ball[i].Position_X = - 4 * Ball_R;
				Ball[i].Position_Y = - 2 * Ball_R * i; 
			}
			Snooker.Player1Scored = 0;
			Snooker.Player2Scored = 0;
		}

		Snooker.isRedTurn = 0;	
	}
	else 
		Snooker.isRedClear = 0;
}

float Reset_X[7] = {0, 331, 331, 331, WIDTH / 2, WIDTH / 9 * 6 - 4 * Ball_R, WIDTH / 6 * 5};
float Reset_Y[7] = {0, 315 + TITLEHEIGHT, 226 + TITLEHEIGHT, 406 + TITLEHEIGHT, 315 + TITLEHEIGHT, 315 + TITLEHEIGHT, 315 + TITLEHEIGHT};

void ResetBlackBall()
{
	if(Ball[6].Position_X == 33)
	{
		Snooker.Player1Scored --;
	}
	else if(Ball[6].Position_X == 1247)
	{
		Snooker.Player2Scored --;
	}	
	float Random_X, Random_Y;
	Random_X = Reset_X[6];
	Random_Y = Reset_Y[6];
	if(Distance(Random_X, Random_Y, Ball[0].Position_X, Ball[0].Position_Y) < 2 * Ball_R)
	{
		while(1)
		{
			Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
			Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
			if(!Ontable(Random_X, Random_Y))
				continue;
			if(Distance(Random_X, Random_Y, Ball[0].Position_X, Ball[0].Position_Y) < 2 * Ball_R)
				continue;
			else
			{
				Ball[6].Position_X = Random_X;
				Ball[6].Position_Y = Random_Y;
				break;
			}
		}
		Ball[6].State = ONTABLE;
		Ball[6].IfScored = 0;
		Ball[6].Score = 0;
		WhichHole[6] = -1;
		Draw_R[6] = 16.0;
		InHoleFinished[6] = false;		
	}
	else
	{
		Ball[6].Position_X = Random_X;
		Ball[6].Position_Y = Random_Y;
		Ball[6].State = ONTABLE;
		Ball[6].IfScored = 0;
		Ball[6].Score = 0;
		WhichHole[6] = -1;
		Draw_R[6] = 16.0;
		InHoleFinished[6] = false;		
	}
} 

void ResetBalls()
{
	if(!Snooker.isRedClear)
	{
		int i, j;
		float Random_X, Random_Y;
		for(i = 1; i < 7; i ++)
		{
			if(Ball[i].State == INHOLE)
			{
				Random_X = Reset_X[i];
				Random_Y = Reset_Y[i];
				for(j = 0; j < 22; j ++)
				{
					if(i == j)
						continue;
					if(Distance(Random_X, Random_Y, Ball[j].Position_X, Ball[j].Position_Y) <= 2 * Ball_R)
						break;				
				}
				if(j < 22)
				{		
					while(1)
					{
						Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
						Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
						if(!Ontable(Random_X, Random_Y))
							continue;
						for(j = 0; j < 22; j ++)
						{
							if(i == j)
								continue;
							if(Distance(Random_X, Random_Y, Ball[j].Position_X, Ball[j].Position_Y) < 2 * Ball_R)
								break;					
						}
						if(j >= 22)
						{
							Ball[i].Position_X = Random_X;
							Ball[i].Position_Y = Random_Y;
							break;
						}
					}					
				}
				else
				{
					Ball[i].Position_X = Random_X;
					Ball[i].Position_Y = Random_Y;
				}
				Ball[i].State = ONTABLE;
				Ball[i].IfScored = 0;
				Ball[i].Score = 0;
				WhichHole[i] = -1;
				Draw_R[i] = 16.0;
				InHoleFinished[i] = false;			
			}
		}
	}
	else if(Snooker.NowTime != Snooker.Score)
	{
		int i, j;
		float Random_X, Random_Y;
		for(i = Snooker.NowTime; i < 7; i ++)
		{
			if(Ball[i].State == INHOLE)
			{
				Random_X = Reset_X[i];
				Random_Y = Reset_Y[i];
				for(j = 0; j < 22; j ++)
				{
					if(i == j)
						continue;
					if(Distance(Random_X, Random_Y, Ball[j].Position_X, Ball[j].Position_Y) <= 2 * Ball_R)
						break;					
				}
				if(j < 22)
				{		
					while(1)
					{
						Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
						Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
						if(!Ontable(Random_X, Random_Y))
							continue;
						for(j = 1; j < 22; j ++)
						{
							if(i == j)
								continue;
							if(Distance(Random_X, Random_Y, Ball[j].Position_X, Ball[j].Position_Y) < 2 * Ball_R)
								break;							
						}
						if(j >= 22)
						{
							Ball[i].Position_X = Random_X;
							Ball[i].Position_Y = Random_Y;
							break;
						}
					}					
				}
				else
				{
					Ball[i].Position_X = Random_X;
					Ball[i].Position_Y = Random_Y;
				}
				Ball[i].State = ONTABLE;
				Ball[i].IfScored = 0;
				Ball[i].Score = 0;
				WhichHole[i] = -1;
				Draw_R[i] = 16.0;
				InHoleFinished[i] = false;		
			}
		}
		int SnookerPlayerScored1 = 0;
		int SnookerPlayerScored2 = 0;
		for(i = 1; i < 7; i ++)
		{
			if(Ball[i].Position_X == 33)
			{
				SnookerPlayerScored1 ++;	
			}
			else if(Ball[i].Position_X == 1247)
			{
				SnookerPlayerScored2 ++;
			}
		}	
		Snooker.Player1Scored = SnookerPlayerScored1;
		Snooker.Player2Scored = SnookerPlayerScored2;
		printf("Snooker.Player1Scored: %d\n", Snooker.Player1Scored);
		printf("Snooker.Player2Scored: %d\n", Snooker.Player2Scored);	
	}
	else if(Snooker.NowTime == Snooker.Score)
	{
		int i, j;
		float Random_X, Random_Y;
		for(i = Snooker.NowTime; i < 7; i ++)
		{
			if(Ball[i].State == INHOLE)
			{
				Random_X = Reset_X[i];
				Random_Y = Reset_Y[i];
				for(j = 0; j < 22; j ++)
				{
					if(i == j)
						continue;
					if(Distance(Random_X, Random_Y, Ball[j].Position_X, Ball[j].Position_Y) <= 2 * Ball_R)
						break;					
				}
				if(j < 22)
				{		
					while(1)
					{
						Random_X = LeftBroad + Ball_R + rand() % (RightBroad - LeftBroad - 2 * Ball_R);
						Random_Y = UpBroad + Ball_R + rand() % (DownBroad - UpBroad - 2 * Ball_R);
						if(!Ontable(Random_X, Random_Y))
							continue;
						for(j = 1; j < 22; j ++)
						{
							if(i == j)
								continue;
							if(Distance(Random_X, Random_Y, Ball[j].Position_X, Ball[j].Position_Y) < 2 * Ball_R)
								break;							
						}
						if(j >= 22)
						{
							Ball[i].Position_X = Random_X;
							Ball[i].Position_Y = Random_Y;
							break;
						}
					}					
				}
				else
				{
					Ball[i].Position_X = Random_X;
					Ball[i].Position_Y = Random_Y;
				}
				Ball[i].State = ONTABLE;
				Ball[i].IfScored = 0;
				Ball[i].Score = 0;
				WhichHole[i] = -1;
				Draw_R[i] = 16.0;
				InHoleFinished[i] = false;			
				printf("There2 Snooker.NowTime: %d\n", Snooker.NowTime);
				if(!Sides)
				{
					printf("Snooker.Player1Scored: %d\n", Snooker.Player1Scored);
					Snooker.Player1Scored -= 1;
				}
				else
				{
					printf("Snooker.Player2Scored: %d\n", Snooker.Player2Scored);
					Snooker.Player2Scored -= 1;
				}
			}
		}			
	}
}

void GameJudger()
{
	GetSnookerInfo();	
	int TargetLegal = isTargetLegal();
	int ScoreLegal = isScoreLegal();
	if(Snooker.Skip == 1)
		Snooker.Skip = 0;
	if(ScoreLegal != 1 || !TargetLegal || Snooker.isCueInhole)
	{
		if(ScoreLegal == 0);
			//printf("Score Illegal!\n");
		if(ScoreLegal == -1);
			//printf("Not Scored!\n");
		if(!TargetLegal);
			//printf("Target Illegal!\n");
		if(Snooker.isCueInhole);
			//printf("Cue Ball Inhole!\n");
		int Punishment = 0;
		if(ScoreLegal == 0 || Snooker.isCueInhole)
		{
			if(Snooker.TargetBall == 1)
				Punishment = 4;
			else if(Snooker.TargetBall == 2)
				Punishment = 4;
			else if(Snooker.TargetBall == 3)
				Punishment = 4;
			else if(Snooker.TargetBall == 4)
				Punishment = 5;
			else if(Snooker.TargetBall == 5)
				Punishment = 6;
			else if(Snooker.TargetBall == 6)
				Punishment = 7;
			else if(Snooker.TargetBall >= 7)
				Punishment = 1;
			if(Punishment < 4)
				Punishment = 4;
			if(!Sides) 
				Snooker.Player2Score += Punishment;
			else 
				Snooker.Player1Score += Punishment;			
		}
		Sides = !Sides;
		UpdtSticks();
		//播放换边音效 
		PostThreadMessage(MenuMusicID, 0xFFFF, TM_MUSIC, (LPARAM)MU_SwitchSide); 
		Violation = 1;
		if(!Snooker.isRedClear)
			Snooker.isRedTurn = 1;
		else
			Snooker.isRedTurn = 0;
	}
	else
	{
		int NowScore = 0, i;
		for(i = 1; i < 22; i ++)
		{
			if(!Snooker.isRedClear)
			{
				switch(i)
				{
					case 0: NowScore += 0; break;
					case 1: if(Snooker.NewInhole[i] == 1) NowScore += 2; break;
					case 2: if(Snooker.NewInhole[i] == 1) NowScore += 3; break;
					case 3: if(Snooker.NewInhole[i] == 1) NowScore += 4; break;
					case 4: if(Snooker.NewInhole[i] == 1) NowScore += 5; break;
					case 5: if(Snooker.NewInhole[i] == 1) NowScore += 6; break;
					case 6: if(Snooker.NewInhole[i] == 1) NowScore += 7; break;
					default: if(Snooker.NewInhole[i] == 1) NowScore += 1; break;
				}	
			}
		}
		if(!Sides)
			Snooker.Player1Score += NowScore;
		else
			Snooker.Player2Score += NowScore;
		if(!Snooker.isRedClear)
			Snooker.isRedTurn = !Snooker.isRedTurn;
	}
}

void GetSnookerInfo()
{
	int i, j;
	Snooker.isRedScored = 0;
	Snooker.isColourScored = 0;
	Snooker.isRedClear = 0;
	int Temp_BallState[22] = {0};
	for(i = 0; i < 22; i ++)								
		Snooker.NewInhole[i] = 0;
	for(i = 1; i < 22; i ++)
	{
		if(!Snooker.isRedClear) 
			if(i >= 1 && i <= 6)
				Snooker.BallState[i] = 0;
			else;
		else; 
		Temp_BallState[i] = Snooker.BallState[i];		
	}
	for(i = 1; i < 22; i ++)
		if(Ball[i].State == INHOLE)
			Snooker.BallState[i] = 1;
			
	for(i = 1; i < 22; i ++)
	{
		if(Snooker.BallState[i] == 1 && Temp_BallState[i] == 0)
		{
			Snooker.NewInhole[i] = 1; 
			if(i >= 7)
				Snooker.isRedScored = 1;
			else if(i <= 6 && i >= 1)
				Snooker.isColourScored = 1;
			else
				Snooker.isCueInhole = 1;
		}
	}
	if(Ball[0].State == INHOLE)	
		Snooker.isCueInhole = 1;
	else
		Snooker.isCueInhole = 0;
	RedClearJudge();
}

int isScoreLegal()				//未进球: -1, 进错球: 0, 进球: 1 
{
	if(Snooker.Skip == 1)
		return 1;
	int i;
	for(i = 1; i < 22; i ++)
		if(Snooker.NewInhole[i] == 1)
			break;
	if(i >= 22)
		return -1; 
	if(Snooker.isRedTurn && Snooker.isRedScored)
		return 1;
	else if(Snooker.isRedTurn && !Snooker.isRedScored)
		return 0;
	else if(!Snooker.isRedTurn && !Snooker.isRedClear && !Snooker.isColourScored)
		return 0;
	else if(!Snooker.isRedTurn && !Snooker.isRedClear && Snooker.isColourScored)
		return 1;
	else if(Snooker.isRedClear && Snooker.Score != Snooker.NowTime && Snooker.Score != 0)
	{
		Snooker.WrongGoal = 1;
		return 0;
	}
	else if(Snooker.isRedClear && Snooker.Score == Snooker.NowTime)
	{
		int count = 0;
		for(i = Snooker.NowTime; i < 7; i ++)
		{
			if(Ball[i].State == INHOLE)
				count ++;
			else
				break;
		}
		printf("Count = %d\n", count);
		if(!Sides)
		{
			for(i = Snooker.NowTime; i < Snooker.NowTime + count; i ++)
			{
				if(Snooker.isCueInhole || Snooker.NowTime != Snooker.TargetBall);
				else
				{
					Snooker.Player1Score += i + 1;	
				}
			}
		}	
		else
		{
			for(i = Snooker.NowTime; i < Snooker.NowTime + count; i ++)
			{
				if(Snooker.isCueInhole || Snooker.NowTime != Snooker.TargetBall);
				else
				{
					Snooker.Player2Score += i + 1;	
				}
			}
		}
		Snooker.NowTime += count;
				
		return 1;		
	}
	else if(Snooker.isRedClear && Snooker.Score == 0)
		return -1;
}

bool isTargetLegal()
{
	if(Snooker.Skip == 1)
		return true;
	if(!Snooker.isRedClear)
	{
		if(Snooker.isRedTurn && Snooker.TargetBall >= 7)
			return true;
		else if(Snooker.isRedTurn && Snooker.TargetBall < 7)
			return false;
		else if(!Snooker.isRedTurn && Snooker.TargetBall >= 7)
			return false;
		else if(!Snooker.isRedTurn && Snooker.TargetBall < 7 && Snooker.TargetBall != 0)
			return true;
		else 
		return false;		
	}
	else
	{
		if(Snooker.TargetBall != Snooker.NowTime)
			return false;
		else
			return true;
	}
}
