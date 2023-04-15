#ifndef _sets_h
#define _sets_h

#include <graphics.h>

/* �ؼ���Ϣ */
#define VK_ESCAPE 0x1B 		//Esc�� 
#define pi 3.14159265 
/* ϵͳ�ߴ����� */
#define WIDTH 1280			//���ڿ�� 
#define HEIGHT 773 			//���ڸ߶�
#define TITLEHEIGHT 53 		//�������߶�

/* Ԫ���ߴ����� */
#define Ball_R 16			//��뾶
#define Hole_R 30			//���ڰ뾶
#define R InchesX(Ball_R) 

#define Gap 2							//�����϶
#define UpBroad 60 + TITLEHEIGHT		//��̨�ϱ�������Ļ�ϱ߿���� 
#define DownBroad 575 + TITLEHEIGHT		//��̨�±�������Ļ�ϱ߿���� 
#define LeftBroad 128					//��̨���������Ļ��߿���� 
#define RightBroad 1149					//��̨�б�������Ļ�ұ߿���� 

#define Hole1_X 140
#define Hole1_Y 70+TITLEHEIGHT
#define Hole2_X 640
#define Hole2_Y 78+TITLEHEIGHT
#define Hole3_X 1140
#define Hole3_Y 70+TITLEHEIGHT 
#define Hole4_X 140
#define Hole4_Y 566+TITLEHEIGHT 
#define Hole5_X 640
#define Hole5_Y 558+TITLEHEIGHT
#define Hole6_X 1140
#define Hole6_Y 566+TITLEHEIGHT
#define Hole5_A 616
#define Hole5_B 575+TITLEHEIGHT
#define Hole5_C 664 
#define Hole5_D 635+TITLEHEIGHT
#define Hole2_A 616
#define Hole2_B 0+TITLEHEIGHT
#define Hole2_C 664 
#define Hole2_D 60+TITLEHEIGHT
#define DownBroadNearHoleL_X 596
#define DownBroadNearHoleR_X 684
#define DownBroadNearHoleU_Y 575+TITLEHEIGHT
#define DownBroadNearHoleD_Y 595+TITLEHEIGHT
#define UpBroadNearHoleL_X 596
#define UpBroadNearHoleR_X 684
#define UpBroadNearHoleU_Y 40+TITLEHEIGHT
#define UpBroadNearHoleD_Y 60+TITLEHEIGHT

/* ����ϵ�� */
//#define K_Friction 0.9	//����Ч��ϵ���������޸Ĳ����㣬��ʱ�Ƶ������� 
extern double K_Friction;
#define K_VXSwitch 1.0		//��λת��ϵ����ʹ�÷�ʽ X = X + V * K_VXSwitch��ģ���ٶȶ�ʱ����ۻ�Ч�� 
#define K_StopV 0.3    		//ֱ���趨��ֹ���ٽ��ٶ�
#define class struct
/* �������Ͷ��� */
typedef enum{ONTABLE = 0, INHOLE} POS;
typedef enum{STOP = 0, MOVING} STATUS; 
typedef enum{GM_UNDECIDED = 0, GM_SINGLEPLAYER, GM_MULTIPLAYER, GM_POOL, GM_TRAINNING, GM_SNOOKER, GM_BACK} MODE;		//�ֱ����δѡ�񡢵�����Ϸ��������Ϸ�����ˣ�ÿһ�λ�����ɺ�Ҫ���¸�ֵΪUNDECIDED��  
 
void ResetSets();
void InitSets(); 

#endif
