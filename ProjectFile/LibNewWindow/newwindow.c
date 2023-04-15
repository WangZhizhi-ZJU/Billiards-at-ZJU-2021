#include <Windows.h>
#include <Public.h>
#include <stdio.h>

#include <mmsystem.h>
#include <pictures.h>
#include <media.h>
//#include <gui.h>

#include <newwindow.h>

int TimeSysID;
int MenuMusicID;
int CollisionMusicID;
int BackGroundMusicID;

HWND TimeSysWindow;
HWND MenuMusicWindow;
HWND CollisionMusicWindow;
HWND BackGroundMusicWindow;

extern HWND graphicsWindow;

extern HCURSOR CS_Cursor; 

/*窗口类名称、窗口标题、窗口大小、消息回调函数、窗口上下文、内存上下文*/ 
HWND NewWindow(char* ClassName,char* windowTitle,int Width,int Height,WNDPROC EventProc,HDC* hDC,HDC* hMemoryDC){
	HWND hwnd;
	WNDCLASSEX wc; 
	extern HINSTANCE hInstance;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor       = CS_Cursor;
    wc.hIcon         = (HICON)LoadImage(NULL,FILE_ICON,IMAGE_ICON,0,0,LR_LOADFROMFILE);//程序图标 
    wc.hIconSm       = (HICON)LoadImage(NULL,FILE_TITLE_ICON,IMAGE_ICON,0,0,LR_LOADFROMFILE);//标题栏图标 
    wc.hInstance     = hInstance;
    wc.lpfnWndProc   = (WNDPROC)EventProc; //窗口过程回调函数
    wc.lpszClassName = ClassName;
    wc.lpszMenuName  = NULL;
	wc.style         = CS_HREDRAW | CS_VREDRAW;  
	RegisterClassEx(&wc);
    HWND desktop;
    RECT bounds;
	desktop = GetDesktopWindow();
    GetWindowRect(desktop, &bounds);
	int screenWidth = bounds.right - bounds.left;
	int screenHeight = bounds.bottom - bounds.top;
    int WIDTH=(screenWidth-Width)/2;
	int HEIGHT=(screenHeight-Height)/2;
	hwnd = CreateWindowEx(
	  0,//WS_EX_TOPMOST,
      ClassName,
      windowTitle, 
      WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME | WS_MINIMIZEBOX), 
      WIDTH,
      HEIGHT,
      Width, 
      Height,
      graphicsWindow, 
      NULL,
      hInstance,
      (LPSTR) NULL);
    if (hwnd == NULL) 
        printf("InitGraphics: CreateGraphicsWindow failed\n");
    GetClientRect(hwnd, &bounds); //窗口大小修正 
    int dx = Width - RectWidth(&bounds);
    int dy = Height - RectHeight(&bounds);
    SetWindowPos(hwnd, HWND_TOP,
                 WIDTH,HEIGHT,
                 Width + dx,
                 Height + dy, 0);
    printf("Window: %06X\n",hwnd);
	*hDC=GetDC(hwnd);
	*hMemoryDC=CreateCompatibleDC(*hDC);
	if (*hMemoryDC == NULL)
        printf("Internal error: Can't create offscreen device\n");
	HBITMAP osBits = CreateCompatibleBitmap(*hDC,Width,Height);
	if (osBits == NULL)
        printf("Internal error: Can't create offscreen bitmap");
	SelectObject(*hMemoryDC,osBits);
	RECT r;
    SetRect(&r, 0, 0, Width, Height);
    InvalidateRect(hwnd, &r, TRUE);
    BitBlt(*hMemoryDC, 0, 0, Width, Height, NULL, 0, 0, WHITENESS);
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
	return hwnd;
}

