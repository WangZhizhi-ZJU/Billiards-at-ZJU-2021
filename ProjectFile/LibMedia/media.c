#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <graphics.h>
#include <pictures.h>
#include <newwindow.h>
#include <mmsystem.h> //-lwinmm 
#pragma comment(lib,"winmm.lib")

#include <public.h> 
#include <media.h>
#include <error.h>

//GetShortPathName();

/*�洢ý������*/
static int Point=0;
static char *media[MAX_MEDIA+1];

/*�洢ͼƬ*/
static int PointB=0;
static struct{
	LPIMAGE hbitmap;
	HWND hWND;
}pictures[MAX_PICTURES+1];

/*��ȡgraphics.c���ھ��*/
extern HWND graphicsWindow;
extern HCURSOR CS_Cursor; 

/*LoadMusic*/
extern HWND MenuMusicWindow;
extern HWND CollisionMusicWindow;
extern HWND BackGroundMusicWindow;

/*LoadVedioOut*/
static HWND PlayWindow=NULL;
static char *vedioname=NULL;

/*LoadPicture*/
static int x,y;
static HDC hDC,hMDC;
static PAINTSTRUCT ps;

/*ý����Ϣ�ص�����*/
void MediaMessage(WPARAM wParam,LPARAM lParam);

/*ý���¼����*/
static void AddMedia(char *medianame);
static bool HasMedia(char *medianame);
static void DelMedia(char *medianame);

/*ͼƬ��¼����*/
static void DelPicture(HWND hwnd);
static LPIMAGE AddPicture(char *filename,HWND hwnd); 

void LoadSound(char *filename,LPMUSIC* music){ //WAV
	FILE* alertMusicFile = NULL;
	alertMusicFile = fopen(filename,"rb");
	if(!alertMusicFile){
		ErrorMsg("��Ƶ�ļ���ȡʧ�ܣ�");
		return; 
	} 
	fseek(alertMusicFile,0,SEEK_END);
	long fileSize = ftell(alertMusicFile);
	rewind(alertMusicFile);
	int num = fileSize / sizeof(char);
	(*music)=(LPMUSIC)malloc(sizeof(MUSIC));
	(*music)->data=(char*)malloc(sizeof(char)*num);
	(*music)->size=num;
	fread((*music)->data,sizeof(char),num,alertMusicFile);
	fclose(alertMusicFile);
}

void LoadSoundFromDB(LPDATABASE database,int datapos,LPMUSIC* music){ //WAV
	int i;
	FILE* alertMusicFile = NULL;
	alertMusicFile = fopen(database->basename,"rb");
	if(!alertMusicFile){
		ErrorMsg("��Ƶ�ļ���ȡʧ�ܣ�");
		return; 
	} 
	fseek(alertMusicFile,(database->datapos+datapos)->pos,SEEK_SET);
	int num = (database->datapos+datapos)->filesize / sizeof(char);
	(*music)=(LPMUSIC)malloc(sizeof(MUSIC));
	(*music)->data=(char*)malloc(sizeof(char)*num);
	(*music)->size=num;
	fread((*music)->data,sizeof(char),num,alertMusicFile);
	fclose(alertMusicFile);
}

void FreeSound(LPMUSIC* music){
	free((*music)->data);
	free((*music));
	(*music)=NULL;
}

void LoadMusic(char *filename,char *medianame){ //��mp3�򿪲����Ϊĳ���� 
	if(HasMedia(medianame)){
		printf("Media Have Been Open\n");
		return;
	}
	char command[100];
	sprintf(command,"open %s alias %s",filename,medianame);//type waveaudio
	if(mciSendString(command,NULL,0,NULL)){
		printf("Media Open Error\n");
		return;
	}
	AddMedia(medianame);
	sprintf(command,"set %s time format milliseconds",medianame);//����ʱ���ʽΪ����
	mciSendString(command,NULL,0,NULL);
}

void LoadVedio(char *filename,char *medianame,int x,int y,int screenX,int screenY,HWND hWnd){ //��ָ�������в��ţ���ָ����Ƶ�ڴ����е�λ�á���� 
	if(HasMedia(medianame)){
		printf("Media Have Been Open\n");
		return;
	}
	char command[100];
	sprintf(command,"open %s type MPEGVideo alias %s parent %u style %u",filename,medianame,hWnd,WS_CHILD);
	if(mciSendString(command,NULL,0,NULL)){
		printf("Media Open Error\n");
		return;
	}
	AddMedia(medianame);
	sprintf(command,"put %s window at %d %d %d %d",medianame,x,y,screenX,screenY);
	mciSendString(command,NULL,0,NULL);
	sprintf(command,"set %s time format milliseconds",medianame);//����ʱ���ʽΪ����
	mciSendString(command,NULL,0,NULL);
}

