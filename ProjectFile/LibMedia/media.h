#ifndef _media_h
#define _media_h

#include <stdbool.h>
#include <windows.h> 
#include <mmsystem.h>
#include <pthread.h>

#include <public.h>
#include <pictures.h>

/*ý�岥�ź�����*/ 

typedef struct{
	int size;
	char* data;
} MUSIC,*LPMUSIC; 

/*������Ƶ���ź���*/
_media_h void DelCastCD();
_media_h void CastSound(LPMUSIC music);

/*��ý��*/ 
_media_h void LoadSound(char *filename,LPMUSIC* music); //WAV
_media_h void LoadSoundFromDB(LPDATABASE database,int datapos,LPMUSIC* music); //WAV
_media_h void LoadMusic(char *filename,char *medianame); //��mp3�򿪲����Ϊĳ���� 
_media_h void LoadVedio(char *filename,char *medianame,int x,int y,int screenX,int screenY,HWND hWnd); //��ָ�������в��ţ���ָ����Ƶ�ڴ����е�λ�á���� 
_media_h void LoadVedioIn(char *filename,char *medianame,int x,int y,int screenX,int screenY); //�����д����в��ţ���ָ����Ƶ�ڴ����е�λ�á���� 
_media_h void LoadVedioOut(char *filename,char *medianame,char *title,int screenX,int screenY); //���´����в���,��ָ�����ڱ��⡢��Ƶ���
_media_h void LoadPictureOut(char *filename,char *classname,char *title,int screenX,int screenY); //���´�������ʾͼƬ,��ָ���������ơ�����,ͼƬ��ߣ�0��Ϊԭͼ��С�� 
_media_h void LoadPictureOutB(LPIMAGE IM_Picture_Out,char *classname,char *title,int screenX,int screenY); //���´�������ʾͼƬ,��ָ���������ơ�����,ͼƬ��ߣ�0��Ϊԭͼ��С�� 

/*����&��ͣ&����*/
_media_h void StartMedia(char *medianame); //��ͷ����
_media_h void StartMediaWithNotify(char *medianame,HWND hWnd); //��ͷ����
_media_h void PauseMedia(char *medianame); //��ͣ 
_media_h void PlayMedia(char *medianame); //��������
_media_h void PlayMediaWithNotify(char *medianame,HWND hWnd); //��������
_media_h void StopMedia(char *medianame); //ֹͣ����
_media_h void PlayMediaFrom(char *medianame,int position); //��ָ��λ�ò��� (ms) 
_media_h void PlayMediaFromWithNotify(char *medianame,int position,HWND hWnd); //��ָ��λ�ò��� (ms) 
_media_h void PlayMediaTo(char *medianame,int position); //������ָ��λ��ֹͣ (ms) 
_media_h void SeekMedia(char *medianame,int position); //�ƶ���ָ��λ�� (ms) 
_media_h void StartMediaInWithNotify(char *medianame); //���Ų���ԭ���ڻ�ȡ������Ϣ 
_media_h void StartMediaOutWithNotify(char *medianame); //���Ų����´��ڻ�ȡ������Ϣ 

/*����ģʽ*/ 
_media_h void SetRepeat(char *medianame); //����ý��ѭ������ 
_media_h void Fullscreen(char *medianame); //ȫ������ý��(�����ظ����ų�ͻ) 

/*���Ų�������*/
_media_h long GetVolume(char *medianame); //��ȡý������ (1-1000) 
_media_h void SetVolume(char *medianame,int volume); //����ý������ (1-1000) 
_media_h long GetLength(char *medianame); //��ȡý�峤�� (ms) 
_media_h long GetPosition(char *medianame); //��ȡ��ǰλ�� (ms) 
_media_h bool IsPlay(char *medianame); //�ж�ý���Ƿ��ڲ��� 
_media_h void SetSpeed(char *medianame,int factor); //������Ƶ��Ƶ�ٶ� (1000:����/500:һ��/2000:2��/0:����֡�����ܿ��)

/*�ر�ý��*/
_media_h void CloseMedia(char *medianame); //�ر�ý���ļ� 
_media_h void FreeSound(LPMUSIC* music);

/*ý�岥����Ϣ����������*/ 
_media_h void MediaMessage(WPARAM wParam,LPARAM lParam); 

#endif
