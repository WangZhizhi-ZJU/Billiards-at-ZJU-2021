#ifndef _public_h
#define _public_h

/*服务器地址*/
#define WEB_VERSION //是否开启联网检查 
#define WEBSITE "https://20210530194500870.host.wqdlib.cn/" 
#define TIMEOUT 5000

/*窗体图标&鼠标指针*/ 
#define FILE_CURSOR "Data/Icon/cursor.cur"
#define FILE_ICON "Data/Icon/Billiards.ico"
#define FILE_TITLE_ICON "Data/Icon/BilliardsTitle.ico"

/*音频格式*/ 
#define CHANNELS 2
#define SAMPLE_SIZE 16
#define SAMPLE_RATE 48000

/*窗体居中位置修正*/
#define POSCORRECT 0 //-80 

/*遮罩图片后缀名*/
#define MASK_NAME "遮罩" 
#define MARKED_COLOR RGB(0,255,0) 

/*打开媒体的最大数量*/ 
#define MAX_MEDIA 50

/*创建文本框最大数量*/
#define MAX_TEXT_BOX 16 

/*新窗口弹出图片最大数量*/
#define MAX_PICTURES 10 

/*欢迎窗口显示时间&图片*/
#define WELCOME 1 
#define WELCOME_TIME 1000

/*载入字体的最大数量*/ 
#define MAX_FONTS 20
#define FONTS_NAME "汉仪星宇体简"
#define FONTS_PATH "Data/Font/汉仪星宇体简.ttf"
#define TITLE_FONTS_NAME "阿里巴巴普惠体 R"
#define TITLE_FONTS_PATH "Data/Font/ALIBABA-PUHUITI-REGULAR.ttf"
#define BUTTON_FONTS_NAME "方正北魏楷书简体"
#define BUTTON_FONTS_PATH "Data/Font/方正北魏楷书简体.ttf"
#define TEXT_FONTS_NAME "星汉等宽 CN Medium"
#define TEXT_FONTS_PATH "Data/Font/milky-mono-cn-medium.ttf"

/*应用程序名称*/
#define EXE_NAME "Billiards.exe"

#include <windows.h>

/*数据库结构体*/
typedef struct
{
	long pos;
	DWORD filesize;
} DATAPOS,*LPDATAPOS;

typedef struct
{
	long num;
	char *basename;
	LPDATAPOS datapos;
} DATABASE,*LPDATABASE;

#endif
