#include <windows.h>
#include <public.h>
#include <stdio.h>

#include <mmsystem.h>
#include <pictures.h>
#include <media.h>
#include <newwindow.h>

#include <text.h>
#include <gui.h>

int ShowCourse;

extern HCURSOR CS_Cursor; 

extern LPFONTS FT_Title;
extern LPIMAGE IM_StartGame_0;

extern HWND graphicsWindow;

extern int BackGroundMusicID;
extern HWND BackGroundMusicWindow;

static int SettingID;
static HWND hSettingWnd=NULL;

static HDC hSettingDC,hSettingMemoryDC;

static int SettingWidth=640,SettingHeight=360;

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

extern int BGMVolume;
extern int RefreshRate;
extern float ForceBarSpeed;

static int dot1,dot2,dot3;

static int ApplySet();

static void DrawSettingBtn();
static int SetRefreshRate(int dot);
static int GetRefreshRate(int rate);
static LRESULT CALLBACK SettingWndEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static unsigned __stdcall SettingWindow(){
	if(hSettingWnd){
		FLASHWINFO info;
		info.cbSize = sizeof(info);
		info.hwnd = hSettingWnd;
		info.dwFlags = FLASHW_TRAY;		//FLASHW_ALL标题和任务栏、FLASHW_CAPTION标题、FLASHW_STOP停止、FLASHW_TIMER不停、FLASHW_TIMERNOFG直到前端显示、FLASHW_TRAY任务栏
		info.uCount = 2; 				//闪烁窗口的次数
		info.dwTimeout = 0; 			//窗口闪烁的频度，毫秒为单位；若该值为0，则为默认图标的闪烁频度
		FlashWindowEx(&info);
		ShowWindowAsync(hSettingWnd, SW_RESTORE);
		SetForegroundWindow(hSettingWnd);
		ShowWindow(hSettingWnd, SW_SHOWNORMAL);
		return;
	}
	char* ClassName="SettingWindow";
	char* windowTitle="游戏设置";
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
    wc.lpfnWndProc   = (WNDPROC)SettingWndEventProc; //窗口过程回调函数
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
    int WIDTH=(screenWidth-SettingWidth)/2;
	int HEIGHT=(screenHeight-SettingHeight)/2;
	hSettingWnd = CreateWindowEx(
	  WS_EX_TOPMOST,
      ClassName,
      windowTitle, 
      WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME | WS_MINIMIZEBOX ), 
      WIDTH,
      HEIGHT,
      SettingWidth, 
      SettingHeight,
      graphicsWindow, 
      NULL,
      hInstance,
      (LPSTR) NULL);
    if (hSettingWnd == NULL) {
    	printf("InitGraphics: CreateGraphicsWindow failed\n");
    	_endthreadex(0);
    	return;
	}
    GetClientRect(hSettingWnd, &bounds); //窗口大小修正 
    int dx = SettingWidth - RectWidth(&bounds);
    int dy = SettingHeight - RectHeight(&bounds);
    WIDTH=(screenWidth-SettingWidth-dx)/2;
	HEIGHT=(screenHeight-SettingHeight-dy)/2;
    SetWindowPos(hSettingWnd, HWND_TOP,
                 WIDTH,HEIGHT,
                 SettingWidth + dx,
                 SettingHeight + dy, 0);
    printf("Window: %06X\n",hSettingWnd);
	hSettingDC=GetDC(hSettingWnd);
	hSettingMemoryDC=CreateCompatibleDC(hSettingDC);
	if (hSettingMemoryDC == NULL)
        printf("Internal error: Can't create offscreen device\n");
	HBITMAP osBits = CreateCompatibleBitmap(hSettingDC,SettingWidth,SettingHeight);
	if (osBits == NULL)
        printf("Internal error: Can't create offscreen bitmap");
	SelectObject(hSettingMemoryDC,osBits);
	RECT r;
    SetRect(&r, 0, 0, SettingWidth, SettingHeight);
    InvalidateRect(hSettingWnd, &r, TRUE);
    BitBlt(hSettingMemoryDC, 0, 0, SettingWidth, SettingHeight, NULL, 0, 0, WHITENESS);
    EnableWindow(graphicsWindow,false);
	/*初始化*/
	dot1=GetRefreshRate(RefreshRate);
	dot2=(int)(320*BGMVolume/1000.0);
	dot3=(int)(320*ForceBarSpeed/100.0);
	/*显示*/
	DrawSettingBtn();
	ShowWindow(hSettingWnd, SW_SHOWNORMAL);
    UpdateWindow(hSettingWnd);
	return;
}