void ChangeWindowSize(HWND hWnd,HDC hMemoryDC,int Width,int Height){ //改变窗口大小并居中显示 
	SetWindowPos(hWnd, HWND_TOP, 0, 0, Width, Height, SWP_NOMOVE|SWP_DRAWFRAME);
	HWND desktop;
    RECT bounds;
	desktop = GetDesktopWindow();
    GetWindowRect(desktop, &bounds);
	int screenWidth = bounds.right - bounds.left;
	int screenHeight = bounds.bottom - bounds.top;
    int WIDTH=(screenWidth-Width)/2;
	int HEIGHT=(screenHeight-Height)/2;
	GetClientRect(hWnd, &bounds); //窗口大小修正 
    int dx = Width - RectWidth(&bounds);
    int dy = Height - RectHeight(&bounds);
    SetWindowPos(hWnd, HWND_TOP, WIDTH, HEIGHT, Width + dx, Height + dy, 0);
    HDC hDC=GetDC(hWnd);
    HBITMAP osBits = CreateCompatibleBitmap(hDC,Width,Height);
    DeleteObject(SelectObject(hMemoryDC,osBits));
    ReleaseDC(hWnd,hDC);
}

static int x,y;
static HDC hDC;
static HDC hMemoryDC;
static PAINTSTRUCT ps;
static LPIMAGE IM_Welcome;
static bool fend=true;

