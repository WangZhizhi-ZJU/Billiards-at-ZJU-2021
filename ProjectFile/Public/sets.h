#ifndef _sets_h
#define _sets_h

#include <graphics.h>

/* 关键信息 */
#define VK_ESCAPE 0x1B 		//Esc键 
#define pi 3.14159265 
/* 系统尺寸数据 */
#define WIDTH 1280			//窗口宽度 
#define HEIGHT 773 			//窗口高度
#define TITLEHEIGHT 53 		//标题栏高度

/* 元件尺寸数据 */
#define Ball_R 16			//球半径
#define Hole_R 30			//洞口半径
#define R InchesX(Ball_R) 

#define Gap 2							//球体间隙
#define UpBroad 60 + TITLEHEIGHT		//球台上边沿与屏幕上边框距离 
#define DownBroad 575 + TITLEHEIGHT		//球台下边沿与屏幕上边框距离 
#define LeftBroad 128					//球台左边沿与屏幕左边框距离 
#define RightBroad 1149					//球台有边沿与屏幕右边框距离 

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

/* 物理系数 */
//#define K_Friction 0.9	//阻尼效果系数，这里修改不方便，暂时移到主函数 
extern double K_Friction;
#define K_VXSwitch 1.0		//速位转换系数，使用方式 X = X + V * K_VXSwitch，模拟速度对时间的累积效果 
#define K_StopV 0.3    		//直接设定静止的临界速度
#define class struct
/* 变量类型定义 */
typedef enum{ONTABLE = 0, INHOLE} POS;
typedef enum{STOP = 0, MOVING} STATUS; 
typedef enum{GM_UNDECIDED = 0, GM_SINGLEPLAYER, GM_MULTIPLAYER, GM_POOL, GM_TRAINNING, GM_SNOOKER, GM_BACK} MODE;		//分别代表未选择、单人游戏、多人游戏、回退（每一次回退完成后要重新赋值为UNDECIDED）  
 
void ResetSets();
void InitSets(); 

#endif
