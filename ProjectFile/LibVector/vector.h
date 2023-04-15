#ifndef _vector_h
#define _vector_h

#define Dimension 2				//������ά�� 

typedef float VECTOR[Dimension];
typedef float MATRIX[2][2]; 
/* ��ȡ������ģ */
float Mod(VECTOR Vector);
/* �����ĵ�λ�� */
void UnitVec(VECTOR Vector);
/* ���뵥λ������ */
void DisToUnitVec(VECTOR Vector, float Position1_X, float Position1_Y, float Position2_X, float Position2_Y);
/* �ٶȵ�λ������ */
void VelToUnitVec(VECTOR Vector, float Velocity_X, float Velocity_Y);
/* �жϵ�λ�������������Ƿ��� */
int LinearEqual(VECTOR Vector1, VECTOR Vector2);
/* ��������������ˮƽ�������ɽ����ң����󷵻� 2 */
double VecCos(VECTOR Vector);
/* ��������������ˮƽ�������ɽ����ң����󷵻� 2 */
double VecSin(VECTOR Vector);

void RotationMatrix(MATRIX Mat, float theta);
void MatTimesVec(MATRIX Mat, VECTOR Vec);
void ReflectRotation(VECTOR Vec, float theta, float RotationRatio);
#endif

