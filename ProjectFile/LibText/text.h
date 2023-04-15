#ifndef _text_h
#define _text_h

#include <windows.h>
#include <stdbool.h>
/*显示字体函数库*/

typedef LOGFONT *LPFONTS; 

/*字体参数初始化*/ 
void LoadFonts(char *fontsname,char *fontspath,LPFONTS* lf); 

/*卸载临时安装的字体 */
void RemoveFonts(); 

/*临时安装新字体 */
bool AddFonts(char *fontspath); 

/*绘制字体*/
void PutText(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC); 
void PutTextB(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC); //绘制左对齐字体
void PutTextC(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC); //绘制右对齐字体 

/*文本框控件*/
typedef struct{
	/*位置&大小*/
	int x,y;
	int w,h;
	/*缓冲区*/ 
	int len;
	int pos;
	int* size;
	char* text;
	/*加载字体*/
	HFONT font; 
	int fontsize;
	TEXTMETRIC metrics;
	/*光标位置*/
	int curpos;
	/*颜色*/
	COLORREF color; // BGR 反色 0xFF - (char) 
	/*选择字符*/
	int in,out;
	/*窗口*/
	HDC hMDC;
	HWND hWnd;
	/*方法*/
	void (*End)();
	void (*Home)();
	void (*Back)();
	void (*Read)(char c);
	void (*Delete)();
	void (*Left)();
	void (*Right)();
	void (*GetPos)(int x);
	void (*SetClip)();
	void (*GetClip)();
	void (*SetText)(char* text);
	char* (*GetText)();
	void (*UnSelect)();
	bool (*IsSelect)();
	void (*SelectAll)();
	void (*GetCurPos)();
	
}TEXT,*LPTEXT;

/*基本操作*/ 
bool InitCheck();
void DrawTextsBox();
void ActiveTextBox(LPTEXT text);
void InitSetText(LPTEXT text,char* _text);

/*创建、释放、注册、注销*/
void CancelTextBox(); 
void RegistTextBox(LPTEXT* text);
void FreeTextBox(LPTEXT* text);
void InitTextBox(LPTEXT* text,int x,int y,int w,int h,int len,int R,int G,int B,LPFONTS font,int fontsize,HWND hWnd,HDC hMDC);

/*回调函数*/
void TextBoxChar(char c);
void TextBoxKeyboard(int key,int event);
void TextBoxMouse(int x, int y, int button, int event);

#endif
