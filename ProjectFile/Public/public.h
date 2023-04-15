#ifndef _public_h
#define _public_h

/*��������ַ*/
#define WEB_VERSION //�Ƿ���������� 
#define WEBSITE "https://20210530194500870.host.wqdlib.cn/" 
#define TIMEOUT 5000

/*����ͼ��&���ָ��*/ 
#define FILE_CURSOR "Data/Icon/cursor.cur"
#define FILE_ICON "Data/Icon/Billiards.ico"
#define FILE_TITLE_ICON "Data/Icon/BilliardsTitle.ico"

/*��Ƶ��ʽ*/ 
#define CHANNELS 2
#define SAMPLE_SIZE 16
#define SAMPLE_RATE 48000

/*�������λ������*/
#define POSCORRECT 0 //-80 

/*����ͼƬ��׺��*/
#define MASK_NAME "����" 
#define MARKED_COLOR RGB(0,255,0) 

/*��ý����������*/ 
#define MAX_MEDIA 50

/*�����ı����������*/
#define MAX_TEXT_BOX 16 

/*�´��ڵ���ͼƬ�������*/
#define MAX_PICTURES 10 

/*��ӭ������ʾʱ��&ͼƬ*/
#define WELCOME 1 
#define WELCOME_TIME 1000

/*����������������*/ 
#define MAX_FONTS 20
#define FONTS_NAME "�����������"
#define FONTS_PATH "Data/Font/�����������.ttf"
#define TITLE_FONTS_NAME "����Ͱ��ջ��� R"
#define TITLE_FONTS_PATH "Data/Font/ALIBABA-PUHUITI-REGULAR.ttf"
#define BUTTON_FONTS_NAME "������κ�������"
#define BUTTON_FONTS_PATH "Data/Font/������κ�������.ttf"
#define TEXT_FONTS_NAME "�Ǻ��ȿ� CN Medium"
#define TEXT_FONTS_PATH "Data/Font/milky-mono-cn-medium.ttf"

/*Ӧ�ó�������*/
#define EXE_NAME "Billiards.exe"

#include <windows.h>

/*���ݿ�ṹ��*/
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
