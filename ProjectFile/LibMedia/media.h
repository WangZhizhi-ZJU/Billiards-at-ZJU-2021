#ifndef _media_h
#define _media_h

#include <stdbool.h>
#include <windows.h> 
#include <mmsystem.h>
#include <pthread.h>

#include <public.h>
#include <pictures.h>

/*媒体播放函数库*/ 

typedef struct{
	int size;
	char* data;
} MUSIC,*LPMUSIC; 

/*波形音频播放函数*/
_media_h void DelCastCD();
_media_h void CastSound(LPMUSIC music);

/*打开媒体*/ 
_media_h void LoadSound(char *filename,LPMUSIC* music); //WAV
_media_h void LoadSoundFromDB(LPDATABASE database,int datapos,LPMUSIC* music); //WAV
_media_h void LoadMusic(char *filename,char *medianame); //将mp3打开并标记为某名称 
_media_h void LoadVedio(char *filename,char *medianame,int x,int y,int screenX,int screenY,HWND hWnd); //在指定窗口中播放，且指定视频在窗口中的位置、宽高 
_media_h void LoadVedioIn(char *filename,char *medianame,int x,int y,int screenX,int screenY); //在已有窗口中播放，且指定视频在窗口中的位置、宽高 
_media_h void LoadVedioOut(char *filename,char *medianame,char *title,int screenX,int screenY); //在新窗口中播放,且指定窗口标题、视频宽高
_media_h void LoadPictureOut(char *filename,char *classname,char *title,int screenX,int screenY); //在新窗口中显示图片,且指定窗口名称、标题,图片宽高（0则为原图大小） 
_media_h void LoadPictureOutB(LPIMAGE IM_Picture_Out,char *classname,char *title,int screenX,int screenY); //在新窗口中显示图片,且指定窗口名称、标题,图片宽高（0则为原图大小） 

/*播放&暂停&控制*/
_media_h void StartMedia(char *medianame); //从头播放
_media_h void StartMediaWithNotify(char *medianame,HWND hWnd); //从头播放
_media_h void PauseMedia(char *medianame); //暂停 
_media_h void PlayMedia(char *medianame); //继续播放
_media_h void PlayMediaWithNotify(char *medianame,HWND hWnd); //继续播放
_media_h void StopMedia(char *medianame); //停止播放
_media_h void PlayMediaFrom(char *medianame,int position); //从指定位置播放 (ms) 
_media_h void PlayMediaFromWithNotify(char *medianame,int position,HWND hWnd); //从指定位置播放 (ms) 
_media_h void PlayMediaTo(char *medianame,int position); //播放至指定位置停止 (ms) 
_media_h void SeekMedia(char *medianame,int position); //移动至指定位置 (ms) 
_media_h void StartMediaInWithNotify(char *medianame); //播放并在原窗口获取播完消息 
_media_h void StartMediaOutWithNotify(char *medianame); //播放并在新窗口获取播完消息 

/*播放模式*/ 
_media_h void SetRepeat(char *medianame); //将该媒体循环播放 
_media_h void Fullscreen(char *medianame); //全屏播放媒体(会与重复播放冲突) 

/*播放参数设置*/
_media_h long GetVolume(char *medianame); //获取媒体音量 (1-1000) 
_media_h void SetVolume(char *medianame,int volume); //设置媒体音量 (1-1000) 
_media_h long GetLength(char *medianame); //获取媒体长度 (ms) 
_media_h long GetPosition(char *medianame); //获取当前位置 (ms) 
_media_h bool IsPlay(char *medianame); //判断媒体是否在播放 
_media_h void SetSpeed(char *medianame,int factor); //设置音频视频速度 (1000:正常/500:一半/2000:2倍/0:不丢帧尽可能快放)

/*关闭媒体*/
_media_h void CloseMedia(char *medianame); //关闭媒体文件 
_media_h void FreeSound(LPMUSIC* music);

/*媒体播放消息处理函数声明*/ 
_media_h void MediaMessage(WPARAM wParam,LPARAM lParam); 

#endif
