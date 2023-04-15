#ifndef _newwindow_h
#define _newwindow_h

#include <windows.h>

/*新建一个窗体*/ 
/*窗口类名称、窗口标题、窗口大小、消息回调函数、窗口上下文、内存上下文*/ 
HWND NewWindow(char* ClassName,char* windowTitle,int Width,int Height,WNDPROC EventProc,HDC* hDC,HDC* hMemoryDC);

/*改变窗口大小并居中显示*/
void ChangeWindowSize(HWND hWnd,HDC hMemoryDC,int Width,int Height); 

/*欢迎窗口*/
void WelcomeWindow();

/*退出确认窗口*/
void ExitWindow();
 
/*音乐播放处理窗口初始化*/
extern HWND MenuMusicWindow;
extern HWND CollisionMusicWindow;
extern HWND BackGroundMusicWindow;
void InitMusicWindow(); 

/*随机播放BGM*/
void BackGroundMusic();

enum{
	BGM_PLAY,
	BGM_PAUSE,
	BGM_VOLUME,
};

#endif
