#include <windows.h>
#include <public.h>
#include <stdio.h>

#include <mmsystem.h>
#include <pictures.h>
#include <media.h>
#include <newwindow.h>

#include <text.h>

int ShowCourse;

extern HCURSOR CS_Cursor; 

extern LPFONTS FT_Title;
extern LPIMAGE IM_StartGame_0;

extern HWND graphicsWindow;

extern int BackGroundMusicID;
extern HWND BackGroundMusicWindow;

static int CourseID;
static HWND hCourseWnd=NULL;

static HDC hCourseDC,hCourseMemoryDC;

static int CourseWidth=960,CourseHeight=540;

static PAINTSTRUCT ps;

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

enum{
	COURSE_EXIT,
};

static int coursepage = 0;
static int stopflag = 0;

static void DrawCourseBtn();
static void PaintAll();
static LRESULT CALLBACK CourseWndEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static unsigned __stdcall CourseWindow(){
	if(hCourseWnd){
		FLASHWINFO info;
		info.cbSize = sizeof(info);
		info.hwnd = hCourseWnd;
		info.dwFlags = FLASHW_TRAY;		//FLASHW_ALL标题和任务栏、FLASHW_CAPTION标题、FLASHW_STOP停止、FLASHW_TIMER不停、FLASHW_TIMERNOFG直到前端显示、FLASHW_TRAY任务栏
		info.uCount = 2; 				//闪烁窗口的次数
		info.dwTimeout = 0; 			//窗口闪烁的频度，毫秒为单位；若该值为0，则为默认图标的闪烁频度
		FlashWindowEx(&info);
		ShowWindowAsync(hCourseWnd, SW_RESTORE);
		SetForegroundWindow(hCourseWnd);
		ShowWindow(hCourseWnd, SW_SHOWNORMAL);
		return;
	}
	char* ClassName="CourseWindow";
	char* windowTitle="游戏教程";
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
    wc.lpfnWndProc   = (WNDPROC)CourseWndEventProc; //窗口过程回调函数
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
    int WIDTH=(screenWidth-CourseWidth)/2;
	int HEIGHT=(screenHeight-CourseHeight)/2;
	hCourseWnd = CreateWindowEx(
	  WS_EX_TOPMOST,
      ClassName,
      windowTitle, 
      WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME | WS_MINIMIZEBOX ), 
      WIDTH,
      HEIGHT,
      CourseWidth, 
      CourseHeight,
      graphicsWindow, 
      NULL,
      hInstance,
      (LPSTR) NULL);
    if (hCourseWnd == NULL) {
    	printf("InitGraphics: CreateGraphicsWindow failed\n");
    	_endthreadex(0);
    	return;
	}
    GetClientRect(hCourseWnd, &bounds); //窗口大小修正 
    int dx = CourseWidth - RectWidth(&bounds);
    int dy = CourseHeight - RectHeight(&bounds);
    WIDTH=(screenWidth-CourseWidth-dx)/2;
	HEIGHT=(screenHeight-CourseHeight-dy)/2;
    SetWindowPos(hCourseWnd, HWND_TOP,
                 WIDTH,HEIGHT,
                 CourseWidth + dx,
                 CourseHeight + dy, 0);
    printf("Window: %06X\n",hCourseWnd);
	hCourseDC=GetDC(hCourseWnd);
	hCourseMemoryDC=CreateCompatibleDC(hCourseDC);
	if (hCourseMemoryDC == NULL)
        printf("Internal error: Can't create offscreen device\n");
	HBITMAP osBits = CreateCompatibleBitmap(hCourseDC,CourseWidth,CourseHeight);
	if (osBits == NULL)
        printf("Internal error: Can't create offscreen bitmap");
	SelectObject(hCourseMemoryDC,osBits);
	RECT r;
    SetRect(&r, 0, 0, CourseWidth, CourseHeight);
    InvalidateRect(hCourseWnd, &r, TRUE);
    BitBlt(hCourseMemoryDC, 0, 0, CourseWidth, CourseHeight, NULL, 0, 0, WHITENESS);
    EnableWindow(graphicsWindow,false);
	DrawCourseBtn();
	/*初始化*/
	coursepage = 0;
	stopflag = 0;
	PostThreadMessage(BackGroundMusicID, 0xFFFF, BGM_PAUSE, 0); 
	/*显示*/
	ShowWindow(hCourseWnd, SW_SHOWNORMAL);
    UpdateWindow(hCourseWnd);
    DrawCourseBtn();
	PaintAll();
	LoadVedio("Data/Video/Basic.dat","V",220,105,705,396,hCourseWnd);
	StartMediaWithNotify("V",hCourseWnd);
	return;
}