void LoadVedioIn(char *filename,char *medianame,int x,int y,int screenX,int screenY){ //�����д����в��ţ���ָ����Ƶ�ڴ����е�λ�á���� 
	if(HasMedia(medianame)){
		printf("Media Have Been Open\n");
		return;
	}
	char command[100];
	sprintf(command,"open %s type MPEGVideo alias %s parent %u style %u",filename,medianame,graphicsWindow,WS_CHILD);
	if(mciSendString(command,NULL,0,NULL)){
		printf("Media Open Error\n");
		return;
	}
	AddMedia(medianame);
	sprintf(command,"put %s window at %d %d %d %d",medianame,x,y,screenX,screenY);
	mciSendString(command,NULL,0,NULL);
	sprintf(command,"set %s time format milliseconds",medianame);//����ʱ���ʽΪ����
	mciSendString(command,NULL,0,NULL);
}

static LRESULT CALLBACK PlayEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//��Ƶ��Ϣ�ص����� 
void LoadVedioOut(char *filename,char *medianame,char *title,int screenX,int screenY){ //���´����в���,��ָ�����ڱ��⡢��Ƶ���
	if(HasMedia(medianame)){
		printf("Media Have Been Open\n");
		return;
	}
	if(vedioname || PlayWindow!=NULL){
		printf("ֻ�ܵ�������һ����Ƶ��\n");
		return;
	}
	HDC hDC;
	HDC hMemoryDC;
	vedioname=medianame;
	char command[100];
	PlayWindow = NewWindow(medianame,title,screenX+6,screenY+29,PlayEventProc,&hDC,&hMemoryDC);
	sprintf(command,"open %s type MPEGVideo alias %s parent %u style %u",filename,medianame,PlayWindow,WS_CHILD);
	if(mciSendString(command,NULL,0,NULL)){
		printf("Media Open Error\n");
		vedioname=NULL;
		DestroyWindow(PlayWindow);
		PlayWindow=NULL;
		return;
	}
	AddMedia(medianame);
	sprintf(command,"put %s window at %d %d %d %d",medianame,0,0,screenX,screenY);
	mciSendString(command,NULL,0,NULL);
	sprintf(command,"set %s time format milliseconds",medianame);//����ʱ���ʽΪ����
	mciSendString(command,NULL,0,NULL);
	//sprintf(command,"window %s text title",medianame);
	//mciSendString(command,NULL,0,NULL);
}

#define IM_MAX_SIZE 600.0
static LRESULT CALLBACK PictureEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//ͼƬ��Ϣ�ص����� 
void LoadPictureOut(char *filename,char *classname,char *title,int screenX,int screenY){ //���´�������ʾͼƬ,��ָ���������ơ�����,ͼƬ��ߣ�0��Ϊԭͼ��С�� 
	bool sizechanged=false; 
	if(!screenX || !screenY){
		sizechanged=true;
		screenX=10;
		screenY=10;
	}
	x=screenX,y=screenY;
	DestroyCursor(CS_Cursor);
	CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
	HWND PictureWindow = NewWindow(classname,title,screenX,screenY,PictureEventProc,&hDC,&hMDC);
	if(sizechanged) ShowWindow(PictureWindow, SW_HIDE);
	LPIMAGE IM_Picture_Out=AddPicture(filename,PictureWindow);
	if(sizechanged){
		if(IM_Picture_Out->width > IM_Picture_Out->height){
			x=screenX=IM_MAX_SIZE;
			y=screenY=IM_Picture_Out->height*IM_MAX_SIZE/IM_Picture_Out->width;
		}else{
			y=screenY=IM_MAX_SIZE;
			x=screenX=IM_Picture_Out->width*IM_MAX_SIZE/IM_Picture_Out->height;
		}
		ChangeWindowSize(PictureWindow,hMDC,screenX,screenY);
		ShowWindow(PictureWindow, SW_SHOWNORMAL);
	}
	DrawAvatar(IM_Picture_Out,0,0,screenX,screenY,PictureWindow,hMDC);
	EnableWindow(graphicsWindow,false);
}

void LoadPictureOutB(LPIMAGE IM_Picture_Out,char *classname,char *title,int screenX,int screenY){ //���´�������ʾͼƬ,��ָ���������ơ�����,ͼƬ��ߣ�0��Ϊԭͼ��С�� 
	if(!screenX || !screenY){
		screenX=IM_Picture_Out->width;
		screenY=IM_Picture_Out->height;
	}
	x=screenX,y=screenY;
	DestroyCursor(CS_Cursor);
	CS_Cursor = LoadCursorFromFile(FILE_CURSOR);
	HWND PictureWindow = NewWindow(classname,title,screenX,screenY,PictureEventProc,&hDC,&hMDC);
	DrawAvatar(IM_Picture_Out,0,0,screenX,screenY,PictureWindow,hMDC);
	EnableWindow(graphicsWindow,false);
}

