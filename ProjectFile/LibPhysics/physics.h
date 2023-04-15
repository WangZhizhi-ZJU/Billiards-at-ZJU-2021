#ifndef _physic_h
#define _physic_h

/*
 * ���ߺ���: Distance
 * ------------------------------------------
 * Distance�ܹ�ʵ�ּ����������롣 
 */
double Distance(double DotA_X, double DotA_Y, double DotB_X, double DotB_Y);

/* ����Ч������ */
/*
 * ������: MoveUpdate, Move, Friction, Collision, Stop
 * ------------------------------------------
 * Collision���������ж�ÿ��С�����ײ������������޸����ˮƽ��ֱ�ٶȷ����� 
 * Moveģ���ٶȶ�ʱ����ۻ���Friction(���˶�ʱ�յ���������Stop��������ٶ�
 * ��С��ĳ���ٽ�ֵK_StopVʱ����Ϊ�˶�ֹͣ��MoveUpdate�Ƕ��ĸ�Ч���ķ�װ��
 */

void PreJudge(); 
void MoveUpdate(int i);
void Move(int i);
void Friction(int i);
void Collision(int i);
void Stop(int i);

/*
 * ����: ScoreJudge() 
 * ------------------------------------------
 * ScoreJudge()�������ڱ���ÿһ�����ÿһ���򶴵�λ�ù�ϵ���ж����Ƿ������ĸ��������������λ�á� 
 */
void ScoreJudge();

#include <vector.h>
void PowerControl();
void PowerToVelocity(float Power, VECTOR UnitVector);
void ExerciseBallReset();
void AIFreeResetCueBall();

#include <stdbool.h>
bool Ontable(float X, float Y);
void PoolRules();
void ClassicRules();
bool isTargetValid();
bool isScoreValid(); 
void GetTargetBall();
void GetPoolInfo();
void SetInholeBallsAfterConfirmed();
void BallReset(int i);
void ResetTwoBalls();

void Swirl();
void GetSwirl(int HitPosition_X, int HitPosition_Y);

void InitPool();
void InitClassic();
void InitRotation();
void InitTrainning();
void InitBallVelocity();
void InitSnooker();

int isScoreLegal();
bool isTargetLegal();
void GetSnookerInfo();
void GameJudger();
void ResetBalls();
void RedClearJudge();
void ResetBlackBall();

#endif