static int BT_no=0,BT_page0=0,BT_page1=0,BT_page2=0,BT_page3=0,BT_play=0,BT_left=0,BT_right=0,BT_button=0,BT_update=0;
static int f_bt_no=0,f_bt_p0=0,f_bt_p1=0,f_bt_p2=0,f_bt_p3=0,f_bt_play=0,f_bt_left=0,f_bt_right=0;

extern LPIMAGE IM_CourseBK0,
			   IM_CourseBK1,
			   IM_CourseBK2,
			   IM_CourseBK3,
			   IM_GM_Esc_0,
			   IM_CoursePlay,
			   IM_CoursePause,
			   IM_CourseRight,
			   IM_CourseLeft;

static void DrawCourseBtn(){
	switch(coursepage){
		case 0:
			DrawPicture(IM_CourseBK0,0,0,960,540,hCourseWnd,hCourseMemoryDC);
			PutTextB("基础玩法",225,10,460,80,255,255,255,70,FT_Title,hCourseWnd,hCourseMemoryDC);
		break;
		case 1:
			DrawPicture(IM_CourseBK1,0,0,960,540,hCourseWnd,hCourseMemoryDC);
			PutTextB("经典模式",225,10,460,80,255,255,255,70,FT_Title,hCourseWnd,hCourseMemoryDC);
		break;
		case 2:
			DrawPicture(IM_CourseBK2,0,0,960,540,hCourseWnd,hCourseMemoryDC);
			PutTextB("美式台球",225,10,460,80,255,255,255,70,FT_Title,hCourseWnd,hCourseMemoryDC);
		break;
		case 3:
			DrawPicture(IM_CourseBK3,0,0,960,540,hCourseWnd,hCourseMemoryDC);
			PutTextB("斯诺克",225,10,460,80,255,255,255,70,FT_Title,hCourseWnd,hCourseMemoryDC);
		break;
	}
	switch(BT_play){
		case 0:
			if(f_bt_play)
				f_bt_play = !f_bt_play;
			if(stopflag)
				Draw32BitPicture(IM_CoursePlay,678,30,40,40,hCourseWnd,hCourseMemoryDC,192);
			else
				Draw32BitPicture(IM_CoursePause,678,30,40,40,hCourseWnd,hCourseMemoryDC,192);
		break;
		case 1:
			if(!f_bt_play){
				f_bt_play = !f_bt_play;
				PlayOnBtMusic();
			}
			if(stopflag)
				Draw32BitPicture(IM_CoursePlay,678,30,40,40,hCourseWnd,hCourseMemoryDC,225);
			else
				Draw32BitPicture(IM_CoursePause,678,30,40,40,hCourseWnd,hCourseMemoryDC,225);
		break;
		case 2:
			if(stopflag)
				Draw32BitPicture(IM_CoursePlay,678,30,40,40,hCourseWnd,hCourseMemoryDC,255);
			else
				Draw32BitPicture(IM_CoursePause,678,30,40,40,hCourseWnd,hCourseMemoryDC,255);
		break;
	}
	switch(BT_left){
		case 0:
			if(f_bt_left)
				f_bt_left = !f_bt_left;
			Draw32BitPicture(IM_CourseLeft,605,30,40,40,hCourseWnd,hCourseMemoryDC,192);
		break;
		case 1:
			if(!f_bt_left){
				f_bt_left = !f_bt_left;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_CourseLeft,605,30,40,40,hCourseWnd,hCourseMemoryDC,225);
		break;
		case 2:
			Draw32BitPicture(IM_CourseLeft,605,30,40,40,hCourseWnd,hCourseMemoryDC,255);
		break;
	}
	switch(BT_right){
		case 0:
			if(f_bt_right)
				f_bt_right = !f_bt_right;
			Draw32BitPicture(IM_CourseRight,750,30,40,40,hCourseWnd,hCourseMemoryDC,192);
		break;
		case 1:
			if(!f_bt_right){
				f_bt_right = !f_bt_right;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_CourseRight,750,30,40,40,hCourseWnd,hCourseMemoryDC,225);
		break;
		case 2:
			Draw32BitPicture(IM_CourseRight,750,30,40,40,hCourseWnd,hCourseMemoryDC,255);
		break;
	}
	switch(BT_page0){
		case 0:
			if(f_bt_p0)
				f_bt_p0 = !f_bt_p0;
		break;
		case 1:
			if(!f_bt_p0){
				f_bt_p0 = !f_bt_p0;
				PlayOnBtMusic();
			}
		break;
		case 2:
			;
		break;
	}
	switch(BT_page1){
		case 0:
			if(f_bt_p1)
				f_bt_p1 = !f_bt_p1;
		break;
		case 1:
			if(!f_bt_p1){
				f_bt_p1 = !f_bt_p1;
				PlayOnBtMusic();
			}
		break;
		case 2:
			;
		break;
	}
	switch(BT_page2){
		case 0:
			if(f_bt_p2)
				f_bt_p2 = !f_bt_p2;
		break;
		case 1:
			if(!f_bt_p2){
				f_bt_p2 = !f_bt_p2;
				PlayOnBtMusic();
			}
		break;
		case 2:
			;
		break;
	}
	switch(BT_page3){
		case 0:
			if(f_bt_p3)
				f_bt_p3 = !f_bt_p3;
		break;
		case 1:
			if(!f_bt_p3){
				f_bt_p3 = !f_bt_p3;
				PlayOnBtMusic();
			}
		break;
		case 2:
			;
		break;
	}
	switch(BT_no){
		case 0:
			if(f_bt_no)
				f_bt_no = !f_bt_no;
			Draw32BitPicture(IM_GM_Esc_0,33,25,151,34,hCourseWnd,hCourseMemoryDC,192);
		break;
		case 1:
			if(!f_bt_no){
				f_bt_no = !f_bt_no;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_GM_Esc_0,33,25,151,34,hCourseWnd,hCourseMemoryDC,225);
		break;
		case 2:
			Draw32BitPicture(IM_GM_Esc_0,33,25,151,34,hCourseWnd,hCourseMemoryDC,255);
		break;
	}
	PutText("关闭教程",33,25,150,59,255,255,255,25,FT_Title,hCourseWnd,hCourseMemoryDC);
}

