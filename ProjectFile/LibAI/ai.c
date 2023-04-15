#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <pictures.h> 
#include <sets.h>
#include <physics.h>
#include <vector.h>

#include <math.h>

#define INF_DISTANCE 998244353.01					//无~限~远！

//调试模式是否开启 
//#define DEBUG_MODE

extern float MaxVelocity;
extern int NUM;
extern float Hole_X[6];
extern float Hole_Y[6];
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
} Ball[16];

static int AI_Inited = 0;							//AI初始化状态记录 

//通过三角形三点坐标，计算三角形的面积。
inline float TriangleArea(float x1, float y1, float x2, float y2, float x3, float y3) {
	return(0.50*(x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2));
}

//判断第三球是否阻挡了两点之间的路径，返回1则不阻挡，返回0则阻挡。
inline int isBlocked(float x1, float y1, float x2, float y2, float x, float y) {
	return((TriangleArea(x1,y1,x2,y2,x,y) / Distance(x1,y1,x2,y2) > 0.50 * Ball_R) ? 1 : 0);
}

extern float Power;
extern int CurrentPosition_X;
extern int CurrentPosition_Y;

//击球！
inline void hitBall(int ball_id, int hole_id) 
{
	//如果调用的球已经进洞了，那怎么也得随机抓一个打一下吧 
	if(Ball[ball_id].State == INHOLE) 
	{
		Ball[0].Velocity_X = rand() % 5 - 2.50;
		Ball[0].Velocity_Y = rand() % 5 - 2.50;
		CurrentPosition_X = Ball[0].Position_X + 100 * Ball[0].Velocity_X / (sqrt(Ball[0].Velocity_X*Ball[0].Velocity_X + Ball[0].Velocity_Y*Ball[0].Velocity_Y));
		CurrentPosition_Y = Ball[0].Position_Y + 100 * Ball[0].Velocity_Y / (sqrt(Ball[0].Velocity_X*Ball[0].Velocity_X + Ball[0].Velocity_Y*Ball[0].Velocity_Y));
		return;
	}
	//向量：从母球到目标球 
	VECTOR Direction = {Ball[ball_id].Position_X - Ball[0].Position_X, Ball[ball_id].Position_Y - Ball[0].Position_Y};
	//向量：从目标球到目标洞 
	VECTOR Shift = {Hole_X[hole_id] - Ball[ball_id].Position_X, Hole_Y[hole_id] - Ball[ball_id].Position_Y};
	//力度因子
	float PowerFactor = Mod(Direction) + Mod(Shift); 
	Power = PowerFactor / 10.0 > 100.0 ? 100.0 : PowerFactor / 10.0;
	//计算偏移量，让球能被击往目标的方向 
	UnitVec(Shift);
	Direction[0] += (-2.0) * Ball_R * Shift[0];
	Direction[1] += (-2.0) * Ball_R * Shift[1];
	
	UnitVec(Direction);
	Ball[0].Velocity_X = 0.040 * PowerFactor * Direction[0];
	Ball[0].Velocity_Y = 0.040 * PowerFactor * Direction[1];
	CurrentPosition_X = Ball[ball_id].Position_X - 2*Ball_R*Direction[0]/sqrt(Direction[0]*Direction[0] + Direction[1]*Direction[1]);
	CurrentPosition_Y = Ball[ball_id].Position_Y - 2*Ball_R*Direction[1]/sqrt(Direction[0]*Direction[0] + Direction[1]*Direction[1]);
} 

#ifdef DEBUG_MODE
//hitBall test function
void TesthitBall() 
{
	static int i = 1;
	static int j;
	if(j>5) {
		i++;
		j=0;
	}
	if(i>=NUM-1) return;
	printf("Test: Hit ball %d to hole %d\n",i,j);
	hitBall(i,j);
	j++;
}
#endif