void StartMedia(char *medianame){ //��ͷ����
	char command[100];
	sprintf(command,"play %s",medianame);
	mciSendString(command,NULL,0,NULL);
}

void StartMediaWithNotify(char *medianame,HWND hWnd){ //��ͷ����
	char command[100];
	sprintf(command,"play %s notify",medianame);
	mciSendString(command,NULL,0,hWnd);
}

void StartMediaInWithNotify(char *medianame){ //���Ų���ԭ���ڻ�ȡ������Ϣ 
	char command[100];
	sprintf(command,"play %s notify",medianame);
	mciSendString(command,NULL,0,graphicsWindow);
}

void StartMediaOutWithNotify(char *medianame){ //���Ų����´��ڻ�ȡ������Ϣ 
	char command[100];
	sprintf(command,"play %s notify",medianame);
	mciSendString(command,NULL,0,PlayWindow);
}

void StartMenuMusicOutWithNotify(char *medianame){ //���Ų����´��ڻ�ȡ������Ϣ 
	char command[100];
	sprintf(command,"play %s notify",medianame);
	mciSendString(command,NULL,0,MenuMusicWindow);
}

void StartCollisionMusicOutWithNotify(char *medianame){ //���Ų����´��ڻ�ȡ������Ϣ 
	char command[100];
	sprintf(command,"play %s notify",medianame);
	mciSendString(command,NULL,0,CollisionMusicWindow);
}

void StartBackGroundMusicOutWithNotify(char *medianame){ //���Ų����´��ڻ�ȡ������Ϣ 
	char command[100];
	sprintf(command,"play %s notify",medianame);
	mciSendString(command,NULL,0,BackGroundMusicWindow);
}

void PauseMedia(char *medianame){ //��ͣ
	char command[100];
	sprintf(command,"pause %s",medianame);
	mciSendString(command,NULL,0,NULL);
}

void PlayMedia(char *medianame){ //��������
	char command[100];
	sprintf(command,"resume %s",medianame);
	mciSendString(command,NULL,0,NULL);
}

void PlayMediaWithNotify(char *medianame,HWND hWnd){ //��������
	char command[100];
	sprintf(command,"resume %s notify",medianame);
	mciSendString(command,NULL,0,hWnd);
}

void StopMedia(char *medianame){ //ֹͣ����
	char command[100];
	sprintf(command,"stop %s",medianame);
	mciSendString(command,NULL,0,NULL);
}

void PlayMediaFrom(char *medianame,int position){ //��ָ��λ�ò��� (ms) 
	char command[100];
	sprintf(command,"play %s from %d",medianame,position);
	mciSendString(command,NULL,0,NULL);
}

void PlayMediaFromWithNotify(char *medianame,int position,HWND hWnd){ //��ָ��λ�ò��� (ms) 
	char command[100];
	sprintf(command,"play %s from %d notify",medianame,position);
	mciSendString(command,NULL,0,hWnd);
}

void PlayMediaTo(char *medianame,int position){ //������ָ��λ��ֹͣ (ms) 
	char command[100];
	sprintf(command,"play %s to %d",medianame,position);
	mciSendString(command,NULL,0,NULL);
}

void SeekMedia(char *medianame,int position){ //�ƶ���ָ��λ�� (ms) 
	char command[100];
	sprintf(command,"seek %s to %d",medianame,position);
	mciSendString(command,NULL,0,NULL);
}

void SetRepeat(char *medianame){ //����ý��ѭ������ 
	char command[100];
	sprintf(command,"play %s repeat",medianame);
	mciSendString(command,NULL,0,NULL);
}

void SetSpeed(char *medianame,int factor){ //������Ƶ��Ƶ�ٶ� (factorΪһ����ֵ��1000�����ٶ�/500�����ٶ�һ��/2000�����ٶ�2��/0��������Ƶ֡������¾����ܵؿ��)
	char command[100];
	sprintf(command,"set %s speed %d",medianame,factor);//type waveaudio
	mciSendString(command,NULL,0,NULL);
}

long GetVolume(char *medianame){ //��ȡý������ (1-1000) 
	long lVolume;
	char command[100];
	char chVolume[256];
	sprintf(command,"status %s volume",medianame);
	mciSendString(command,chVolume,255,NULL);
	lVolume=strtol(chVolume,NULL,10);
	//printf("Volume:%ld\n",lVolume);
	return lVolume;
}

void SetVolume(char *medianame,int volume){ //����ý������ (1-1000) 
	char command[100];
	sprintf(command,"setaudio %s volume to %d",medianame,volume);
	mciSendString(command,NULL,0,NULL);
}

