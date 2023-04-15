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

extern char IF_FilePath[MAX_PATH];// Ӧ��·�� 

extern bool initialized;
extern double xResolution, yResolution;
extern double windowWidth;
extern double windowHeight;

extern HINSTANCE hInstance;//Ӧ�ó���ʵ����� 

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
{
	/*����Ӧ�ó���ʵ����� */ 
    hInstance = hThisInstance;
    return RegisterAll(hThisInstance,hPrevInstance,lpszArgument,nFunsterStil);
}

_register_h int RegisterAll(HINSTANCE hThisInstance,
                    		HINSTANCE hPrevInstance,
                   			LPSTR lpszArgument,
                    		int nFunsterStil){
	/*����Ƿ��ظ���*/ 
	CreateMutex(0,1,"Billiards");
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		HWND hwnd = FindWindow(GWClassName,"Billiards");
		if(hwnd){
			FLASHWINFO info;
			info.cbSize = sizeof(info);
			info.hwnd = hwnd;
			info.dwFlags = FLASHW_TRAY;		//FLASHW_ALL�������������FLASHW_CAPTION���⡢FLASHW_STOPֹͣ��FLASHW_TIMER��ͣ��FLASHW_TIMERNOFGֱ��ǰ����ʾ��FLASHW_TRAY������
			info.uCount = 2; 				//��˸���ڵĴ���
			info.dwTimeout = 0; 			//������˸��Ƶ�ȣ�����Ϊ��λ������ֵΪ0����ΪĬ��ͼ�����˸Ƶ��
			FlashWindowEx(&info);
			ShowWindowAsync(hwnd, SW_RESTORE);
			SetForegroundWindow(hwnd);
		}
		else MessageBox(NULL,"�����Ѿ��򿪣�","Billiards Error",16);
		return 0;
	}
	/*����ļ���*/
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
		MessageBox(NULL,"����������","Billiards Error",16);
		return 0;
	}
	free(str);
	free(exename);
	/*�����Ļ�ֱ���*/
    double xSpace, ySpace;
    double xScale, yScale, scaleFactor;
    initialized = FALSE;
    xResolution = GetXResolution();
    yResolution = GetYResolution();
    #include <sets.h>
    SetWindowSize((double)(WIDTH) / xResolution, (double)(HEIGHT) / yResolution); //���ô��ڴ�С 
    xSpace = GetFullScreenWidth();
    ySpace = GetFullScreenHeight();
    xScale = xSpace / windowWidth;
    yScale = ySpace / windowHeight;
    scaleFactor = (xScale < yScale) ? xScale : yScale;
	if (scaleFactor < 0.993530){
        MessageBox(NULL,"��Ļ�ֱ����ڽ�֧�ֵ� [1280*768] �����ϣ�\n���飺�Ƿ���ڴ˷ֱ��� / ���ź�����Ҫ��~","Billiards Error",16);
        return 0;
    }
	/*�������汾*/
	#ifdef WEB_VERSION 
	#include <http.h>
	ft_http_client_t* http = 0;	
    ft_http_init();	
    http = ft_http_new();
	const char* body = 0;
	body = ft_http_sync_request(http, WEBSITE, M_GET,0,0,0,0);
	if(body==NULL){
		MessageBox(NULL,"���ӷ�����ʧ�ܣ�","Billiards Error",16);
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
		MessageBox(NULL,"���İ汾�ѹ��ڣ�����ϵ����~","Billiards Error",16);
		return 0;
	}
    if (http) ft_http_destroy(http);
    ft_http_deinit();
	#endif
	/*·���Ƿ����*/
	if(strlen(IF_FilePath)>0xFF) ErrorMsg("�ļ�Ŀ¼������\n�뽫�ļ��зŵ�������ߴ��̸�Ŀ¼�򿪣�");
	/*�ļ����*/
	FILE *check;
	if((check=fopen(FONTS_PATH,"r"))) fclose(check); else ErrorMsg("�����ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen(TITLE_FONTS_PATH,"r"))) fclose(check); else ErrorMsg("�����ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen(BUTTON_FONTS_PATH,"r"))) fclose(check); else ErrorMsg("�����ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen(TEXT_FONTS_PATH,"r"))) fclose(check); else ErrorMsg("�����ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen(FILE_CURSOR,"r"))) fclose(check); else ErrorMsg("����ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen(FILE_ICON,"r"))) fclose(check); else ErrorMsg("ͼ���ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen(FILE_TITLE_ICON,"r"))) fclose(check); else ErrorMsg("ͼ���ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen("Data/Video/Basic.dat","r"))) fclose(check); else ErrorMsg("��ѧ��Ƶ�ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen("Data/Video/Classic.dat","r"))) fclose(check); else ErrorMsg("��ѧ��Ƶ�ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen("Data/Video/Pool.dat","r"))) fclose(check); else ErrorMsg("��ѧ��Ƶ�ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen("Data/Video/Snooker.dat","r"))) fclose(check); else ErrorMsg("��ѧ��Ƶ�ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen("Save/data.sav","r"))) fclose(check); else ErrorMsg("�浵���ݿ��ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen("Billiards.db","r"))) fclose(check); else ErrorMsg("ͼƬ���ݿ��ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen("Sound.db","r"))) fclose(check); else ErrorMsg("��Ƶ���ݿ��ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen("Config.ini","r"))) fclose(check); else ErrorMsg("���������ļ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	if((check=fopen("Readme.txt","r"))) fclose(check); else ErrorMsg("˵���ĵ���ʧ��\n��ȷ�ϳ����ļ��������ԣ�");
	/*������Դ*/
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
