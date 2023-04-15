#ifndef _pictures_h
#define _pictures_h

#include <stdbool.h>
#include <windows.h>
#include <public.h>
/*绘制图片函数库2.0*/

/*图片存储结构体*/
typedef struct
{
	long width;
	long height;
	HBITMAP hbitmap;
} IMAGE,*LPIMAGE;

/*加载&释放图片*/
_pictures_h bool LoadPicture(char *filename, LPIMAGE *mapbuf, HDC hDC); // 加载图片 (*.BMP/*.JPG/*.GIF) 
_pictures_h bool LoadPictureFromDB(LPDATABASE data, int datapos, LPIMAGE *mapbuf, HDC hDC); // 从数据库读取图片 (*.BMP/*.JPG/*.GIF) 
_pictures_h void LoadPictureFromWnd(LPIMAGE *mapbuf,int x,int y,int screenX,int screenY,HWND hWnd); //从当前显示界面加载图片 
_pictures_h bool FreePicture(LPIMAGE *mapbuf); // 释放图片 (*.BMP/*.JPG/*.GIF) 

/*显示图片*/
_pictures_h void DrawPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// 显示图片3.0
_pictures_h void DrawAvatar(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// 显示头像 

/*简单效果*/
_pictures_h void DrawAreaPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,int oriX,int oriY,int areaX,int areaY,HWND hWnd,HDC hMDC);// 显示图片指定范围2.0
_pictures_h void DrawInvertPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// 反色显示图片2.0

/*透明图片*/
_pictures_h void DrawAlphaPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,char alpha);// 指定透明度alpha显示图片2.0
_pictures_h void Draw32BitPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,char alpha);// 显示32位含透明通道图片2.0
_pictures_h void DrawMaskPicture(LPIMAGE pImage,LPIMAGE pMaskImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// 遮罩法显示透明图片2.0
_pictures_h void DrawKeyColorPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// 色彩法显示透明图片2.0
_pictures_h void DrawLimitedPicture(LPIMAGE pImage,LPIMAGE pBackImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// 将非透明区域恢复为背景图片2.0

/*旋转图片*/
_pictures_h void RotatePictureA(LPIMAGE pImage,int x,int y,int screenX,int screenY,float degree,HWND hWnd,HDC hMDC);// 绕中心旋转图片&指定度数2.0
_pictures_h void RotatePictureB(LPIMAGE pImage,int x,int y,int screenX,int screenY,float cosine,float sine,HWND hWnd,HDC hMDC);// 绕中心旋转图片&已知三角函数 2.0
_pictures_h void RotatePictureC(LPIMAGE pImage,int x,int y,int screenX,int screenY,float cosine,float sine,HWND hWnd,HDC hMDC);// 绕顶角旋转图片&已知三角函数 1.0
_pictures_h void RotatePictureD(LPIMAGE pImage0,LPIMAGE pImage1,int cx,int cy,int d,double cosine,double sine,HWND hWnd,HDC hMDC);// 绕顶角旋转图片&已知三角函数 2.0

/*模糊效果*/
_pictures_h void DrawBlurPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,int radius);// 图片模糊 
 
/*获取截图*/
_pictures_h void SaveScrBmp(char* filename,HWND hWnd); //获取当前屏幕客户区截图 
_pictures_h void SaveDskBmp(char* filename); //获取当前桌面截图

/*图片叠加*/
_pictures_h void MakeBlurPicture(LPIMAGE *mapbuf,LPIMAGE pImage,HWND hWnd,int radius); // 创建图片模糊 
_pictures_h void MakeAlphaPicture(LPIMAGE *mapbuf,LPIMAGE pImage,HWND hWnd,char alpha); // 叠加透明图片 
_pictures_h void MakeSizePicture(LPIMAGE *mapbuf,LPIMAGE pImage,int w,int h,HWND hWnd); // 制作指定大小图片  

/*读写图片*/
_pictures_h bool ReadBMP(LPIMAGE *mapbuf,FILE *p,DWORD bytes);
_pictures_h void WriteBMP(LPIMAGE image,FILE *p); 

#endif