void Fullscreen(char *medianame){ //ȫ������ý��(�����ظ����ų�ͻ) 
	char command[100];
	sprintf(command,"play %s fullscreen",medianame);
	mciSendString(command,NULL,0,NULL);
}

long GetLength(char *medianame){ //��ȡý�峤�� (ms) 
	long lLength;
	char command[100];
	char sPosition[256];
	wsprintf(command,"status %s length",medianame);
	mciSendString(command,sPosition,255,0);
	lLength=strtol(sPosition,NULL,10);
	//printf("Length:%ld\n",lLength);
	return lLength;
}

long GetPosition(char *medianame){ //��ȡ��ǰλ�� (ms) 
	long lPosition;
	char command[100];
	char sPosition[256];
	sprintf(command,"status %s position",medianame);
	mciSendString(command,sPosition,255,0);
	lPosition=strtol(sPosition,NULL,10);
	//printf("Position:%ld\n",lPosition);
	return lPosition;
}

bool IsPlay(char *medianame){ //�ж�ý���Ƿ��ڲ��� 
	char command[100];
	char sInformation[256];
	sprintf(command,"status %s mode",medianame);
	mciSendString(command,sInformation,255,0);
	//printf("Mode:%s\n",sInformation);
	return sInformation[0]=='p'?true:false;
}

void CloseMedia(char *medianame){ //�ر�ý���ļ� 
	char command[100];
	sprintf(command,"close %s",medianame);
	mciSendString(command,NULL,0,NULL);
	DelMedia(medianame);
}

static LRESULT CALLBACK PlayEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg)
    {
    	case MM_MCINOTIFY:
    		MediaMessage(wParam,lParam);
    		return 0;
        case WM_CLOSE:// ����������Ϣ
            PlayWindow=NULL;
			DestroyWindow(hwnd);
            CloseMedia(vedioname);
            DelMedia(vedioname);
            vedioname=NULL;
        	return 0;
        default:
            // pass��ϵͳ
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

static LRESULT CALLBACK PictureEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg)
    {
    	case WM_PAINT:// ��ͼ��Ϣ
			hDC=BeginPaint(hwnd,&ps);
			BitBlt(hDC,0,0,x,y,hMDC,0,0,SRCCOPY);
    		EndPaint(hwnd,&ps);
    		return 0;
        case WM_CLOSE:// ����������Ϣ
        	EnableWindow(graphicsWindow,true);
        	SetForegroundWindow(graphicsWindow);
        	DelPicture(hwnd);
            DestroyWindow(hwnd);
        	return 0;
        case WM_SETCURSOR:// ���ָ�������Ϣ 
			SetCursor(CS_Cursor);
			return 0;
		case WM_SYSCOMMAND:
			switch(wParam){
				case HTCAPTION|SC_MOVE://�����϶����� 
					return 0;
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}
		case WM_KEYDOWN:
			if(wParam==VK_ESCAPE)
				PostMessage(hwnd,WM_CLOSE,0,0); 
			return 0; 
        default:
            // pass��ϵͳ
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

#include <test.h>

void MediaMessage(WPARAM wParam,LPARAM lParam){
	switch(wParam){
		case MCI_NOTIFY_SUCCESSFUL:
			{
			int loop;
			for(loop=1;loop<=Point;loop++)
				printf("[%d]%s��",loop,media[loop]);
			printf("\n%d %s Over\n",lParam,media[lParam]);
			if(media[lParam][0] == '_')
				BackGroundMusic();
			}
		return;
	}
}

static void AddMedia(char *medianame){
	media[++Point]=(char*)malloc((strlen(medianame)+1)*sizeof(char));
	strcpy(media[Point],medianame);
	return;
}

static bool HasMedia(char *medianame){
	int loop;
	for(loop=1;loop<=Point;loop++)
		if(strcmp(medianame,media[loop])==0) return true;
	return false;
}

static void DelMedia(char *medianame){
	int loop;
	for(loop=1;loop<=Point;loop++)
		if(strcmp(medianame,media[loop])==0) break;
	if(loop>Point) return;
	free(media[loop]);
	for(;loop<Point;loop++)
		media[loop]=media[loop+1];
	Point--;
	return;
}

static LPIMAGE AddPicture(char *filename,HWND hwnd){
	pictures[++PointB].hWND=hwnd;
	LoadPicture(filename,&pictures[PointB].hbitmap,NULL);
	return pictures[PointB].hbitmap;
}

static void DelPicture(HWND hwnd){
	int loop;
	for(loop=1;loop<=PointB;loop++)
		if(pictures[loop].hWND==hwnd) break;
	if(loop>PointB) return;
	FreePicture(&pictures[loop].hbitmap);
	for(;loop<PointB;loop++)
		pictures[loop]=pictures[loop+1];
	PointB--;
	return;
}
