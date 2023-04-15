#include <stdio.h>
#include <math.h>
#include <vector.h>

float Mod(VECTOR Vector)
{
	int i;
	float Res = 0;
	for(i = 0; i < Dimension; i ++)
		Res += Vector[i] * Vector[i];
	return sqrt(Res);
}

void UnitVec(VECTOR Vector)
{
	int i;
	float mod = Mod(Vector);
	if(mod != 0)
		for(i = 0; i < Dimension; i ++)
			Vector[i] /= mod;
	else;
}

void DisToUnitVec(VECTOR Vector, float Position1_X, float Position1_Y, float Position2_X, float Position2_Y)
{
	Vector[0] = Position2_X - Position1_X;
	Vector[1] = Position2_Y - Position1_Y;
	UnitVec(Vector);	
}

void VelToUnitVec(VECTOR Vector, float Velocity_X, float Velocity_Y)
{
	Vector[0] = Velocity_X;
	Vector[1] = Velocity_Y;
	UnitVec(Vector);	
}

int LinearEqual(VECTOR Vector1, VECTOR Vector2)
{
	int i, flag = 0;
	
	for(i = 0; i < Dimension; i ++)			//如果两向量同向相等 
		if(Vector1[i] != Vector2[i])
			break;
	if(i >= Dimension)
		return 1;
		
	for(i = 0; i < Dimension; i ++)			//如果两向量反向相等 
		if(Vector1[i] != - Vector2[i])
			break;
	if(i >= Dimension)		
		return 1;
		
	for(i = 0; i < Dimension; i ++)			//如果向量 1是零向量 
		if(Vector1[i] != 0)
			break;
	if(i >= Dimension)
		return 1;
		
	for(i = 0; i < Dimension; i ++)			//如果向量 2是零向量
		if(Vector2[i] != 0)
			break;
	if(i >= Dimension)
		return 1;
			
	return 0;
}

double VecCos(VECTOR Vector)
{
	if(Dimension > 2)						//不支持 2维以上的向量 
		return 2;
	if(Mod(Vector) == 0)					//零向量返回 2 
		return 0;
	return (Vector[0] / Mod(Vector));
}

double VecSin(VECTOR Vector)
{
	if(Dimension > 2)
		return 2;
	if(Mod(Vector) == 0)					//零向量返回 2 
		return 0;
	return (Vector[1] / Mod(Vector));
} 


void RotationMatrix(MATRIX Mat, float theta)
{
	Mat[0][0] = cos(theta);
	Mat[0][1] = - sin(theta);
	Mat[1][0] = sin(theta);
	Mat[1][1] = cos(theta);
}

void MatTimesVec(MATRIX Mat, VECTOR Vec)
{
	VECTOR Res = {0, 0};
	int i, j;
	for(i = 0; i < 2; i ++)
		for(j = 0; j < 2; j ++)
			Res[i] += Mat[i][j] * Vec[j];
	Vec[0] = Res[0];
	Vec[1] = Res[1];
}

void ReflectRotation(VECTOR Vec, float theta, float RotationRatio)
{
	VECTOR Res = {0, 0};
	MATRIX Mat = {{0, 0}, {0, 0}};
	Mat[0][0] = cos(theta * RotationRatio);
	Mat[0][1] = - sin(theta * RotationRatio);
	Mat[1][0] = sin(theta * RotationRatio);
	Mat[1][1] = cos(theta * RotationRatio);
	int i, j;
	for(i = 0; i < 2; i ++)
		for(j = 0; j < 2; j ++)
			Res[i] += Mat[i][j] * Vec[j];
	Vec[0] = Res[0];
	Vec[1] = Res[1];	
}