static int BT_no=0,BT_apply=0,BT_dot1=0,BT_dot2=0,BT_dot3=0,BT_button=0,BT_update=0;
static int f_bt_no=0,f_bt_apply=0,f_bt_dot1=0,f_bt_dot2=0,f_bt_dot3=0;

extern LPIMAGE IM_SettingBK,
			   IM_GM_Esc_0,
			   IM_SettingEnter,
			   IM_SeekBar,
			   IM_SeekBarCover,
			   IM_SeekBarDot;
			   
static char temptext[10];

static void DrawSettingBtn(){
	DrawPicture(IM_SettingBK,0,0,640,360,hSettingWnd,hSettingMemoryDC);
	PutText("游戏设置",225,20,415,65,255,255,255,45,FT_Title,hSettingWnd,hSettingMemoryDC);
	DrawPicture(IM_SeekBar,225,108,320,3,hSettingWnd,hSettingMemoryDC);
	DrawPicture(IM_SeekBar,225,165,320,3,hSettingWnd,hSettingMemoryDC);
	DrawPicture(IM_SeekBar,225,220,320,3,hSettingWnd,hSettingMemoryDC);
	DrawAreaPicture(IM_SeekBarCover,225,108,320,3,0,0,dot1,3,hSettingWnd,hSettingMemoryDC);
	DrawAreaPicture(IM_SeekBarCover,225,165,320,3,0,0,dot2,3,hSettingWnd,hSettingMemoryDC);
	DrawAreaPicture(IM_SeekBarCover,225,220,320,3,0,0,dot3,3,hSettingWnd,hSettingMemoryDC);
	Draw32BitPicture(IM_SeekBarDot,225+dot1-6,108-5,13,13,hSettingWnd,hSettingMemoryDC,0xFF);
	Draw32BitPicture(IM_SeekBarDot,225+dot2-6,165-5,13,13,hSettingWnd,hSettingMemoryDC,0xFF);
	Draw32BitPicture(IM_SeekBarDot,225+dot3-6,220-5,13,13,hSettingWnd,hSettingMemoryDC,0xFF);
	sprintf(temptext,"%d",(int)(27*dot1/32.0)+30);
	PutTextC(temptext,500,81,545,95,255,255,255,20,FT_Title,hSettingWnd,hSettingMemoryDC);
	sprintf(temptext,"%d",(int)(100*dot2/320.0));
	PutTextC(temptext,500,138,545,152,255,255,255,20,FT_Title,hSettingWnd,hSettingMemoryDC);
	sprintf(temptext,"%d",(int)(100*dot3/320.0));
	PutTextC(temptext,500,194,545,208,255,255,255,20,FT_Title,hSettingWnd,hSettingMemoryDC);
	switch(BT_dot1){
		case 0:
			if(f_bt_dot1)
				f_bt_dot1 = !f_bt_dot1;
		break;
		case 1:
			if(!f_bt_dot1){
				f_bt_dot1 = !f_bt_dot1;
				PlayOnBtMusic();
			}
		break;
		case 2:
			;
		break;
	}
	switch(BT_dot2){
		case 0:
			if(f_bt_dot2)
				f_bt_dot2 = !f_bt_dot2;
		break;
		case 1:
			if(!f_bt_dot2){
				f_bt_dot2 = !f_bt_dot2;
				PlayOnBtMusic();
			}
		break;
		case 2:
			;
		break;
	}
	switch(BT_dot3){
		case 0:
			if(f_bt_dot3)
				f_bt_dot3 = !f_bt_dot3;
		break;
		case 1:
			if(!f_bt_dot3){
				f_bt_dot3 = !f_bt_dot3;
				PlayOnBtMusic();
			}
		break;
		case 2:
			;
		break;
	}
	PutText("屏幕刷新率",90,95,205,125,255,255,255,25,FT_Title,hSettingWnd,hSettingMemoryDC);
	PutText("游戏内音量",90,150,205,180,255,255,255,25,FT_Title,hSettingWnd,hSettingMemoryDC);
	PutText("力度条移速",90,205,205,235,255,255,255,25,FT_Title,hSettingWnd,hSettingMemoryDC);
	switch(BT_no){
		case 0:
			if(f_bt_no)
				f_bt_no = !f_bt_no;
			Draw32BitPicture(IM_GM_Esc_0,157,285,132,30,hSettingWnd,hSettingMemoryDC,192);
		break;
		case 1:
			if(!f_bt_no){
				f_bt_no = !f_bt_no;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_GM_Esc_0,157,285,132,30,hSettingWnd,hSettingMemoryDC,225);
		break;
		case 2:
			Draw32BitPicture(IM_GM_Esc_0,157,285,132,30,hSettingWnd,hSettingMemoryDC,255);
		break;
	}
	switch(BT_apply){
		case 0:
			if(f_bt_apply)
				f_bt_apply = !f_bt_apply;
			Draw32BitPicture(IM_SettingEnter,364,285,132,30,hSettingWnd,hSettingMemoryDC,192);
		break;
		case 1:
			if(!f_bt_apply){
				f_bt_apply = !f_bt_apply;
				PlayOnBtMusic();
			}
			Draw32BitPicture(IM_SettingEnter,364,285,132,30,hSettingWnd,hSettingMemoryDC,225);
		break;
		case 2:
			Draw32BitPicture(IM_SettingEnter,364,285,132,30,hSettingWnd,hSettingMemoryDC,255);
		break;
	}
	PutText("退出设置",157,285,257,315,255,255,255,23,FT_Title,hSettingWnd,hSettingMemoryDC);
	PutText("应用设置",364,285,464,315,255,255,255,23,FT_Title,hSettingWnd,hSettingMemoryDC);
}

