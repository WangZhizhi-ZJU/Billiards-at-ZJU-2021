/*获取控制台消息句柄*/
#include <stdio.h>
#include <stdbool.h>
#include <windows.h> 

#include <public.h>

extern HWND graphicsWindow;

static bool CALLBACK CtrlHandler(DWORD fdwCtrlType);// 控制台窗口消息处理回调函数 
void GetConsoleMsg(){
	if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler,true))
    	printf("Control handler setting failed....\n"); 
}

static bool CALLBACK CtrlHandler(DWORD fdwCtrlType){ 
	switch(fdwCtrlType){     
		case CTRL_C_EVENT:
			RemoveFontResource(FONTS_PATH); 
			RemoveFontResource(TITLE_FONTS_PATH); 
			RemoveFontResource(BUTTON_FONTS_PATH); 
			RemoveFontResource(TEXT_FONTS_PATH); 
			printf("Ctrl-C event\n");
			printf("手动删除字体\n");
		return true;

		case CTRL_CLOSE_EVENT:
		case CTRL_LOGOFF_EVENT: //注销 
		case CTRL_SHUTDOWN_EVENT: //关机 
			RemoveFonts();
			ExitMusicWindow();
            PostMessage(graphicsWindow,WM_CLOSE,0,0);
		return true; 

		case CTRL_BREAK_EVENT:       
			printf("Ctrl-Break event\n");
		return true; 

		default: 
			return false; // pass thru, let the system to handle the event.
	} 
}
