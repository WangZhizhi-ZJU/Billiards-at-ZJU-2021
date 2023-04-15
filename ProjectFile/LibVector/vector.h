#ifndef _vector_h
#define _vector_h

#define Dimension 2				//向量的维数 

typedef float VECTOR[Dimension];
typedef float MATRIX[2][2]; 
/* 求取向量的模 */
float Mod(VECTOR Vector);
/* 向量的单位化 */
void UnitVec(VECTOR Vector);
/* 距离单位向量化 */
void DisToUnitVec(VECTOR Vector, float Position1_X, float Position1_Y, float Position2_X, float Position2_Y);
/* 速度单位向量化 */
void VelToUnitVec(VECTOR Vector, float Velocity_X, float Velocity_Y);
/* 判断单位向量、零向量是否共线 */
int LinearEqual(VECTOR Vector1, VECTOR Vector2);
/* 计算生成向量与水平方向所成角余弦，错误返回 2 */
double VecCos(VECTOR Vector);
/* 计算生成向量与水平方向所成角正弦，错误返回 2 */
double VecSin(VECTOR Vector);

void RotationMatrix(MATRIX Mat, float theta);
void MatTimesVec(MATRIX Mat, VECTOR Vec);
void ReflectRotation(VECTOR Vec, float theta, float RotationRatio);
#endif

