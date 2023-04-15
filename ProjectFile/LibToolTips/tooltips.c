#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
//#pragma comment(lib, "comctl32.lib")

#include <tooltips.h>

extern HINSTANCE hInstance; //应用程序实例句柄
extern HWND graphicsWindow;

void InitToolTips(LPTOOLTIPS* tooltips,HWND hwndParent){
	if(!hwndParent)
		hwndParent = graphicsWindow;
	(*tooltips) = (LPTOOLTIPS)malloc(sizeof(TOOLTIPS));
	(*tooltips)->hwndParent = hwndParent;
	//创建 
	(*tooltips)->hTTWnd = CreateWindowEx(
							WS_EX_TOPMOST, 
							TOOLTIPS_CLASS, 
							NULL,
							WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
							CW_USEDEFAULT, 
							CW_USEDEFAULT, 
							CW_USEDEFAULT, 
							CW_USEDEFAULT,
							hwndParent, 
							NULL, 
							hInstance, 
							NULL);
	//设置 
	SetWindowPos((*tooltips)->hTTWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void DestroyToolTips(LPTOOLTIPS* tooltips){
	DestroyWindow((*tooltips)->hTTWnd);
	free(*tooltips);
	(*tooltips)=NULL;
}

void AddToolTips(LPTOOLTIPS tooltips,UID Id,int x,int y,int w,int h,char* Text){
	TOOLINFO ti = {0};
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = tooltips->hwndParent;
	ti.hinst = hInstance;
	ti.lpszText = Text;
	ti.uId = (UINT_PTR)Id;
	SetRect(&ti.rect,x,y,x+w,y+h);
	SendMessage(tooltips->hTTWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
	//SendMessage(hTTWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)(LPTOOLINFO)&ti);
}

void DelToolTips(LPTOOLTIPS tooltips,UID Id){
	TOOLINFO ti = {0};
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = tooltips->hwndParent;
	ti.hinst = hInstance;
	ti.uId = (UINT_PTR)Id;
	SendMessage(tooltips->hTTWnd, TTM_DELTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
}

