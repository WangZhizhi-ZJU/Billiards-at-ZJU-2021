#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <pictures.h> 
#include <sets.h>
#include <physics.h>
#include <vector.h>

#include <math.h>

#define INF_DISTANCE 998244353.01					//��~��~Զ��

//����ģʽ�Ƿ��� 
//#define DEBUG_MODE

extern float MaxVelocity;
extern int NUM;
extern float Hole_X[6];
extern float Hole_Y[6];
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
} Ball[16];

static int AI_Inited = 0;							//AI��ʼ��״̬��¼ 

//ͨ���������������꣬���������ε������
inline float TriangleArea(float x1, float y1, float x2, float y2, float x3, float y3) {
	return(0.50*(x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2));
}

//�жϵ������Ƿ��赲������֮���·��������1���赲������0���赲��
inline int isBlocked(float x1, float y1, float x2, float y2, float x, float y) {
	return((TriangleArea(x1,y1,x2,y2,x,y) / Distance(x1,y1,x2,y2) > 0.50 * Ball_R) ? 1 : 0);
}

extern float Power;
extern int CurrentPosition_X;
extern int CurrentPosition_Y;

//����
inline void hitBall(int ball_id, int hole_id) 
{
	//������õ����Ѿ������ˣ�����ôҲ�����ץһ����һ�°� 
	if(Ball[ball_id].State == INHOLE) 
	{
		Ball[0].Velocity_X = rand() % 5 - 2.50;
		Ball[0].Velocity_Y = rand() % 5 - 2.50;
		CurrentPosition_X = Ball[0].Position_X + 100 * Ball[0].Velocity_X / (sqrt(Ball[0].Velocity_X*Ball[0].Velocity_X + Ball[0].Velocity_Y*Ball[0].Velocity_Y));
		CurrentPosition_Y = Ball[0].Position_Y + 100 * Ball[0].Velocity_Y / (sqrt(Ball[0].Velocity_X*Ball[0].Velocity_X + Ball[0].Velocity_Y*Ball[0].Velocity_Y));
		return;
	}
	//��������ĸ��Ŀ���� 
	VECTOR Direction = {Ball[ball_id].Position_X - Ball[0].Position_X, Ball[ball_id].Position_Y - Ball[0].Position_Y};
	//��������Ŀ����Ŀ�궴 
	VECTOR Shift = {Hole_X[hole_id] - Ball[ball_id].Position_X, Hole_Y[hole_id] - Ball[ball_id].Position_Y};
	//��������
	float PowerFactor = Mod(Direction) + Mod(Shift); 
	Power = PowerFactor / 10.0 > 100.0 ? 100.0 : PowerFactor / 10.0;
	//����ƫ�����������ܱ�����Ŀ��ķ��� 
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

//�ж��Ƿ������赲��ָ��id����ĸ��֮���·��������1���赲������0���赲��
//ע�⣺����������ῼ��ĸ���ⲻ�ϻ���ι�� 
int isBallBlocking(int ball_id) 
{
	//���idȡֵ��ΧΪ1~(NUM-1)�������򷵻�-1
	if(ball_id<1||ball_id>NUM-1) return(-1);
	
	//�����Ǻ����жϴ��� 
	int i;
	int ret = 1;
	for(i=1;i<NUM;i++) {
		if(i!=ball_id) {
			//��ÿ����ץ���ж�һ�Σ���Ȼ�����Լ����Լ��жϰ��� 
			ret = ret & isBlocked ( Ball[ball_id].Position_X, Ball[ball_id].Position_Y	, 	//ball_id������ 
									Ball[0].Position_X, Ball[0].Position_Y				,   //ĸ������� 
									Ball[i].Position_X, Ball[i].Position_Y				);  //��i��������� 
		}
	}
	return(ret);
}

//�ж��Ƿ������赲��ָ��id����ָ����֮���·��������1���赲������0���赲��
//ע�⣺����������ῼ��ĸ��
int isPathBlocking(int ball_id, int hole_id) {
	
	//���idȡֵ��ΧΪ1~(NUM-1)�������򷵻�-1
	if(ball_id<1||ball_id>NUM-1) return(-1);
	//����idȡֵ��ΧΪ1~6�������򷵻�-1
	if(hole_id<1||hole_id>6) return(-1);
	
	//�����Ǻ����жϴ��� 
	int i;
	int ret = 1;
	for(i=1;i<NUM;i++) {
		if(i!=ball_id) {
			//��ÿ����ץ���ж�һ�Σ���Ȼ�����Լ����Լ��жϰ��� 
			ret = ret & isBlocked ( Ball[ball_id].Position_X, Ball[ball_id].Position_Y	, 	//ball_id������ 
									Hole_X[hole_id], Hole_Y[hole_id]					,   //hole_id������ 
									Ball[i].Position_X, Ball[i].Position_Y				);  //��i��������� 
		}
	}
	return(ret);
}

//�ж�ָ�����Ƿ���ָ�����"��ֱ�ӻ�������"�ڣ�����1���ڣ�����0���ڡ�
//"��ֱ�ӻ�������"��ָĸ�����ָ�����ָ���򲻾�������Ҳ�ܴﵽ������
//�ж�ԭ�����Ҷ���������ʽ 
int isArrivable(int ball_id, int hole_id) {
	//��Ϊ���ô���д�ܳ�̫�ѿ��ˣ�������ͼԤ��������ú�����жϼ��һЩ 
	double 	d1 = Distance(Ball[0].Position_X, Ball[0].Position_Y, Ball[ball_id].Position_X, Ball[ball_id].Position_Y),
			d2 = Distance(Ball[ball_id].Position_X, Ball[ball_id].Position_Y, Hole_X[hole_id], Hole_Y[hole_id]),
			d3 = Distance(Ball[0].Position_X, Ball[0].Position_Y, Hole_X[hole_id], Hole_Y[hole_id]);
	//��ƽ��~ 
	d1*=d1, d2*=d2, d3*=d3;
	return((d1+d2<d3)?1:0);
}

//��һ������6������������д�����ݣ����е��������Ƕ���ID��������ball_id�����ľ����ɽ���Զ����
void sortHoles(int ball_id, int* ArrayPtr) {
	
	//�����������鲢û����ô��Ԫ�أ�����ֱ�Ӳ�������
	ArrayPtr[5] = 0;
	int i,j,max_idx;
	double max_dis;
	//���������� 
	double DistanceArray[6];
	for(i=0;i<6;i++) DistanceArray[i] = Distance(Ball[ball_id].Position_X, Ball[ball_id].Position_Y, Hole_X[i], Hole_Y[i]);
	//��������ͦ�ٵģ����ö����µ�structureд������.jpg
	
	//һ��ѭ����Ѱ�Ҿ����i+1���Ķ�
	for(i=0;i<6;i++) {
		max_idx = -1, max_dis = INF_DISTANCE;
		//����ѭ������Ǿ�������Ķ�
		for(j=0;j<6;j++) {
			if(DistanceArray[j] < max_dis) {
				max_idx = j, max_dis = DistanceArray[j];
			}
		}
		
		//���ҵ�����idд�����飬����������Ϊ����Զ�����ظ�д��
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

//��һ������NUM-1������������д�����ݣ����е������������ID�����������ĸ���ɽ���Զ���� 
void sortByDistance(int* ArrayPtr) 
{
	//�����������鲢û����ô��Ԫ�أ�����ֱ�Ӳ�������
	ArrayPtr[NUM-2] = 0;
	 
	int i,j,max_idx;
	double max_dis;
	//����������
	double DistanceArray[NUM];
	DistanceArray[0] = 0;
	for(i = 1;i<NUM;i++) DistanceArray[i] = Distance( Ball[0].Position_X, Ball[0].Position_Y  , 
													Ball[i].Position_X, Ball[i].Position_Y );
	//��������ͦ�ٵģ����ö����µ�structureд������.jpg���㻹���Ǹ����������������� 
	
	//һ��ѭ����Ѱ�Ҿ����i+1������ 
	for(i = 0;i<NUM-1;i++) {
		max_idx = -1, max_dis = INF_DISTANCE;
		//����ѭ������Ǵ�ʱ�����������
		for(j=1;j<NUM;j++) {
			if(DistanceArray[j] < max_dis) {
				max_idx = j, max_dis = DistanceArray[j];
			}
		}
		//���ҵ�����idд�����飬����������Ϊ����Զ�����ظ�д�� 
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
	д����ô�����߰���ĺ���������Ҫ����������
*/

int AI_Init() {
	srand(time(NULL));
	AI_Inited = 1;
	return(0);		//�򵥴ֱ�
}

int AI_Clear() {
	if(AI_Inited) {
		AI_Inited = 0;
		return(0);
	} else {
		return(-1);
	}
	return(1);		//���Ҳ����ô�򵥴ֱ������°գ� 
}

int AI_Play() {
	//���ʼ��û�У�
	if(AI_Init==0) return(-1);	 
	//��ȡ������
	int* BallRanking = (int*)malloc(sizeof(int)*(NUM-1));
	sortByDistance(BallRanking); 
	
	int hitted = 0;			//�Ƿ��Ѿ����� 
	int holeRnk,ballRnk;
	int* HoleRanking;
	for(ballRnk=0;ballRnk<NUM-1;ballRnk++) {		//For each Rank of Ball:
		//�ų��������� 
		if(Ball[BallRanking[ballRnk]].State)continue;
		//�ӽ���Զ���ж�ÿһ�����Ƿ��ܱ�ѡ��ΪĿ�����
		if(isBallBlocking(BallRanking[ballRnk])) {
			//�������������Ķ���ʼ
			HoleRanking =  (int*)malloc(sizeof(int)*6);
			sortHoles(BallRanking[ballRnk], HoleRanking);
			//�ֱ��ж���ÿ���������Ƿ���� 
			for(holeRnk=0;holeRnk<6;holeRnk++) {	//For each Rank of Hole:
				if(isPathBlocking(BallRanking[ballRnk], HoleRanking[holeRnk])) {
					//��ʱ����ĸ��ballRnk����ballRnk��holeRnk��·������ͨ����
					/* ����*/
						hitBall(BallRanking[ballRnk], HoleRanking[holeRnk]);
						printf("������������%d��\n",BallRanking[ballRnk]);
					hitted = 1; break;
				}
			}
		}
		//�����������ɣ����˳�ѭ��
		if(hitted) break; 
	}
	//���û���ҵ����ʵĻ�������������һ����
	if(hitted==0) 
	{
		int i;
		for(i = 1; i < NUM; i ++)
			if(Ball[i].State == ONTABLE)
				break;
		int target_ball = i;
		int target_hole = rand()%6;
		/* ���� */
		hitBall(target_ball, target_hole);
		return(1);
		printf("�����������%d��\n", target_ball);
		hitted = 1; 
	}
	
	//�ͷ��ڴ�ռ䣬����ռ�ù����ڴ� 
	free(BallRanking);
	free(HoleRanking);
	//�����ˡ� 
	return(0);
}