//判断是否有球阻挡了指定id的球到母球之间的路径，返回1则不阻挡，返回0则阻挡。
//注意：这个函数不会考虑母球（这不废话啊喂） 
int isBallBlocking(int ball_id) 
{
	//球的id取值范围为1~(NUM-1)，超出则返回-1
	if(ball_id<1||ball_id>NUM-1) return(-1);
	
	//以下是核心判断代码 
	int i;
	int ret = 1;
	for(i=1;i<NUM;i++) {
		if(i!=ball_id) {
			//把每个球都抓来判断一次（当然不能自己跟自己判断啊） 
			ret = ret & isBlocked ( Ball[ball_id].Position_X, Ball[ball_id].Position_Y	, 	//ball_id的坐标 
									Ball[0].Position_X, Ball[0].Position_Y				,   //母球的坐标 
									Ball[i].Position_X, Ball[i].Position_Y				);  //第i号球的坐标 
		}
	}
	return(ret);
}

//判断是否有球阻挡了指定id的球到指定洞之间的路径，返回1则不阻挡，返回0则阻挡。
//注意：这个函数不会考虑母球
int isPathBlocking(int ball_id, int hole_id) {
	
	//球的id取值范围为1~(NUM-1)，超出则返回-1
	if(ball_id<1||ball_id>NUM-1) return(-1);
	//洞的id取值范围为1~6，超出则返回-1
	if(hole_id<1||hole_id>6) return(-1);
	
	//以下是核心判断代码 
	int i;
	int ret = 1;
	for(i=1;i<NUM;i++) {
		if(i!=ball_id) {
			//把每个球都抓来判断一次（当然不能自己跟自己判断啊） 
			ret = ret & isBlocked ( Ball[ball_id].Position_X, Ball[ball_id].Position_Y	, 	//ball_id的坐标 
									Hole_X[hole_id], Hole_Y[hole_id]					,   //hole_id的坐标 
									Ball[i].Position_X, Ball[i].Position_Y				);  //第i号球的坐标 
		}
	}
	return(ret);
}

//判断指定洞是否在指定球的"可直接击入区域"内，返回1则在，返回0则不在。
//"可直接击入区域"是指母球击中指定球后，指定球不经过反弹也能达到的区域。
//判定原理：余弦定理导出不等式 
int isArrivable(int ball_id, int hole_id) {
	//因为觉得代码写很长太难看了，于是试图预定义变量让后面的判断简洁一些 
	double 	d1 = Distance(Ball[0].Position_X, Ball[0].Position_Y, Ball[ball_id].Position_X, Ball[ball_id].Position_Y),
			d2 = Distance(Ball[ball_id].Position_X, Ball[ball_id].Position_Y, Hole_X[hole_id], Hole_Y[hole_id]),
			d3 = Distance(Ball[0].Position_X, Ball[0].Position_Y, Hole_X[hole_id], Hole_Y[hole_id]);
	//求平方~ 
	d1*=d1, d2*=d2, d3*=d3;
	return((d1+d2<d3)?1:0);
}

//向一个包含6个整数的数组写入数据，其中的整数都是洞的ID，代表球ball_id到洞的距离由近到远排序。
void sortHoles(int ball_id, int* ArrayPtr) {
	
	//如果输入的数组并没有这么多元素，那我直接不干辣！
	ArrayPtr[5] = 0;
	int i,j,max_idx;
	double max_dis;
	//读距离数据 
	double DistanceArray[6];
	for(i=0;i<6;i++) DistanceArray[i] = Distance(Ball[ball_id].Position_X, Ball[ball_id].Position_Y, Hole_X[i], Hole_Y[i]);
	//反正数字挺少的，懒得定义新的structure写快排了.jpg
	
	//一层循环：寻找距离第i+1近的洞
	for(i=0;i<6;i++) {
		max_idx = -1, max_dis = INF_DISTANCE;
		//二层循环：标记距离最近的洞
		for(j=0;j<6;j++) {
			if(DistanceArray[j] < max_dis) {
				max_idx = j, max_dis = DistanceArray[j];
			}
		}
		
		//将找到的球id写入数组，并将距离设为无限远以免重复写入
		if(max_idx!=-1) {
			ArrayPtr[i] = max_idx;
			DistanceArray[max_idx] = INF_DISTANCE;
		}
	}
	
	#ifdef DEBUG_MODE
	printf("sortHoles return:[");
	for(i=0;i<6;i++) {
		printf("%d", ArrayPtr[i]);
		if(i<5) printf(" ");
	}
	printf("]\n");
	#endif
}

