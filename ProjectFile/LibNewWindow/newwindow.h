#ifndef _newwindow_h
#define _newwindow_h

#include <windows.h>

/*�½�һ������*/ 
/*���������ơ����ڱ��⡢���ڴ�С����Ϣ�ص����������������ġ��ڴ�������*/ 
HWND NewWindow(char* ClassName,char* windowTitle,int Width,int Height,WNDPROC EventProc,HDC* hDC,HDC* hMemoryDC);

/*�ı䴰�ڴ�С��������ʾ*/
void ChangeWindowSize(HWND hWnd,HDC hMemoryDC,int Width,int Height); 

/*��ӭ����*/
void WelcomeWindow();

/*�˳�ȷ�ϴ���*/
void ExitWindow();
 
/*���ֲ��Ŵ����ڳ�ʼ��*/
extern HWND MenuMusicWindow;
extern HWND CollisionMusicWindow;
extern HWND BackGroundMusicWindow;
void InitMusicWindow(); 

/*�������BGM*/
void BackGroundMusic();

enum{
	BGM_PLAY,
	BGM_PAUSE,
	BGM_VOLUME,
};

#endif