static bool enableflag = true;
static bool onseekbar1 = false;
static bool onseekbar2 = false;
static bool onseekbar3 = false;

static void MouseProc(int x, int y, int button, int event){
	if(button == LEFT_BUTTON && event == BUTTON_UP) BT_button=0;
	if(onseekbar1){
		if(event == MOUSEMOVE){
			dot1 = x - 225;
			if(dot1 < 18) dot1 = 0;
			else if(dot1 < 45) dot1 = 36;
			else if(dot1 < 80) dot1 = 54;
			else if(dot1 < 121) dot1 = 107;
			else if(dot1 < 148) dot1 = 136;
			else if(dot1 < 204) dot1 = 160;
			else if(dot1 < 284) dot1 = 249;
			else dot1 = 320;
			DrawSettingBtn();
		}else if(button == LEFT_BUTTON && event == BUTTON_UP){
			onseekbar1=false;
			if((y>103 && y<116) && (x>225+dot1-6 && x<225+dot1+7));
			else ResetCursors();
		}
	}else if(onseekbar2){
		if(event == MOUSEMOVE){
			dot2 = x - 225;
			if(dot2 < 0) dot2 = 0;
			else if(dot2 > 320) dot2 = 320;
			DrawSettingBtn();
		}else if(button == LEFT_BUTTON && event == BUTTON_UP){
			onseekbar2=false;
			if((y>160 && y<173) && (x>225+dot2-6 && x<225+dot2+7));
			else ResetCursors();
		}
	}else if(onseekbar3){
		if(event == MOUSEMOVE){
			dot3 = x - 225;
			if(dot3 < 0) dot3 = 0;
			else if(dot3 > 320) dot3 = 320;
			DrawSettingBtn();
		}else if(button == LEFT_BUTTON && event == BUTTON_UP){
			onseekbar3=false;
			if((y>215 && y<228) && (x>225+dot3-6 && x<225+dot3+7));
			else ResetCursors();
		}
	}
	else if((y>285 && y<315) && ((x>157 && x<289) 
							 ||  (x>364 && x<496))
		 || (y>103 && y<116) && (x>225+dot1-6 && x<225+dot1+7)
		 || (y>160 && y<173) && (x>225+dot2-6 && x<225+dot2+7)
		 || (y>215 && y<228) && (x>225+dot3-6 && x<225+dot3+7)){
		DestroyCursor(CS_Cursor);
		CS_Cursor = LoadCursor(NULL, IDC_HAND);
		SetCursor(CS_Cursor);
		BT_no=0,BT_apply=0,BT_dot1=0,BT_dot2=0,BT_dot3=0,BT_button=0,BT_update=1;
		if((y>103 && y<116) && (x>225+dot1-6 && x<225+dot1+7)){//dot1
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_dot1=1; else{
				BT_dot1=2;
				onseekbar1=true;
			} 
		}
		else if((y>160 && y<173) && (x>225+dot2-6 && x<225+dot2+7)){//dot2
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_dot2=1; else{
				BT_dot2=2;
				onseekbar2=true;
			} 
		}
		else if((y>215 && y<228) && (x>225+dot3-6 && x<225+dot3+7)){//dot3
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_dot3=1; else{
				BT_dot3=2;
				onseekbar3=true;
			} 
		}
		else if(x>157 && x<289 && y>285 && y<315){//back
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_no=1; else BT_no=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				BT_no=0;
				ResetCursors();
				PostMessage(hSettingWnd,WM_CLOSE,0,0); 
			}
		}
		else if(x>364 && x<496 && y>285 && y<315){//apply
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_apply=1; else BT_apply=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				BT_apply=0;
				ApplySet();
				PlaySaveMusic();
			}
		}
		DrawSettingBtn();
	}
	else if((x>225 && x<545) && ((y>103 && y<116) 
							 ||  (y>160 && y<173) 
							 ||  (y>215 && y<228))){
		DestroyCursor(CS_Cursor);
		CS_Cursor = LoadCursor(NULL, IDC_HAND);
		SetCursor(CS_Cursor);
		BT_dot1=0,BT_dot2=0,BT_dot3=0,BT_button=0,BT_update=1;
		if(y>103 && y<116){//bar1
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_dot1=1; else BT_dot1=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				BT_dot1=0;
				dot1 = x - 225;
				if(dot1 < 18) dot1 = 0;
				else if(dot1 < 45) dot1 = 36;
				else if(dot1 < 80) dot1 = 54;
				else if(dot1 < 121) dot1 = 107;
				else if(dot1 < 148) dot1 = 136;
				else if(dot1 < 204) dot1 = 160;
				else if(dot1 < 284) dot1 = 249;
				else dot1 = 320;
			}
		}
		else if(y>160 && y<173){//bar2
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_dot2=1; else BT_dot2=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				BT_dot2=0;
				dot2 = x - 225;
			}
		}
		else if(y>215 && y<228){//bar3
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				BT_button=1;
			if(!BT_button) BT_dot3=1; else BT_dot3=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				BT_dot3=0;
				dot3 = x - 225;
			}
		}
		DrawSettingBtn();
	}else if(BT_update){
		DestroyCursor(CS_Cursor);
		CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
		SetCursor(CS_Cursor);
		BT_no=0,BT_apply=0,BT_dot1=0,BT_dot2=0,BT_dot3=0,BT_button=0;
		DrawSettingBtn();
		BT_update=0;
	}
}