static bool enableflag = true;

static void MouseProc(int x, int y, int button, int event){
	if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
	if( (x>33 && x<184 && y>25 && y<57)
	 || (x>33 && x<182) && ((y>102 && y<184)&&coursepage!=0
	 					||  (y>209 && y<291)&&coursepage!=1
						||  (y>315 && y<397)&&coursepage!=2
						||  (y>422 && y<504)&&coursepage!=3)
	 || (y>30 && y<70)  && ((x>678 && x<718)
	 					||  (x>605 && x<645)
						||  (x>750 && x<790))){
		DestroyCursor(CS_Cursor);
		CS_Cursor = LoadCursor(NULL, IDC_HAND);
		SetCursor(CS_Cursor);
		BT_no=0,BT_page0=0,BT_page1=0,BT_page2=0,BT_page3=0,BT_play=0,BT_left=0,BT_right=0,BT_button=0,BT_update=1;
		if((x>33 && x<182) && ((y>102 && y<184)
	 					   ||  (y>209 && y<291)
						   ||  (y>315 && y<397)
						   ||  (y>422 && y<504))){//page 
			if(y>102 && y<184){
				if(button == LEFT_BUTTON && event == BUTTON_DOWN)
					BT_button=1;
				if(!BT_button) BT_page0=1; else BT_page0=2;
				if(button == LEFT_BUTTON && event == BUTTON_UP){
					ResetCursors();
					BT_page0=0;
					coursepage=0;
					DrawCourseBtn();
					PaintAll();
					CloseMedia("V");
					LoadVedio("Data/Video/Basic.dat","V",220,105,705,397,hCourseWnd);
					StartMediaWithNotify("V",hCourseWnd);
				}
			}
			else if(y>209 && y<291){
				if(button == LEFT_BUTTON && event == BUTTON_DOWN)
					BT_button=1;
				if(!BT_button) BT_page1=1; else BT_page1=2;
				if(button == LEFT_BUTTON && event == BUTTON_UP){
					ResetCursors();
					BT_page1=0;
					coursepage=1;
					DrawCourseBtn();
					PaintAll();
					CloseMedia("V");
					LoadVedio("Data/Video/Classic.dat","V",220,105,705,397,hCourseWnd);
					StartMediaWithNotify("V",hCourseWnd);
				}
			}
			else if(y>315 && y<397){
				if(button == LEFT_BUTTON && event == BUTTON_DOWN)
					BT_button=1;
				if(!BT_button) BT_page2=1; else BT_page2=2;
				if(button == LEFT_BUTTON && event == BUTTON_UP){
					ResetCursors();
					BT_page2=0;
					coursepage=2;
					DrawCourseBtn();
					PaintAll();
					CloseMedia("V");
					LoadVedio("Data/Video/Pool.dat","V",220,105,705,397,hCourseWnd);
					StartMediaWithNotify("V",hCourseWnd);
				}
			}
			else if(y>422 && y<504){
				if(button == LEFT_BUTTON && event == BUTTON_DOWN)
					BT_button=1;
				if(!BT_button) BT_page3=1; else BT_page3=2;
				if(button == LEFT_BUTTON && event == BUTTON_UP){
					ResetCursors();
					BT_page3=0;
					coursepage=3;
					DrawCourseBtn();
					PaintAll();
					CloseMedia("V");
					LoadVedio("Data/Video/Snooker.dat","V",220,105,705,397,hCourseWnd);
					StartMediaWithNotify("V",hCourseWnd);
				}
			}
		}
		if((y>30 && y<70)  && ((x>678 && x<718)
	 					   ||  (x>605 && x<645)
						   ||  (x>750 && x<790))){//control
			if(x>678 && x<718){
				if(button == LEFT_BUTTON && event == BUTTON_DOWN)
					BT_button=1;
				if(!BT_button) BT_play=1; else BT_play=2;
				if(button == LEFT_BUTTON && event == BUTTON_UP){
					BT_play=0;
					stopflag=!stopflag;
					if(stopflag)
						PauseMedia("V");
					else
						StartMediaWithNotify("V",hCourseWnd);
				}
			}
			else if(x>605 && x<645){
				if(button == LEFT_BUTTON && event == BUTTON_DOWN)
					BT_button=1;
				if(!BT_button) BT_left=1; else BT_left=2;
				if(button == LEFT_BUTTON && event == BUTTON_UP){
					BT_left=0;
					long pos=GetPosition("V");
					PlayMediaFromWithNotify("V",pos>5000?pos-5000:0,hCourseWnd);
					if(stopflag)
						stopflag=!stopflag;
				}
			}
			else if(x>750 && x<790){
				if(button == LEFT_BUTTON && event == BUTTON_DOWN)
					BT_button=1;
				if(!BT_button) BT_right=1; else BT_right=2;
				if(button == LEFT_BUTTON && event == BUTTON_UP){
					BT_right=0;
					long pos=GetPosition("V");
					long len=GetLength("V");
					if(pos<len-5000){
						PlayMediaFromWithNotify("V",pos+5000,hCourseWnd);
						if(stopflag)
							stopflag=!stopflag;
					}
					else{
						PauseMedia("V");
						if(!stopflag)
							stopflag=!stopflag;
					}	
				}
			}
		}
		else if(x>33 && x<184 && y>25 && y<57){//back
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_no=1; else BT_no=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				DestroyCursor(CS_Cursor);
				CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
				SetCursor(CS_Cursor);
				PostMessage(hCourseWnd,WM_CLOSE,0,0); 
				BT_no=0;
			}
		}
		DrawCourseBtn();
	}else if(BT_update){
		DestroyCursor(CS_Cursor);
		CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
		SetCursor(CS_Cursor);
		BT_no=0,BT_page0=0,BT_page1=0,BT_page2=0,BT_page3=0,BT_play=0,BT_left=0,BT_right=0,BT_button=0;
		DrawCourseBtn();
		BT_update=0;
	}
}

