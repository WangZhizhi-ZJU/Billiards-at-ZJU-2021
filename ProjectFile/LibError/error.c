#include <windows.h>
#include <process.h>

#include <course.h>
#include <setting.h>

extern HWND graphicsWindow;

void ErrorMsg(char* msg){
	MessageBox(NULL,msg,"Billiards Error",16);
	HideCourseWindow(); 
	HideSettingWindow(); 
	PostMessage(graphicsWindow,WM_CLOSE,0,0);
	FreeConsole();
	exit(0);
}
