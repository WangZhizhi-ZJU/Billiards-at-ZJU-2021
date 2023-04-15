#ifndef _text_h
#define _text_h

#include <windows.h>
#include <stdbool.h>
/*��ʾ���庯����*/

typedef LOGFONT *LPFONTS; 

/*���������ʼ��*/ 
void LoadFonts(char *fontsname,char *fontspath,LPFONTS* lf); 

/*ж����ʱ��װ������ */
void RemoveFonts(); 

/*��ʱ��װ������ */
bool AddFonts(char *fontspath); 

/*��������*/
void PutText(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC); 
void PutTextB(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC); //�������������
void PutTextC(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC); //�����Ҷ������� 

/*�ı���ؼ�*/
typedef struct{
	/*λ��&��С*/
	int x,y;
	int w,h;
	/*������*/ 
	int len;
	int pos;
	int* size;
	char* text;
	/*��������*/
	HFONT font; 
	int fontsize;
	TEXTMETRIC metrics;
	/*���λ��*/
	int curpos;
	/*��ɫ*/
	COLORREF color; // BGR ��ɫ 0xFF - (char) 
	/*ѡ���ַ�*/
	int in,out;
	/*����*/
	HDC hMDC;
	HWND hWnd;
	/*����*/
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

/*��������*/ 
bool InitCheck();
void DrawTextsBox();
void ActiveTextBox(LPTEXT text);
void InitSetText(LPTEXT text,char* _text);

/*�������ͷš�ע�ᡢע��*/
void CancelTextBox(); 
void RegistTextBox(LPTEXT* text);
void FreeTextBox(LPTEXT* text);
void InitTextBox(LPTEXT* text,int x,int y,int w,int h,int len,int R,int G,int B,LPFONTS font,int fontsize,HWND hWnd,HDC hMDC);

/*�ص�����*/
void TextBoxChar(char c);
void TextBoxKeyboard(int key,int event);
void TextBoxMouse(int x, int y, int button, int event);

#endif