//向一个包含NUM-1个整数的数组写入数据，其中的整数都是球的ID，代表球距离母球由近到远排序。 
void sortByDistance(int* ArrayPtr) 
{
	//如果输入的数组并没有这么多元素，那我直接不干辣！
	ArrayPtr[NUM-2] = 0;
	 
	int i,j,max_idx;
	double max_dis;
	//读距离数据
	double DistanceArray[NUM];
	DistanceArray[0] = 0;
	for(i = 1;i<NUM;i++) DistanceArray[i] = Distance( Ball[0].Position_X, Ball[0].Position_Y  , 
													Ball[i].Position_X, Ball[i].Position_Y );
	//反正数字挺少的，懒得定义新的structure写快排了.jpg（你还真是个复读机啊。。。） 
	
	//一层循环：寻找距离第i+1近的球 
	for(i = 0;i<NUM-1;i++) {
		max_idx = -1, max_dis = INF_DISTANCE;
		//二层循环：标记此时距离最近的球
		for(j=1;j<NUM;j++) {
			if(DistanceArray[j] < max_dis) {
				max_idx = j, max_dis = DistanceArray[j];
			}
		}
		//将找到的球id写入数组，并将距离设为无限远以免重复写入 
		if(max_idx!=-1) {
			ArrayPtr[i] = max_idx;
			DistanceArray[max_idx] = INF_DISTANCE;
		}
	}
	
	#ifdef DEBUG_MODE
	printf("sortByDistance return:[");
	for(i=0;i<NUM-1;i++) {
		printf("%d", ArrayPtr[i]);
		if(i<NUM-2) printf(" ");
	}
	printf("]\n");
	#endif
}

/*
	写了这么多乱七八糟的函数，终于要进入正题了
*/

int AI_Init() {
	srand(time(NULL));
	AI_Inited = 1;
	return(0);		//简单粗暴
}

int AI_Clear() {
	if(AI_Inited) {
		AI_Inited = 0;
		return(0);
	} else {
		return(-1);
	}
	return(1);		//这个也是这么简单粗暴，退下罢（ 
}

int AI_Play() {
	//你初始化没有？
	if(AI_Init==0) return(-1);	 
	//获取球排序
	int* BallRanking = (int*)malloc(sizeof(int)*(NUM-1));
	sortByDistance(BallRanking); 
	
	int hitted = 0;			//是否已经击球 
	int holeRnk,ballRnk;
	int* HoleRanking;
	for(ballRnk=0;ballRnk<NUM-1;ballRnk++) {		//For each Rank of Ball:
		//排除进洞的球 
		if(Ball[BallRanking[ballRnk]].State)continue;
		//从近到远，判断每一个球是否能被选中为目标击球
		if(isBallBlocking(BallRanking[ballRnk])) {
			//从离这个球最近的洞开始
			HoleRanking =  (int*)malloc(sizeof(int)*6);
			sortHoles(BallRanking[ballRnk], HoleRanking);
			//分别判断向每个洞击球是否可行 
			for(holeRnk=0;holeRnk<6;holeRnk++) {	//For each Rank of Hole:
				if(isPathBlocking(BallRanking[ballRnk], HoleRanking[holeRnk])) {
					//此时，从母球到ballRnk，从ballRnk到holeRnk的路径都是通畅的
					/* 击球！*/
						hitBall(BallRanking[ballRnk], HoleRanking[holeRnk]);
						printf("认真揍球，揍了%d球。\n",BallRanking[ballRnk]);
					hitted = 1; break;
				}
			}
		}
		//如果击球动作完成，则退出循环
		if(hitted) break; 
	}
	//如果没有找到合适的击球对象，则随机揍一个球
	if(hitted==0) 
	{
		int i;
		for(i = 1; i < NUM; i ++)
			if(Ball[i].State == ONTABLE)
				break;
		int target_ball = i;
		int target_hole = rand()%6;
		/* 击球 */
		hitBall(target_ball, target_hole);
		return(1);
		printf("随机揍球，揍了%d球。\n", target_ball);
		hitted = 1; 
	}
	
	//释放内存空间，以免占用过多内存 
	free(BallRanking);
	free(HoleRanking);
	//击完了。 
	return(0);
}