static LRESULT CALLBACK SettingWndEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg)
    {
    	case WM_PAINT:// 绘图消息
			hSettingDC=BeginPaint(hSettingWnd,&ps);
			BitBlt(hSettingDC,0,0,SettingWidth,SettingHeight,hSettingMemoryDC,0,0,SRCCOPY);
			EndPaint(hSettingWnd,&ps);
    		return 0;
        case WM_CLOSE:// 窗口销毁消息
        	hSettingWnd=NULL;
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

static int GetRefreshRate(int rate){
	int HZ=1000.0/rate;
	switch(HZ){
		case 33: return 0;
		case 66: return 36;
		case 76: return 54;
		case 125: return 107;
		case 142: return 136;
		case 166: return 160;
		case 250: return 249;
		case 333: return 320;
		default:
		{
			if(HZ<33) return 0;
			else return 320;
		}
	}
}

static int SetRefreshRate(int dot){
	switch(dot){
		case 0: return 33;
		case 36: return 15;
		case 54: return 13;
		case 107: return 8;
		case 136: return 7;
		case 160: return 6;
		case 249: return 4;
		case 320: return 3;
	}
}

static int ApplySet(){
	RefreshRate = SetRefreshRate(dot1);
	BGMVolume = (int)(100*dot2/320.0)*10;
	ForceBarSpeed = (int)(100*dot3/320.0);
	ResetSets();
	PostThreadMessage(BackGroundMusicID, 0xFFFF, BGM_VOLUME, 0); 
}

void ShowSettingWindow(){
	SettingWindow();
	return;
}

void HideSettingWindow(){
	if(hSettingWnd)
		PostMessage(hSettingWnd,WM_CLOSE,0,0); 
	return;
}