static LRESULT CALLBACK WelcomeEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//欢迎窗口消息回调函数 
void WelcomeWindow(){
	if(!WELCOME){
		return;
	}
	int Width=476;
	int Height=268;
	x=Width,y=Height;
	HWND hwnd;
	WNDCLASSEX wc; 
	extern HINSTANCE hInstance;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = (HICON)LoadImage(NULL,FILE_ICON,IMAGE_ICON,0,0,LR_LOADFROMFILE);//程序图标 
    wc.hIconSm       = (HICON)LoadImage(NULL,FILE_TITLE_ICON,IMAGE_ICON,0,0,LR_LOADFROMFILE);//标题栏图标 
    wc.hInstance     = hInstance;
    wc.lpfnWndProc   = (WNDPROC)WelcomeEventProc; //窗口过程回调函数
    wc.lpszClassName = "WelcomeWindow";
    wc.lpszMenuName  = NULL;
	wc.style         = CS_HREDRAW | CS_VREDRAW;  
	RegisterClassEx(&wc);
    HWND desktop;
    RECT bounds;
	desktop = GetDesktopWindow();
    GetWindowRect(desktop, &bounds);
	int screenWidth = bounds.right - bounds.left;
	int screenHeight = bounds.bottom - bounds.top;
    int WIDTH=(screenWidth-Width)/2;
	int HEIGHT=(screenHeight-Height)/2;
	hwnd = CreateWindowEx(
	  WS_EX_TOPMOST,
      "WelcomeWindow",
      "Welcome", 
      WS_POPUP, 
      WIDTH,
      HEIGHT,
      Width, 
      Height,
      NULL, 
      NULL,
      hInstance,
      (LPSTR) NULL);
    if (hwnd == NULL) 
        printf("InitGraphics: CreateGraphicsWindow failed\n");
	hDC=GetDC(hwnd);
	hMemoryDC=CreateCompatibleDC(hDC);
	if (hMemoryDC == NULL)
        printf("Internal error: Can't create offscreen device");
    /*加载图片*/
	LPDATABASE BilliardsData;
	LoadDB("Billiards.db", &BilliardsData);
    LoadPictureFromDB(BilliardsData, 36 + 10 + 18 + 6 + 10 + 19 + 3 + 19 + 45 + 2 + 3, &IM_Welcome, NULL);
	FreeDB(&BilliardsData);
	SelectObject(hMemoryDC,IM_Welcome->hbitmap);
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
	SetTimer(hwnd,1,WELCOME_TIME,NULL);
	MSG messages;           
    while (fend&&GetMessage (&messages, NULL, 0, 0)){
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
	return;
}

static LRESULT CALLBACK WelcomeEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg)
    {
    	case WM_PAINT:// 绘图消息 
			hDC=BeginPaint(hwnd,&ps);
			BitBlt(hDC,0,0,x,y,hMemoryDC,0,0,SRCCOPY);
    		EndPaint(hwnd,&ps);
    		return 0;
    	case WM_TIMER:// 定时消息 
    		KillTimer(hwnd,1);
    		DeleteDC(hDC);
    		DeleteDC(hMemoryDC);
    		DestroyWindow(hwnd);
    		FreePicture(&IM_Welcome);
    		fend=false;
    		return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

#include <process.h>
static int Width=20,Height=20;
static LRESULT CALLBACK MusicEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//播放音频消息回调函数 
static HWND CreateMusicWindow(char *title){	
	WNDCLASSEX wc; 
	extern HINSTANCE hInstance;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor       = LoadCursorFromFile(FILE_CURSOR);
    wc.hIcon         = (HICON)LoadImage(NULL,FILE_ICON,IMAGE_ICON,0,0,LR_LOADFROMFILE);
    wc.hIconSm       = (HICON)LoadImage(NULL,FILE_TITLE_ICON,IMAGE_ICON,0,0,LR_LOADFROMFILE);
    wc.hInstance     = hInstance;
    wc.lpfnWndProc   = (WNDPROC)MusicEventProc;
    wc.lpszClassName = title;
    wc.lpszMenuName  = NULL;
	wc.style         = CS_HREDRAW | CS_VREDRAW;  
	RegisterClassEx(&wc);
    HWND desktop;
    RECT bounds;
	desktop = GetDesktopWindow();
    GetWindowRect(desktop, &bounds);
	int screenWidth = bounds.right - bounds.left;
	int screenHeight = bounds.bottom - bounds.top;
    int WIDTH=(screenWidth-Width)/2;
	int HEIGHT=(screenHeight-Height)/2;
	HWND hwnd = CreateWindowEx(
	  WS_EX_TOPMOST,
      title,
      title, 
      WS_POPUP, 
      WIDTH,
      HEIGHT,
      Width, 
      Height,
      NULL, 
      NULL,
      hInstance,
      (LPSTR) NULL);
    if (hwnd == NULL) {
        printf("InitGraphics: CreateGraphicsWindow failed\n");
    }
    printf("Window: %06X\n",hwnd);
    hDC=GetDC(hwnd);
    hMemoryDC=CreateCompatibleDC(hDC);
    HBITMAP osBits = CreateCompatibleBitmap(hDC,Width,Height);
	if (osBits == NULL)
        printf("Internal error: Can't create offscreen bitmap");
	SelectObject(hMemoryDC,osBits);
    extern LPIMAGE IM_Music;
	DrawPicture(IM_Music,0,0,20,20,hwnd,hMemoryDC);
    //ShowWindow(hwnd, SW_SHOWNORMAL);
	ShowWindow(hwnd, SW_HIDE);
    UpdateWindow(hwnd);    
    return hwnd;
}

extern LPMUSIC MU_Collide;
extern LPMUSIC MU_Reflect;

static void PlayCollideMusic()
{
	PlaySound(NULL, NULL, SND_FILENAME);
	PlaySound((LPCSTR)MU_Collide->data, NULL, SND_MEMORY | SND_ASYNC);
	return;
}

static void PlayReflectMusic()
{
	PlaySound(NULL, NULL, SND_FILENAME);
	PlaySound((LPCSTR)MU_Reflect->data, NULL, SND_MEMORY | SND_ASYNC);
	return;
}

#include <timesys.h>
/*游戏时间系统*/
extern LPTIMESYS GameTimeSys; 

#include <callback.h>

static int MU_PROCESS = 1;
void BackGroundMusic();

static LRESULT CALLBACK MusicEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg)
    {
    	case WM_PAINT:// 绘图消息 
			hDC=BeginPaint(hwnd,&ps);
			BitBlt(hDC,0,0,Width,Height,hMemoryDC,0,0,SRCCOPY);
	    	EndPaint(hwnd,&ps);
    		return 0;
    	case MM_MCINOTIFY:
    		MediaMessage(wParam,lParam);
    		return 0;
    	case WM_TIMER:// 定时消息
    		switch(wParam){
				case TM_BEGIN_MUSIC:
					KillTimer(hwnd,TM_BEGIN_MUSIC);
					/*随机背景音乐*/
    				BackGroundMusic();
    			break;
    			case TM_HIT_COUNTDOWN:
    				if(GameTimeSys->countdown_n>0){
						GameTimeSys->countdown_n--;
						GameTimeSys->UpdtStr();
						if(GameTimeSys->countdown_n<=5)
							PlayCtDnMusic();
					}else{
						EndGetHit();
						EndGetPower();
						EndFreeResetCueBall();
						StartHit();
					}
    			break;
    			case TM_CAST_CD:
    				KillTimer(hwnd,TM_CAST_CD);
    				DelCastCD();
    			break;
			}
    		return 0;
    	case WM_DESTROY:
    		printf("DestroyWindow: %X\n",hwnd);
            PostQuitMessage(0);
            return 0;
		case 0xFFFF://窗口通信保留字段 
    		{
    			char MSG[100];
    			sprintf(MSG,"hwnd:%X , msg:%X , wParam:%d , lParam:%d",hwnd,msg,wParam,lParam); 
    			if(hwnd == TimeSysWindow)
	    			MessageBox(TimeSysWindow,MSG,"TimeSysWindow",64);
				else if(hwnd == MenuMusicWindow)
	    			MessageBox(MenuMusicWindow,MSG,"MenuMusicWindow",64);
    			else if(hwnd == CollisionMusicWindow)
    				MessageBox(CollisionMusicWindow,MSG,"CollisionMusicWindow",64);
    			else if(hwnd == BackGroundMusicWindow)
    				MessageBox(BackGroundMusicWindow,MSG,"BackGroundMusicWindow",64);
			}
			return 0;   
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

static unsigned __stdcall CreateTimeSysWindow();
static unsigned __stdcall CreateMenuMusicWindow();
static unsigned __stdcall CreateCollisionMusicWindow();
static unsigned __stdcall CreateBackGroundMusicWindow();

static unsigned __stdcall CreateTimeSysWindow(){
	TimeSysWindow = CreateMusicWindow("TimeSysWindow");
	/*初始化计时变量*/
	InitTimeSys(&GameTimeSys);
	MU_PROCESS = 2;
	MSG messages;           
    while (GetMessage (&messages, NULL, 0, 0)){
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    _endthreadex(0);
}

static unsigned __stdcall CreateMenuMusicWindow(){
	MenuMusicWindow = CreateMusicWindow("MenuMusicWindow");
	MU_PROCESS = 3;
	MSG messages;           
    while (GetMessage (&messages, NULL, 0, 0)){
    	switch(messages.message){
    		case 0xFFFF://通信保留字段 
    		{
    			switch(messages.wParam){
					case TM_MUSIC:
	    				CastSound((LPMUSIC)messages.lParam);
					break;
				}
			}
    		break;
		}
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    _endthreadex(0);
}

static unsigned __stdcall CreateCollisionMusicWindow(){
	CollisionMusicWindow = CreateMusicWindow("CollisionMusicWindow");
	MU_PROCESS = 4;
	MSG messages;           
    while (GetMessage (&messages, NULL, 0, 0)){
    	switch(messages.message){
    		case 0xFFFF://通信保留字段 
    		{
    			switch(messages.wParam){
	    			case TM_MUSIC:
	    				CastSound((LPMUSIC)messages.lParam);
					break;
				}
			}
    		break;
		}
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    _endthreadex(0);
}

int BGMVolume = 1000;
static int BGMnum=0;

static int LoadBKmusic(char *filetype); //filetype:MP3 or WAV
static void PlayBackGroundMusic();
static void PauseBackGroundMusic();
static void BackGroundMusicVolume(int Volume);
static unsigned __stdcall CreateBackGroundMusicWindow(){
	BackGroundMusicWindow = CreateMusicWindow("BackGroundMusicWindow");
	/*加载音乐*/
	BGMnum=LoadBKmusic("MP3");
	BGMnum=LoadBKmusic("WAV");
	printf("%02d BGM Have Been Loaded\n",BGMnum);
	extern LPMUSIC MU_Begin;
	CastSound(MU_Begin);
	if(BGMnum){
		BackGroundMusicVolume(BGMVolume);
    	SetTimer(BackGroundMusicWindow,TM_BEGIN_MUSIC,13500,NULL);
	}
    MU_PROCESS = 0xFFFF;
    MSG messages;           
    while (GetMessage (&messages, NULL, 0, 0)){
    	switch(messages.message){
    		case 0xFFFF://通信保留字段 
    		{
    			switch(messages.wParam){
	    			case BGM_PAUSE:
	    				PauseBackGroundMusic();
					break;
					case BGM_PLAY:
						PlayBackGroundMusic();
					break;
					case BGM_VOLUME:
						BackGroundMusicVolume(BGMVolume);
					break;
				}
			}
    		break;
		}
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    _endthreadex(0);
}

void InitMusicWindow(){
	printf("InitMusicWindow\n");
	while(MU_PROCESS!=0xFFFF){ //保证音频资源按顺序加载 
		switch(MU_PROCESS){
			case 1:
				MU_PROCESS = 0;
				_beginthreadex(NULL,0,&CreateTimeSysWindow,NULL,0,&TimeSysID);
				printf("ID: %06d, ",TimeSysID); 
			break;
			case 2:
				MU_PROCESS = 0;
				_beginthreadex(NULL,0,&CreateMenuMusicWindow,NULL,0,&MenuMusicID);
				printf("ID: %06d, ",MenuMusicID); 
			break;
			case 3:
				MU_PROCESS = 0;
				_beginthreadex(NULL,0,&CreateCollisionMusicWindow,NULL,0,&CollisionMusicID);
				printf("ID: %06d, ",CollisionMusicID); 
			break;
			case 4:
				MU_PROCESS = 0;
				_beginthreadex(NULL,0,&CreateBackGroundMusicWindow,NULL,0,&BackGroundMusicID);
				printf("ID: %06d, ",BackGroundMusicID); 
			break;
		}
	}
	return;
}

void ExitMusicWindow(){
	PostMessage(TimeSysWindow,WM_DESTROY,0,0); 
	PostMessage(MenuMusicWindow,WM_DESTROY,0,0); 
	PostMessage(CollisionMusicWindow,WM_DESTROY,0,0); 
	PostMessage(BackGroundMusicWindow,WM_DESTROY,0,0);
	return;
}

#include <io.h>

static int LoadBKmusic(char *filetype) //filetype:MP3 or WAV
{   
	static int i=0;
	long handle; //用于查找的句柄
	struct _finddata_t fileinfo;  //文件信息的结构体
	char file[MAX_PATH],filename[MAX_PATH],alasname[10];
	sprintf(file,"Data/BGM/*.%s",filetype);      
    handle=_findfirst(file,&fileinfo);  //第一次查找
    if(handle==-1) return i;
    printf("[Loading %s]\n",filetype);
    /*加载音乐*/
    sprintf(alasname,"_BK%d",++i);
    sprintf(filename,"Data/BGM/%s",fileinfo.name);
    printf("[BGM%02d] %s\n",i,fileinfo.name);
    LoadMusic(filename,alasname);
    while(!_findnext(handle,&fileinfo)){ //循环查找其他符合的文件，知道找不到其他的为止
		sprintf(alasname,"_BK%d",++i);
    	sprintf(filename,"Data/BGM/%s",fileinfo.name);
    	printf("[BGM%02d] %s\n",i,fileinfo.name);
    	LoadMusic(filename,alasname); 
	}
    _findclose(handle); //关闭句柄
    return i;
}

#include <time.h>
#include <stdlib.h>

static int lastchoise=0;
static char alasname[10];

void BackGroundMusic(){
	int choise=lastchoise;
	srand((unsigned)time(NULL));
	if(lastchoise)
		while(choise==lastchoise)
			choise = rand() % BGMnum + 1;
	else{
		choise = rand() % BGMnum + 1;
	}
	lastchoise=choise;
	printf("BGMChoice: %d\n",choise);
	sprintf(alasname,"_BK%d",choise);
	SeekMedia(alasname,0);
	StartBackGroundMusicOutWithNotify(alasname);
} 

static void PauseBackGroundMusic(){
	printf("BGMPause: %d\n",lastchoise);
	sprintf(alasname,"_BK%d",lastchoise);
	PauseMedia(alasname);
}

static void PlayBackGroundMusic(){
	printf("BGMPlay: %d\n",lastchoise);
	sprintf(alasname,"_BK%d",lastchoise);
	PlayMedia(alasname);
}

static void BackGroundMusicVolume(int Volume){
	int i;
	for(i=1;i<=BGMnum;i++){
		sprintf(alasname,"_BK%d",i);
		SetVolume(alasname,Volume);
	}
}

#include <text.h>
extern LPFONTS FT_Title;
extern LPIMAGE IM_StartGame_0;
static HWND hExitWnd=NULL;
static HDC hExitDC,hExitMemoryDC;
static int ExitWidth=300,ExitHeight=150;
static enum{
    NO_BUTTON = 0,
    LEFT_BUTTON,
    MIDDLE_BUTTON,
    RIGHT_BUTTON
}Mouse_Button;
static enum {
    BUTTON_DOWN,
    BUTTON_DOUBLECLICK,
    BUTTON_UP,
    ROLL_UP,
    ROLL_DOWN,
    MOUSEMOVE	
}Mouse_Event;

static void DrawExitBtn();
static LRESULT CALLBACK ExitWndEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ExitWindow(){
	if(hExitWnd){
		ShowWindow(hExitWnd, SW_SHOWNORMAL);
		return;
	}
	char* ClassName="ExitWindow";
	char* windowTitle="退出提示";
	WNDCLASSEX wc; 
	extern HINSTANCE hInstance;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor       = CS_Cursor;
    wc.hIcon         = (HICON)LoadImage(NULL,FILE_ICON,IMAGE_ICON,0,0,LR_LOADFROMFILE);//程序图标 
    wc.hIconSm       = (HICON)LoadImage(NULL,FILE_TITLE_ICON,IMAGE_ICON,0,0,LR_LOADFROMFILE);//标题栏图标 
    wc.hInstance     = hInstance;
    wc.lpfnWndProc   = (WNDPROC)ExitWndEventProc; //窗口过程回调函数
    wc.lpszClassName = ClassName;
    wc.lpszMenuName  = NULL;
	wc.style         = CS_HREDRAW | CS_VREDRAW;  
	RegisterClassEx(&wc);
    HWND desktop;
    RECT bounds;
	desktop = GetDesktopWindow();
    GetWindowRect(desktop, &bounds);
	int screenWidth = bounds.right - bounds.left;
	int screenHeight = bounds.bottom - bounds.top;
    int WIDTH=(screenWidth-ExitWidth)/2;
	int HEIGHT=(screenHeight-ExitHeight)/2;
	hExitWnd = CreateWindowEx(
	  WS_EX_TOPMOST,
      ClassName,
      windowTitle, 
      WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME | WS_MINIMIZEBOX), 
      WIDTH,
      HEIGHT,
      ExitWidth, 
      ExitHeight,
      graphicsWindow, 
      NULL,
      hInstance,
      (LPSTR) NULL);
    if (hExitWnd == NULL) 
        printf("InitGraphics: CreateGraphicsWindow failed\n");
    GetClientRect(hExitWnd, &bounds); //窗口大小修正 
    int dx = ExitWidth - RectWidth(&bounds);
    int dy = ExitHeight - RectHeight(&bounds);
    WIDTH=(screenWidth-ExitWidth-dx)/2;
	HEIGHT=(screenHeight-ExitHeight-dy)/2;
    SetWindowPos(hExitWnd, HWND_TOP,
                 WIDTH,HEIGHT,
                 ExitWidth + dx,
                 ExitHeight + dy, 0);
    printf("Window: %06X\n",hExitWnd);
	hExitDC=GetDC(hExitWnd);
	hExitMemoryDC=CreateCompatibleDC(hExitDC);
	if (hExitMemoryDC == NULL)
        printf("Internal error: Can't create offscreen device\n");
	HBITMAP osBits = CreateCompatibleBitmap(hExitDC,ExitWidth,ExitHeight);
	if (osBits == NULL)
        printf("Internal error: Can't create offscreen bitmap");
	SelectObject(hExitMemoryDC,osBits);
	RECT r;
    SetRect(&r, 0, 0, ExitWidth, ExitHeight);
    InvalidateRect(hExitWnd, &r, TRUE);
    BitBlt(hExitMemoryDC, 0, 0, ExitWidth, ExitHeight, NULL, 0, 0, WHITENESS);
    EnableWindow(graphicsWindow,false);
    DrawExitBtn();
	ShowWindow(hExitWnd, SW_SHOWNORMAL);
    UpdateWindow(hExitWnd);
    PlayQuitMusic();
	return;
}

static int BT_yes=0,BT_no=0,BT_button=0,BT_update=0;
static int f_bt_yes=0,f_bt_no=0;

extern LPIMAGE IM_EX_BK;

static void DrawExitBtn(){
	DrawPicture(IM_EX_BK,0,0,300,200,hExitWnd,hExitMemoryDC);
	PutText(" 您确定退出求是台球吗？",0,0,300,80,0,0,0,25,FT_Title,hExitWnd,hExitMemoryDC);
	switch(BT_yes){
		case 0:
			if(f_bt_yes)
				f_bt_yes = !f_bt_yes;
			Draw32BitPicture(IM_StartGame_0,35,80,100,40,hExitWnd,hExitMemoryDC,48);
		break;
		case 1:
			if(!f_bt_yes){
				f_bt_yes = !f_bt_yes;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_StartGame_0,35,80,100,40,hExitWnd,hExitMemoryDC,80);
		break;
		case 2:
			Draw32BitPicture(IM_StartGame_0,35,80,100,40,hExitWnd,hExitMemoryDC,112);
		break;
	}
	switch(BT_no){
		case 0:
			if(f_bt_no)
				f_bt_no = !f_bt_no;
			Draw32BitPicture(IM_StartGame_0,165,80,100,40,hExitWnd,hExitMemoryDC,48);
		break;
		case 1:
			if(!f_bt_no){
				f_bt_no = !f_bt_no;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_StartGame_0,165,80,100,40,hExitWnd,hExitMemoryDC,80);
		break;
		case 2:
			Draw32BitPicture(IM_StartGame_0,165,80,100,40,hExitWnd,hExitMemoryDC,112);
		break;
	}
	PutText("是",35,80,135,120,0,0,0,25,FT_Title,hExitWnd,hExitMemoryDC);
	PutText("否",165,80,265,120,0,0,0,25,FT_Title,hExitWnd,hExitMemoryDC);
}

static void CloseMainWindow(){
	PostMessage(graphicsWindow,WM_CLOSE,0,0);
}

static bool enableflag = true;

static void MouseProc(int x, int y, int button, int event){
	if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
	if( (x>35 && x<135 || x>165 && x<265)
	 && (y>80 && y<120)){
		DestroyCursor(CS_Cursor);
		CS_Cursor = LoadCursor(NULL, IDC_HAND);
		SetCursor(CS_Cursor);
		BT_yes=0,BT_no=0,BT_button=0,BT_update=1;
		if(x>35 && x<135){
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_yes=1; else BT_yes=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
				SetCursor(CS_Cursor);
				PostMessage(hExitWnd,WM_CLOSE,0,0); 
				BT_yes=0;
				enableflag = false;
				HideCourseWindow(); 
				HideSettingWindow(); 
				PlayClosMusic();
				StartEndMasks(CloseMainWindow);
			}
		}
		else if(x>165 && x<265){
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_no=1; else BT_no=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
				SetCursor(CS_Cursor);
				PostMessage(hExitWnd,WM_CLOSE,0,0); 
				BT_no=0;
			}
		}
		DrawExitBtn();
	}else if(BT_update){
		DestroyCursor(CS_Cursor);
		CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
		SetCursor(CS_Cursor);
		BT_yes=0,BT_no=0,BT_button=0;
		DrawExitBtn();
		BT_update=0;
	}
}

static LRESULT CALLBACK ExitWndEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg)
    {
    	case WM_PAINT:// 绘图消息
			hExitDC=BeginPaint(hwnd,&ps);
			BitBlt(hExitDC,0,0,ExitWidth,ExitHeight,hExitMemoryDC,0,0,SRCCOPY);
    		EndPaint(hwnd,&ps);
    		return 0;
        case WM_CLOSE:// 窗口销毁消息
        	hExitWnd=NULL;
        	if(enableflag)
				EnableWindow(graphicsWindow,true);
			SetForegroundWindow(graphicsWindow);
			DestroyWindow(hwnd);
        	return 0;
        case WM_SETCURSOR:// 鼠标指针更新消息 
			SetCursor(CS_Cursor);
			return 0;
		case WM_SYSCOMMAND:
			switch(wParam){
				case HTCAPTION|SC_MOVE://不可拖动窗口 
					return 0;
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}
		
		/*鼠标消息*/
		case WM_LBUTTONDOWN:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), LEFT_BUTTON, BUTTON_DOWN);
    		return 0;
    
    	case WM_LBUTTONUP:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), LEFT_BUTTON, BUTTON_UP);
    		return 0;
    
    	case WM_LBUTTONDBLCLK:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), LEFT_BUTTON, BUTTON_DOUBLECLICK);
    		return 0;
    
    	case WM_MBUTTONDOWN:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), MIDDLE_BUTTON, BUTTON_DOWN);
    		return 0;
    
    	case WM_MBUTTONUP:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), MIDDLE_BUTTON, BUTTON_UP);
    		return 0;
    
    	case WM_MBUTTONDBLCLK:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), MIDDLE_BUTTON, BUTTON_DOUBLECLICK);
    		return 0;
    
    	case WM_RBUTTONDOWN:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), RIGHT_BUTTON, BUTTON_DOWN);
    		return 0;
    
    	case WM_RBUTTONUP:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), RIGHT_BUTTON, BUTTON_UP);
    		return 0;
    
    	case WM_RBUTTONDBLCLK:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), RIGHT_BUTTON, BUTTON_DOUBLECLICK);
    		return 0;
    
    	case WM_MOUSEMOVE:
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam), MOUSEMOVE, MOUSEMOVE);
    		return 0;
    
    	case WM_MOUSEWHEEL:
    		if(HIWORD(wParam) == 120)
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam),MIDDLE_BUTTON,ROLL_UP);
    		else if(HIWORD(wParam)==65416)
    			MouseProc((int) LOWORD(lParam), (int) HIWORD(lParam),MIDDLE_BUTTON,ROLL_DOWN);
    		return 0;
		
		case WM_KEYDOWN:
			if(wParam==VK_ESCAPE)
				PostMessage(hwnd,WM_CLOSE,0,0); 
		
        default:
            // pass给系统
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