static void PaintAll(){
	DrawCourseBtn();
	hCourseDC=BeginPaint(hCourseWnd,&ps);
	BitBlt(hCourseDC,0,0,CourseWidth,CourseHeight,hCourseMemoryDC,0,0,SRCCOPY);
	EndPaint(hCourseWnd,&ps);
}

static LRESULT CALLBACK CourseWndEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg)
    {
    	case WM_PAINT:// 绘图消息
			hCourseDC=BeginPaint(hCourseWnd,&ps);
			BitBlt(hCourseDC,0,0,220,CourseHeight,hCourseMemoryDC,0,0,SRCCOPY);
			BitBlt(hCourseDC,0,0,CourseWidth,105,hCourseMemoryDC,0,0,SRCCOPY);
			BitBlt(hCourseDC,925,105,35,435,hCourseMemoryDC,925,105,SRCCOPY);
			BitBlt(hCourseDC,220,502,740,38,hCourseMemoryDC,220,502,SRCCOPY);
			EndPaint(hCourseWnd,&ps);
    		return 0;
        case WM_CLOSE:// 窗口销毁消息
        	hCourseWnd=NULL;
        	PostThreadMessage(BackGroundMusicID, 0xFFFF, BGM_PLAY, 0); 
        	CloseMedia("V");
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
		case MM_MCINOTIFY:// 响应MCI播放命令 
    		switch(wParam){
				case MCI_NOTIFY_SUCCESSFUL:
					{
						printf("Video End!\n");
						printf("%ld\n",lParam);
						stopflag=!stopflag;
						DrawCourseBtn();
					}
				return;
			}
    		return 0;
		
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

void ShowCourseWindow(){
	CourseWindow();
	return;
}

void HideCourseWindow(){
	if(hCourseWnd)
		PostMessage(hCourseWnd,WM_CLOSE,0,0); 
	return;
}
