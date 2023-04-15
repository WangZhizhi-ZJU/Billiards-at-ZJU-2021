#ifndef _pictures_h
#define _pictures_h

#include <stdbool.h>
#include <windows.h>
#include <public.h>
/*����ͼƬ������2.0*/

/*ͼƬ�洢�ṹ��*/
typedef struct
{
	long width;
	long height;
	HBITMAP hbitmap;
} IMAGE,*LPIMAGE;

/*����&�ͷ�ͼƬ*/
_pictures_h bool LoadPicture(char *filename, LPIMAGE *mapbuf, HDC hDC); // ����ͼƬ (*.BMP/*.JPG/*.GIF) 
_pictures_h bool LoadPictureFromDB(LPDATABASE data, int datapos, LPIMAGE *mapbuf, HDC hDC); // �����ݿ��ȡͼƬ (*.BMP/*.JPG/*.GIF) 
_pictures_h void LoadPictureFromWnd(LPIMAGE *mapbuf,int x,int y,int screenX,int screenY,HWND hWnd); //�ӵ�ǰ��ʾ�������ͼƬ 
_pictures_h bool FreePicture(LPIMAGE *mapbuf); // �ͷ�ͼƬ (*.BMP/*.JPG/*.GIF) 

/*��ʾͼƬ*/
_pictures_h void DrawPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// ��ʾͼƬ3.0
_pictures_h void DrawAvatar(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// ��ʾͷ�� 

/*��Ч��*/
_pictures_h void DrawAreaPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,int oriX,int oriY,int areaX,int areaY,HWND hWnd,HDC hMDC);// ��ʾͼƬָ����Χ2.0
_pictures_h void DrawInvertPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// ��ɫ��ʾͼƬ2.0

/*͸��ͼƬ*/
_pictures_h void DrawAlphaPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,char alpha);// ָ��͸����alpha��ʾͼƬ2.0
_pictures_h void Draw32BitPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,char alpha);// ��ʾ32λ��͸��ͨ��ͼƬ2.0
_pictures_h void DrawMaskPicture(LPIMAGE pImage,LPIMAGE pMaskImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// ���ַ���ʾ͸��ͼƬ2.0
_pictures_h void DrawKeyColorPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// ɫ�ʷ���ʾ͸��ͼƬ2.0
_pictures_h void DrawLimitedPicture(LPIMAGE pImage,LPIMAGE pBackImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC);// ����͸������ָ�Ϊ����ͼƬ2.0

/*��תͼƬ*/
_pictures_h void RotatePictureA(LPIMAGE pImage,int x,int y,int screenX,int screenY,float degree,HWND hWnd,HDC hMDC);// ��������תͼƬ&ָ������2.0
_pictures_h void RotatePictureB(LPIMAGE pImage,int x,int y,int screenX,int screenY,float cosine,float sine,HWND hWnd,HDC hMDC);// ��������תͼƬ&��֪���Ǻ��� 2.0
_pictures_h void RotatePictureC(LPIMAGE pImage,int x,int y,int screenX,int screenY,float cosine,float sine,HWND hWnd,HDC hMDC);// �ƶ�����תͼƬ&��֪���Ǻ��� 1.0
_pictures_h void RotatePictureD(LPIMAGE pImage0,LPIMAGE pImage1,int cx,int cy,int d,double cosine,double sine,HWND hWnd,HDC hMDC);// �ƶ�����תͼƬ&��֪���Ǻ��� 2.0

/*ģ��Ч��*/
_pictures_h void DrawBlurPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,int radius);// ͼƬģ�� 
 
/*��ȡ��ͼ*/
_pictures_h void SaveScrBmp(char* filename,HWND hWnd); //��ȡ��ǰ��Ļ�ͻ�����ͼ 
_pictures_h void SaveDskBmp(char* filename); //��ȡ��ǰ�����ͼ

/*ͼƬ����*/
_pictures_h void MakeBlurPicture(LPIMAGE *mapbuf,LPIMAGE pImage,HWND hWnd,int radius); // ����ͼƬģ�� 
_pictures_h void MakeAlphaPicture(LPIMAGE *mapbuf,LPIMAGE pImage,HWND hWnd,char alpha); // ����͸��ͼƬ 
_pictures_h void MakeSizePicture(LPIMAGE *mapbuf,LPIMAGE pImage,int w,int h,HWND hWnd); // ����ָ����СͼƬ  

/*��дͼƬ*/
_pictures_h bool ReadBMP(LPIMAGE *mapbuf,FILE *p,DWORD bytes);
_pictures_h void WriteBMP(LPIMAGE image,FILE *p); 

#endif
