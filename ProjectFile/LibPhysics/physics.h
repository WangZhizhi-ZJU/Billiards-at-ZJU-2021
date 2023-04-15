#ifndef _physic_h
#define _physic_h

/*
 * 工具函数: Distance
 * ------------------------------------------
 * Distance能够实现计算两点间距离。 
 */
double Distance(double DotA_X, double DotA_Y, double DotB_X, double DotB_Y);

/* 物理效果函数 */
/*
 * 函数族: MoveUpdate, Move, Friction, Collision, Stop
 * ------------------------------------------
 * Collision函数用以判断每个小球的碰撞情况，并重新修改球的水平垂直速度分量， 
 * Move模拟速度对时间的累积，Friction(拟运动时收到的阻力，Stop则让球的速度
 * 在小于某个临界值K_StopV时设置为运动停止。MoveUpdate是对四个效果的封装。
 */

void PreJudge(); 
void MoveUpdate(int i);
void Move(int i);
void Friction(int i);
void Collision(int i);
void Stop(int i);

/*
 * 函数: ScoreJudge() 
 * ------------------------------------------
 * ScoreJudge()函数用于遍历每一个球和每一个球洞的位置关系来判断球是否进洞，母球进洞则重置随机位置。 
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
