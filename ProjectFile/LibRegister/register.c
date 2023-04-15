#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <wincon.h>
#include <Windows.h>
#include <stdbool.h>
#include <error.h>
#include <register.h>
#include <public.h> 
#include <graphics.h>
#include <extgraph.h>

extern char IF_FilePath[MAX_PATH];// 应用路径 

extern bool initialized;
extern double xResolution, yResolution;
extern double windowWidth;
extern double windowHeight;

extern HINSTANCE hInstance;//应用程序实例句柄 

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
{
	/*共享应用程序实例句柄 */ 
    hInstance = hThisInstance;
    return RegisterAll(hThisInstance,hPrevInstance,lpszArgument,nFunsterStil);
}

_register_h int RegisterAll(HINSTANCE hThisInstance,
                    		HINSTANCE hPrevInstance,
                   			LPSTR lpszArgument,
                    		int nFunsterStil){
	/*检查是否重复打开*/ 
	CreateMutex(0,1,"Billiards");
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		HWND hwnd = FindWindow(GWClassName,"Billiards");
		if(hwnd){
			FLASHWINFO info;
			info.cbSize = sizeof(info);
			info.hwnd = hwnd;
			info.dwFlags = FLASHW_TRAY;		//FLASHW_ALL标题和任务栏、FLASHW_CAPTION标题、FLASHW_STOP停止、FLASHW_TIMER不停、FLASHW_TIMERNOFG直到前端显示、FLASHW_TRAY任务栏
			info.uCount = 2; 				//闪烁窗口的次数
			info.dwTimeout = 0; 			//窗口闪烁的频度，毫秒为单位；若该值为0，则为默认图标的闪烁频度
			FlashWindowEx(&info);
			ShowWindowAsync(hwnd, SW_RESTORE);
			SetForegroundWindow(hwnd);
		}
		else MessageBox(NULL,"程序已经打开！","Billiards Error",16);
		return 0;
	}
	/*检查文件名*/
	int i,argn;
	char *s=GetCommandLine();
	char **cmdLineAry=(char **)CommandLineToArgvW((LPWSTR)s,&argn);
	char *str=(char*)malloc((strlen(cmdLineAry[0])-1)*sizeof(char));
	char *exename=(char*)malloc((strlen(EXE_NAME)+1)*sizeof(char));
	for(i=1;cmdLineAry[0][i]!='\"';++i) str[i-1]=cmdLineAry[0][i]; str[i-1]=0;
	if(str[strlen(str)-strlen(EXE_NAME)-1]=='\\'){
		for(i=0;i<strlen(EXE_NAME);i++) exename[i]=str[strlen(str)-strlen(EXE_NAME)+i]; exename[strlen(EXE_NAME)]=0;
		for(i=0;i<strlen(str)-strlen(EXE_NAME);i++) IF_FilePath[i]=str[i]; IF_FilePath[i]=0;
	}	
	if(strcmp(exename,EXE_NAME) != 0){
		free(str);
		free(exename);
		MessageBox(NULL,"程序名错误！","Billiards Error",16);
		return 0;
	}
	free(str);
	free(exename);
	/*检查屏幕分辨率*/
    double xSpace, ySpace;
    double xScale, yScale, scaleFactor;
    initialized = FALSE;
    xResolution = GetXResolution();
    yResolution = GetYResolution();
    #include <sets.h>
    SetWindowSize((double)(WIDTH) / xResolution, (double)(HEIGHT) / yResolution); //设置窗口大小 
    xSpace = GetFullScreenWidth();
    ySpace = GetFullScreenHeight();
    xScale = xSpace / windowWidth;
    yScale = ySpace / windowHeight;
    scaleFactor = (xScale < yScale) ? xScale : yScale;
	if (scaleFactor < 0.993530){
        MessageBox(NULL,"屏幕分辨率于仅支持的 [1280*768] 及以上！\n请检查：是否低于此分辨率 / 缩放后不满足要求~","Billiards Error",16);
        return 0;
    }
	/*联网检查版本*/
	#ifdef WEB_VERSION 
	#include <http.h>
	ft_http_client_t* http = 0;	
    ft_http_init();	
    http = ft_http_new();
	const char* body = 0;
	body = ft_http_sync_request(http, WEBSITE, M_GET,0,0,0,0);
	if(body==NULL){
		MessageBox(NULL,"连接服务器失败！","Billiards Error",16);
		return 0; 
	} 
	char webflag[10];
	{
		int i,pos=strlen(body)-55;
		#include <stdint.h>
		uint32_t buffer;
		for(i=0;i<4;++i){
		UTF8ToUnicode(body+pos+i, &buffer);
		webflag[i]=buffer;
		}
	}
	if(strcmp(webflag,"TRUE")!=0){
		MessageBox(NULL,"您的版本已过期！请联系作者~","Billiards Error",16);
		return 0;
	}
    if (http) ft_http_destroy(http);
    ft_http_deinit();
	#endif
	/*路径是否过长*/
	if(strlen(IF_FilePath)>0xFF) ErrorMsg("文件目录过长！\n请将文件夹放到桌面或者磁盘根目录打开！");
	/*文件检查*/
	FILE *check;
	if((check=fopen(FONTS_PATH,"r"))) fclose(check); else ErrorMsg("字体文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen(TITLE_FONTS_PATH,"r"))) fclose(check); else ErrorMsg("字体文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen(BUTTON_FONTS_PATH,"r"))) fclose(check); else ErrorMsg("字体文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen(TEXT_FONTS_PATH,"r"))) fclose(check); else ErrorMsg("字体文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen(FILE_CURSOR,"r"))) fclose(check); else ErrorMsg("光标文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen(FILE_ICON,"r"))) fclose(check); else ErrorMsg("图标文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen(FILE_TITLE_ICON,"r"))) fclose(check); else ErrorMsg("图标文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen("Data/Video/Basic.dat","r"))) fclose(check); else ErrorMsg("教学视频文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen("Data/Video/Classic.dat","r"))) fclose(check); else ErrorMsg("教学视频文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen("Data/Video/Pool.dat","r"))) fclose(check); else ErrorMsg("教学视频文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen("Data/Video/Snooker.dat","r"))) fclose(check); else ErrorMsg("教学视频文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen("Save/data.sav","r"))) fclose(check); else ErrorMsg("存档数据库文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen("Billiards.db","r"))) fclose(check); else ErrorMsg("图片数据库文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen("Sound.db","r"))) fclose(check); else ErrorMsg("音频数据库文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen("Config.ini","r"))) fclose(check); else ErrorMsg("配置设置文件丢失！\n请确认程序文件的完整性！");
	if((check=fopen("Readme.txt","r"))) fclose(check); else ErrorMsg("说明文档丢失！\n请确认程序文件的完整性！");
	/*加载资源*/
    Main();
    /* Here messages to the application are saved */
	MSG messages;
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    FreeConsole();
    return messages.wParam;
}
