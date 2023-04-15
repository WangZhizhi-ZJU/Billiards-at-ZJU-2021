#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <graphics.h> 
#include <pictures.h>

#include <error.h>
#include <public.h>//���ú궨�� 

#include <olectl.h>
#pragma comment(lib,"Msimg32.lib")

static long long picturetimes = 0;

extern HDC gdc, osdc;
extern HWND graphicsWindow;

extern double PIXELPERMMX;
extern double PIXELPERMMY;

bool LoadPicture(char *filename, LPIMAGE *mapbuf, HDC hDC) // ����ͼƬ (*.BMP/*.JPG/*.GIF) 
{
	if(!hDC) hDC = gdc;
	HANDLE file = NULL;
	file = CreateFile(				//��ָ����·�� filename �ж�ȡ�ļ����
			filename, 				//Ҫ�򿪵��ļ��������豸��������ַ�������󳤶���ANSI�汾��ΪMAX_PATH����unicode�汾��Ϊ32767��
			GENERIC_READ,			//GENERIC_READ �����ʣ�GENERIC_WRITE д���ʣ������ʹ�ã����㣬��ʾֻ�����ȡ��һ���豸�йص���Ϣ ��
			FILE_SHARE_READ,		//�� ������FILE_SHARE_DELETEֻ��ɾ�����������Ȩ�ޣ�FILE_SHARE_READֻ����������ʵ�Ȩ�ޣ�FILE_SHARE_WRITE ֻ������д���ʵ�Ȩ�ޡ�
			NULL,					//ָ��һ��SECURITY_ATTRIBUTES�ṹ��ָ�룬�������ļ��İ�ȫ���ԣ��������ϵͳ֧�ֵĻ���
			OPEN_EXISTING,			//CREATE_NEW ����,���������;CREATE_ALWAYS ����,��д;OPEN_EXISTING �������;OPEN_ALWAYS �粻�����򴴽�;TRUNCATE_EXISTING �������ļ�����Ϊ�㳤��
			FILE_ATTRIBUTE_NORMAL,	//FILE_ATTRIBUTE_NORMAL Ĭ������
			NULL					//ͨ�������������ΪNULL��Ϊ�ձ�ʾ��ʹ��ģ�壬һ��Ϊ�ա�
			);
	if(file == INVALID_HANDLE_VALUE){ 
		ErrorMsg("ͼƬ�ļ���ȡʧ�ܣ�");
		printf("Fail to load image, File not exist\n");
		return false;
	} 
	*mapbuf = (LPIMAGE)malloc(sizeof(IMAGE));
	DWORD filesize = 0;
	filesize = GetFileSize(file, NULL);	//�ú������ڻ�ȡָ���ļ��Ĵ�С�����ȣ�����ȡ�Ĵ�С������ 0xFFFFFFFF(4GB) ���ڡ���������ʹ�� GetFileSizeEx ������
	HGLOBAL global = NULL;
	global = GlobalAlloc(GMEM_MOVEABLE, filesize); //�ú����Ӷ��з���һ����Ŀ���ֽ�����GMEM_MOVEABLE ������ƶ����ڴ�,�����ڴ������,�ú��� GlobalLock תΪָ�롣 
	LPVOID data = NULL;
	data = GlobalLock(global);
	DWORD bytes;
	ReadFile(			//ע�������һ��ָ��ָ���ָ�롣 
		 file,			//��Ҫ�������ݵ��ļ����,������ָ����ļ�������GENERIC_READ �������Ե��ļ��� 
		 data, 			//�������ݵĻ�������
		 filesize, 		//ָ��Ҫ��ȡ���ֽ�����
		 &bytes, 		//ָ��һ��DWORD���ͱ�����ָ�룬�������ն�ȡ���ֽ����������һ������ΪNULL����ôһ��Ҫ�������������
		 NULL);			//OVERLAPPED�ṹ��ָ��,����ļ�����FILE_FLAG_OVERLAPPED��ʽ�򿪵Ļ�,��ô���ָ��Ͳ���ΪNULL��
	GlobalUnlock(global); //GlobalUnlock��������������ڴ�飬ʹָ����ڴ���ָ����Ч��GlobalLock�������ڴ棬һ��Ҫ��GlobalUnlock������
	IStream *istream;
	CreateStreamOnHGlobal(		//��ָ���ڴ洴��������
					global, 	//��GlobalAlloc����������ڴ�����
					TRUE, 		//ָ���ƶ����ڴ��ڸö����ͷź��Ƿ�Ҳ�Զ��ͷš�FALSE��������ʽ���ͷ�hGlobal��TRUE��hGlobal���ջ��Զ��ͷš�
					&istream); 	//IStreamָ��ĵ�ַ����ָ���ڸú���ִ�к�ָ���´����������󡣸ò�������ΪNULL��
	IPicture *ipicture; //����һ��IPicture�ӿ�ָ�룬��ʾͼƬ����
	OleLoadPicture(
			istream, 				//ָ�������ͼ�����ݵ�����ָ�롣 
			filesize, 				//�����ж�ȡ���ֽ���; ���Ӧ��ȡ����������Ϊ�㡣
			TRUE, 					//ͼ�����Զ�Ӧ�ĳ�ֵ ��KeepOriginalFormat���Եĳ�ʼֵ�෴�����ΪTRUE����KeepOriginalFormat����ΪFALSE����֮��Ȼ��
			&IID_IPicture, 			//���ýӿڵı�ʶ�����ñ�ʶ������Ҫ��ppvObj�з��صĽӿ�ָ������͡�IPicture
			(LPVOID*)&ipicture);	//���IPicture�ӿ�ָ��
	OLE_XSIZE_HIMETRIC width;
	OLE_YSIZE_HIMETRIC height;
	ipicture->lpVtbl->get_Width(ipicture, &width);
	ipicture->lpVtbl->get_Height(ipicture, &height);
	(*mapbuf)->width = 1 + (int)(width / PIXELPERMMX);
	(*mapbuf)->height = 1 + (int)(height / PIXELPERMMY);
	HDC hmapdc;
	hmapdc = CreateCompatibleDC(hDC);
	if ((*mapbuf)->hbitmap != NULL)
		DeleteObject((*mapbuf)->hbitmap);
	(*mapbuf)->hbitmap = CreateCompatibleBitmap(hDC, (*mapbuf)->width, (*mapbuf)->height);
	SelectObject(hmapdc, (*mapbuf)->hbitmap);
    ipicture->lpVtbl->Render(ipicture, hmapdc, 0, 0, (*mapbuf)->width, (*mapbuf)->height, 0, height, width, -height, NULL);
	ipicture->lpVtbl->Release(ipicture);
	istream->lpVtbl->Release(istream);
	DeleteDC(hmapdc);
	GlobalFree(global);
	CloseHandle(file);
} 

bool LoadPictureFromDB(LPDATABASE database, int datapos, LPIMAGE *mapbuf, HDC hDC) // �����ݿ��ȡͼƬ (*.BMP/*.JPG/*.GIF) 
{
	if(!hDC) hDC = gdc;
	HANDLE file = NULL;
	file = CreateFile(database->basename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(file == INVALID_HANDLE_VALUE){ 
		ErrorMsg("ͼƬ�ļ���ȡʧ�ܣ�");
		printf("Fail to load database, File not exist\n");
		return false;
	} 
	*mapbuf = (LPIMAGE)malloc(sizeof(IMAGE));
	SetFilePointer(file,(database->datapos+datapos)->pos,NULL,FILE_BEGIN); //ƫ����ͼƬ���ݿ�ʼ�� 
	HGLOBAL global = NULL;
	global = GlobalAlloc(GMEM_MOVEABLE, (database->datapos+datapos)->filesize); 
	LPVOID data = NULL;
	data = GlobalLock(global);
	DWORD bytes;
	ReadFile(file, data, (database->datapos+datapos)->filesize, &bytes, NULL);				
	GlobalUnlock(global); 	
	IStream *istream;
	CreateStreamOnHGlobal( global, TRUE,  &istream); 
	IPicture *ipicture; //����һ��IPicture�ӿ�ָ�룬��ʾͼƬ����
	OleLoadPicture( istream,  (database->datapos+datapos)->filesize,  TRUE,  &IID_IPicture,  (LPVOID*)&ipicture);	
	OLE_XSIZE_HIMETRIC width;
	OLE_YSIZE_HIMETRIC height;
	ipicture->lpVtbl->get_Width(ipicture, &width);
	ipicture->lpVtbl->get_Height(ipicture, &height);
	(*mapbuf)->width = 1 + (int)(width / PIXELPERMMX);
	(*mapbuf)->height = 1 + (int)(height / PIXELPERMMY);
	HDC hmapdc;
	hmapdc = CreateCompatibleDC(hDC);
	if ((*mapbuf)->hbitmap != NULL)
		DeleteObject((*mapbuf)->hbitmap);
	(*mapbuf)->hbitmap = CreateCompatibleBitmap(hDC, (*mapbuf)->width, (*mapbuf)->height);
	SelectObject(hmapdc, (*mapbuf)->hbitmap);
    ipicture->lpVtbl->Render(ipicture, hmapdc, 0, 0, (*mapbuf)->width, (*mapbuf)->height, 0, height, width, -height, NULL);
	ipicture->lpVtbl->Release(ipicture);
	istream->lpVtbl->Release(istream);
	DeleteDC(hmapdc);
	GlobalFree(global);
	CloseHandle(file);
} 

bool FreePicture(LPIMAGE *mapbuf) // �ͷ�ͼƬ (*.BMP/*.JPG/*.GIF) 
{
	if(!(*mapbuf)->hbitmap) return false;
	DeleteObject((*mapbuf)->hbitmap);
	free(*mapbuf);
	mapbuf = NULL;
	return true;
}

void DrawPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// ��ʾͼƬ3.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
	StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    /*�ͷ�*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawAvatar(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// ��ʾͷ�� 
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    SetStretchBltMode(hMDC,HALFTONE);
    //SetStretchBltMode(hMDC,COLORONCOLOR);
	StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    /*�ͷ�*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawAreaPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,int oriX,int oriY,int areaX,int areaY,HWND hWnd,HDC hMDC)// ��ʾͼƬָ����Χ2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HDC hTempMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+areaX,y+areaY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    if(!screenX || !screenY){
		StretchBlt(hMDC,x,y,areaX,areaY,hMemoryDC,oriX,oriY,areaX,areaY,SRCCOPY);
	}else{
		HBITMAP hTempBitBMP=CreateCompatibleBitmap(hDC,screenX,screenY); // ��ȡ��߶ȡ���ɫλ����Ŀ�����ֵ�����ɫ��λ��
		HBITMAP hOldTempBitMap=SelectObject(hTempMemoryDC,hTempBitBMP);
		/*����ͼ��*/
		//SetStretchBltMode(hTempMemoryDC,HALFTONE);
		SetStretchBltMode(hTempMemoryDC,COLORONCOLOR);
		StretchBlt(hTempMemoryDC,0,0,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    	StretchBlt(hMDC,x,y,areaX,areaY,hTempMemoryDC,oriX,oriY,areaX,areaY,SRCCOPY);
    	/*�ͷ�*/ 
    	DeleteObject(SelectObject(hTempMemoryDC,hOldTempBitMap));
    	DeleteDC(hTempMemoryDC);
	}
    /*�ͷ�*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawInvertPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// ��ɫ��ʾͼƬ2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
    StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,NOTSRCCOPY);
    /*�ͷ�*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawAlphaPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,char alpha)// ָ��͸����alpha��ʾͼƬ2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    BLENDFUNCTION Alpha={0x00,0,alpha,0x00};
    AlphaBlend(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,Alpha);
    /*�ͷ�*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void Draw32BitPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,char alpha)// ��ʾ32λ��͸��ͨ��ͼƬ2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    BLENDFUNCTION Alpha={0x00,0,alpha,0x01};
    AlphaBlend(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,Alpha);
    /*�ͷ�*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawMaskPicture(LPIMAGE pImage,LPIMAGE pMaskImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// ���ַ���ʾ͸��ͼƬ2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*ȥ�����ֺ�ɫ���֣�������ַ���������ȡANDͼƬȡOR*/ 
    //SetStretchBltMode(hMDC,HALFTONE);
	SetStretchBltMode(hMDC,COLORONCOLOR);
    /*������ͼ�뱳��ͼ��"OR"����  */
    HBITMAP hOldBitMap=SelectObject(hMemoryDC,pMaskImage->hbitmap);
    StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pMaskImage->width,pMaskImage->height,SRCPAINT);
    /*��ǰ��ͼ�뱳��ͼ��"AND"����*/
	SelectObject(hMemoryDC,pImage->hbitmap);
	StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCAND);
    /*�ͷ�*/ 
    SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawKeyColorPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// ɫ�ʷ���ʾ͸��ͼƬ2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
    TransparentBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,MARKED_COLOR);//͸��ɫΪMARKED_COLOR
    /*�ͷ�*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawLimitedPicture(LPIMAGE pImage,LPIMAGE pBackImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// ����͸������ָ�Ϊ����ͼƬ2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMaskDC=CreateCompatibleDC(hDC);
	HDC hImageDC=CreateCompatibleDC(hDC);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	/*������ɫλͼ*/
	HBITMAP hMaskBMP = CreateBitmap(screenX,screenY,1,1,NULL);
    /*�������ֻ�ͼ����*/ 
    HBITMAP hOldMaskBMP = SelectObject(hMaskDC,hMaskBMP);
	HBITMAP hOldBitMap = SelectObject(hImageDC,pImage->hbitmap);
	HBITMAP hOldBackBitMap = SelectObject(hMemoryDC,pBackImage->hbitmap);
	/*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����͸������Ϊ��ɫ����������Ϊ��ɫ������λͼ*/
	SetBkColor(hImageDC,MARKED_COLOR); 
	//SetStretchBltMode(hMaskDC,HALFTONE);
	SetStretchBltMode(hMaskDC,COLORONCOLOR);
    StretchBlt(hMaskDC,0,0,screenX,screenY,hImageDC,0,0,pImage->width,pImage->height,SRCCOPY);
	/*����͸������Ϊ��ɫ���������򱣳ֲ����λͼ*/
	SetBkColor(hMemoryDC,RGB(0,0,0));
	SetTextColor(hMemoryDC,RGB(255,255,255));
	BitBlt(hMemoryDC,x,y,screenX,screenY,hMaskDC,0,0,SRCAND);
	/*͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ*/
	SetBkColor(hMDC,RGB(255,255,255));
	SetTextColor(hMDC,RGB(0,0,0));
	BitBlt(hMDC,x,y,screenX,screenY,hMaskDC,0,0,SRCAND);
	/*"��"����,��������Ч��*/
	BitBlt(hMDC,x,y,screenX,screenY,hMemoryDC,x,y,SRCPAINT);
    /*�ͷ�*/ 
    SelectObject(hImageDC,hOldBitMap);
    SelectObject(hMemoryDC,hOldBackBitMap);
	DeleteObject(SelectObject(hMaskDC,hOldMaskBMP));
    DeleteDC(hMaskDC);
	DeleteDC(hImageDC);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
	//picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

static HBITMAP GetRotatedBitmapNTA(HWND hWnd,HDC hMDC,HBITMAP hBitmap,float radians,COLORREF clrBack);
void RotatePictureA(LPIMAGE pImage,int x,int y,int screenX,int screenY,float degree,HWND hWnd,HDC hMDC)// ��������תͼƬ&ָ������2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hNewBitMap=(HBITMAP)GetRotatedBitmapNTA(hWnd,hMDC,pImage->hbitmap,((float)degree)*M_PI/180,MARKED_COLOR);//��תͼƬ 
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,hNewBitMap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    SetStretchBltMode(hMDC,HALFTONE);
    //SetStretchBltMode(hMDC,COLORONCOLOR);
    TransparentBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,MARKED_COLOR);//��ȥ͸������ 
    /*�ͷ�*/ 
    DeleteObject(SelectObject(hMemoryDC,hOldBitMap));
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

static HBITMAP GetRotatedBitmapNTA(HWND hWnd,HDC hMDC,HBITMAP hBitmap,float radians,COLORREF clrBack)
{
	HDC hDC=GetDC(hWnd);
	HDC destDC=CreateCompatibleDC(hDC);
	HDC sourceDC=CreateCompatibleDC(hDC);
	BITMAP bm;
	GetObject(hBitmap,sizeof(bm),&bm);
	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);
	/*���㶥������*/
	int cx = (int)(bm.bmWidth / 2);
	int cy = (int)(bm.bmHeight / 2);
	int x1 = (int)(bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(-bm.bmWidth * sine);
	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(0,max(x1, max(x2,x3)));
	int maxy = max(0,max(y1, max(y2,y3)));
	/*��������ת*/ 
	int ncx = (maxx+minx) / 2;
	int ncy = (maxy+miny) / 2;
	HBITMAP hbmResult = CreateCompatibleBitmap(hMDC,bm.bmWidth,bm.bmHeight);
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC,hbmResult);
	HBITMAP hbmOldSource = (HBITMAP)SelectObject(sourceDC,hBitmap);
	HBRUSH hbrBack = CreateSolidBrush(clrBack);
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC,hbrBack);
	PatBlt(destDC,0,0,bm.bmWidth,bm.bmHeight,PATCOPY);
	DeleteObject(SelectObject(destDC,hbrOld));
	SetGraphicsMode(destDC,GM_ADVANCED);
	/*��������*/
	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = cx-ncx;
	xform.eDy = cy-ncy;
	SetWorldTransform(destDC,&xform);
	BitBlt(destDC,0,0,bm.bmWidth,bm.bmHeight,sourceDC,0,0,SRCCOPY);
	SelectObject(destDC,hbmOldDest);
	SelectObject(sourceDC,hbmOldSource);
	DeleteObject(hbmOldDest);
    DeleteObject(hbmOldSource);
	DeleteDC(destDC);
	DeleteDC(sourceDC);
	ReleaseDC(hWnd,hDC);
	return hbmResult;
}

static HBITMAP GetRotatedBitmapNTB(HWND hWnd,HDC hMDC,HBITMAP hBitmap,float cosine,float sine,COLORREF clrBack);
void RotatePictureB(LPIMAGE pImage,int x,int y,int screenX,int screenY,float cosine,float sine,HWND hWnd,HDC hMDC)// ��������תͼƬ&��֪���Ǻ��� 2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	if(cosine==-1 && sine==0)
		cosine+=0.0001,sine+=0.0001;
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hNewBitMap=(HBITMAP)GetRotatedBitmapNTB(hWnd,hMDC,pImage->hbitmap,cosine,sine,MARKED_COLOR);//��תͼƬ 
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,hNewBitMap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
    TransparentBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,MARKED_COLOR);//��ȥ͸������ 
    /*�ͷ�*/ 
    DeleteObject(SelectObject(hMemoryDC,hOldBitMap));
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

static HBITMAP GetRotatedBitmapNTB(HWND hWnd,HDC hMDC,HBITMAP hBitmap,float cosine,float sine,COLORREF clrBack)
{
	HDC hDC=GetDC(hWnd);
	HDC destDC=CreateCompatibleDC(hDC);
	HDC sourceDC=CreateCompatibleDC(hDC);
	BITMAP bm;
	GetObject(hBitmap,sizeof(bm),&bm);
	/*���㶥������*/
	int cx = (int)(bm.bmWidth / 2);
	int cy = (int)(bm.bmHeight / 2);
	int x1 = (int)(bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(-bm.bmWidth * sine);
	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(0,max(x1, max(x2,x3)));
	int maxy = max(0,max(y1, max(y2,y3)));
	/*��������ת*/ 
	int ncx = (maxx+minx) / 2;
	int ncy = (maxy+miny) / 2;
	HBITMAP hbmResult = CreateCompatibleBitmap(hMDC,bm.bmWidth,bm.bmHeight);
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC,hbmResult);
	HBITMAP hbmOldSource = (HBITMAP)SelectObject(sourceDC,hBitmap);
	HBRUSH hbrBack = CreateSolidBrush(clrBack);
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC,hbrBack);
	PatBlt(destDC,0,0,bm.bmWidth,bm.bmHeight,PATCOPY);
	DeleteObject(SelectObject(destDC,hbrOld));
	SetGraphicsMode(destDC,GM_ADVANCED);
	/*��������*/
	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = cx-ncx;
	xform.eDy = cy-ncy;
	SetWorldTransform(destDC,&xform);
	BitBlt(destDC,0,0,bm.bmWidth,bm.bmHeight,sourceDC,0,0,SRCCOPY);
	SelectObject(destDC,hbmOldDest);
	SelectObject(sourceDC,hbmOldSource);
	DeleteObject(hbmOldDest);
    DeleteObject(hbmOldSource);
	DeleteDC(destDC);
	DeleteDC(sourceDC);
	ReleaseDC(hWnd,hDC);
	return hbmResult;
}

static int w,h;

static HBITMAP GetRotatedBitmapNTC(HWND hWnd,HDC hMDC,HBITMAP hBitmap,float cosine,float sine,COLORREF clrBack);
void RotatePictureC(LPIMAGE pImage,int x,int y,int screenX,int screenY,float cosine,float sine,HWND hWnd,HDC hMDC)// �ƶ�����תͼƬ&��֪���Ǻ��� 1.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	if(cosine==-1 && sine==0)
		cosine+=0.0001,sine+=0.0001;
	HDC hDC=GetDC(hWnd);
	HDC hTempDC=CreateCompatibleDC(hDC);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hTempBitMap=CreateCompatibleBitmap(hMDC,screenX,screenY);
	HBITMAP hOldTempMap=SelectObject(hTempDC,hTempBitMap);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
	/*����ͼ��*/
    //SetStretchBltMode(hTempDC,HALFTONE);
	SetStretchBltMode(hTempDC,COLORONCOLOR);
    StretchBlt(hTempDC,0,0,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    SelectObject(hTempDC,hOldTempMap);
    DeleteDC(hTempDC);
    /*��ת*/ 
    HBITMAP hNewBitMap=(HBITMAP)GetRotatedBitmapNTC(hWnd,hMDC,hTempBitMap,cosine,sine,MARKED_COLOR);
    DeleteObject(hTempBitMap);
	SelectObject(hMemoryDC,hNewBitMap);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+w,y+h);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    TransparentBlt(hMDC,x,y,w,h,hMemoryDC,0,0,w,h,MARKED_COLOR);//��ȥ͸������ 
    /*�ͷ�*/ 
    DeleteObject(SelectObject(hMemoryDC,hOldBitMap));
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

static HBITMAP GetRotatedBitmapNTC(HWND hWnd,HDC hMDC,HBITMAP hBitmap,float cosine,float sine,COLORREF clrBack)
{
	HDC hDC=GetDC(hWnd);
	HDC destDC=CreateCompatibleDC(hDC);
	HDC sourceDC=CreateCompatibleDC(hDC);
	BITMAP bm;
	GetObject(hBitmap,sizeof(bm),&bm);
	/*���㶥������*/
	int x1 = (int)(bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(-bm.bmWidth * sine);
	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(0,max(x1, max(x2,x3)));
	int maxy = max(0,max(y1, max(y2,y3)));
	/*�ƶ�����ת*/ 
	w= maxx - minx;
	h= maxy - miny;
	HBITMAP hbmResult = CreateCompatibleBitmap(hMDC,w,h);
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC,hbmResult);
	HBITMAP hbmOldSource = (HBITMAP)SelectObject(sourceDC,hBitmap);
	HBRUSH hbrBack = CreateSolidBrush(clrBack);
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC,hbrBack);
	PatBlt(destDC,0,0,w,h,PATCOPY);
	DeleteObject(SelectObject(destDC,hbrOld));
	SetGraphicsMode(destDC,GM_ADVANCED);
	/*��������*/
	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = (float)-minx;
	xform.eDy = (float)-miny;
	SetWorldTransform(destDC,&xform);
	BitBlt(destDC,0,0,bm.bmWidth,bm.bmHeight,sourceDC,0,0,SRCCOPY);
	SelectObject(destDC,hbmOldDest);
	SelectObject(sourceDC,hbmOldSource);
	DeleteObject(hbmOldDest);
    DeleteObject(hbmOldSource);
	DeleteDC(destDC);
	DeleteDC(sourceDC);
	ReleaseDC(hWnd,hDC);
	return hbmResult;
}

void RotatePictureD(LPIMAGE pImage0,LPIMAGE pImage1,int cx,int cy,int d,double cosine,double sine,HWND hWnd,HDC hMDC)// �ƶ�����תͼƬ&��֪���Ǻ��� 2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	int w=pImage0->width;
	int h=pImage0->height;
	if(cosine==-1 && sine==0)
		cosine+=0.0001,sine+=0.0001;
	HBITMAP Bitmap;
	if(sine >= 0){
		Bitmap = pImage1->hbitmap;
	}else{
		Bitmap = pImage0->hbitmap;
	}
	/*����*/
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,Bitmap);
    /*��ת*/ 
	BITMAP bm;
	GetObject(Bitmap,sizeof(bm),&bm);
	/*���㶥������*/
	double x1 = bm.bmHeight * sine;
	double y1 = bm.bmHeight * cosine;
	double x2 = bm.bmWidth * cosine + bm.bmHeight * sine;
	double y2 = bm.bmHeight * cosine - bm.bmWidth * sine;
	double x3 = bm.bmWidth * cosine;
	double y3 = -bm.bmWidth * sine;
	double minx = min(0,min(x1, min(x2,x3)));
	double miny = min(0,min(y1, min(y2,y3)));
	double maxx = max(0,max(x1, max(x2,x3)));
	double maxy = max(0,max(y1, max(y2,y3)));
	/*�ƶ�����ת*/ 
	int X;
	int Y;
	int W= (int)(maxx - minx);
	int H= (int)(maxy - miny);
	/*ͶӰ������*/
	POINT pt[3];
	if(cosine <= 0 && sine >= 0) //��һ���� 
	{ 
		X = (int)(cx - d*cosine - h*sine /2.0);
		Y = (int)(cy - d*sine + h*cosine /2.0 - w*sine);
		/*pt[0].x = 0;
		pt[0].y = (int)w*sine;
		pt[1].x = (int)-w*cosine;
		pt[1].y = 0;
		pt[2].x = (int)h*sine;
		pt[2].y = H;*/
		pt[0].x = W;
		pt[0].y = (int)-h*cosine;
		pt[1].x = (int)h*sine;
		pt[1].y = H;
		pt[2].x = (int)-w*cosine;
		pt[2].y = 0;
	}
	else if(cosine >= 0 && sine >= 0) //�ڶ����� 
	{
		X = (int)(cx - d*cosine - h*sine/2.0 - w*cosine);
		Y = (int)(cy - d*sine - h*cosine/2.0 - w*sine);
		/*pt[0].x = (int)w*cosine;
		pt[0].y = H;
		pt[1].x = 0;
		pt[1].y = (int)h*cosine;
		pt[2].x = W;
		pt[2].y = (int)w*sine;*/
		pt[0].x = (int)h*sine;
		pt[0].y = 0;
		pt[1].x = W;
		pt[1].y = (int)w*sine;
		pt[2].x = 0;
		pt[2].y = (int)h*cosine;
	}
	else if(cosine >= 0 && sine <= 0) //��������
	{
		X = (int)(cx - d*cosine + h*sine/2.0 - w*cosine);
		Y = (int)(cy - d*sine - h*cosine/2.0);
		pt[0].x = W;
		pt[0].y = (int)h*cosine;
		pt[1].x = (int)-h*sine;
		pt[1].y = H;
		pt[2].x = (int)w*cosine;
		pt[2].y = 0;
	}
	else // �������� 
	{
		X = (int)(cx - d*cosine + h*sine/2.0);
		Y = (int)(cy - d*sine + h*cosine/2.0);
		pt[0].x = (int)-h*sine;
		pt[0].y = 0;
		pt[1].x = W;
		pt[1].y = (int)-w*sine;
		pt[2].x = 0;
		pt[2].y = (int)-h*cosine;
	} 
	/*ͶӰ*/
	HDC hTempDC=CreateCompatibleDC(hDC);
	HBITMAP hNewBitMap=CreateCompatibleBitmap(hMDC,W,H);
	HBITMAP hOldTempMap=SelectObject(hTempDC,hNewBitMap);
    HBRUSH hBrush=CreateSolidBrush(MARKED_COLOR);
    RECT rect;
    rect.left=rect.top=0;
    rect.right=rect.left+W;
    rect.bottom=rect.top+H;
    FillRect(hTempDC,&rect,hBrush);
    DeleteObject(hBrush);
    PlgBlt(hTempDC,pt,hMemoryDC,0,0,pImage0->width,pImage0->height,0,0,0);
    /*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,X,Y,X+W,Y+H);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    TransparentBlt(hMDC,X,Y,W,H,hTempDC,0,0,W,H,MARKED_COLOR);//��ȥ͸������ 
    /*�ͷ�*/ 
    SelectObject(hMemoryDC,hOldBitMap);
    DeleteObject(SelectObject(hTempDC,hOldTempMap));
    DeleteDC(hTempDC);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
}

static HBITMAP CopyDCToBitmap(HDC hScrDC,LPRECT lpRect); // ��Ļ������ת��ΪHBITMAP����
static void SaveBitmapToBmp(HBITMAP hBitmap,char* filename); //����HBITMAP����ͼƬ
void SaveScrBmp(char* filename,HWND hWnd) //��ȡ��ǰ��Ļ�ͻ�����ͼ 
{
	if(!hWnd) hWnd=graphicsWindow;
	HDC hDC=GetDC(hWnd);
	RECT bounds;
	GetClientRect(hWnd, &bounds); 
	HBITMAP hbitmap=CopyDCToBitmap(hDC,&bounds);
	SaveBitmapToBmp(hbitmap,filename); 
	DeleteObject(hbitmap);
	ReleaseDC(hWnd,hDC);
	return;
}

void SaveDskBmp(char* filename) //��ȡ��ǰ�����ͼ
{
	HWND desktop = GetDesktopWindow();
	HDC hDC=GetDC(desktop);
	RECT bounds;
    GetWindowRect(desktop, &bounds);
    LPRECT lprt = &bounds;
    HBITMAP hbitmap=CopyDCToBitmap(GetDC(desktop), lprt);
	SaveBitmapToBmp(hbitmap,filename); 
	DeleteObject(hbitmap);
	ReleaseDC(desktop,hDC);
	return;
}

void LoadPictureFromWnd(LPIMAGE *mapbuf,int x,int y,int screenX,int screenY,HWND hWnd) //�ӵ�ǰ��ʾ�������ͼƬ 
{
	if(!hWnd) hWnd=graphicsWindow;
	HDC hDC=GetDC(hWnd);
	RECT bounds;
	SetRect(&bounds,x,y,x+screenX,y+screenY);
	*mapbuf = (LPIMAGE)malloc(sizeof(IMAGE));
	(*mapbuf)->width=screenX;
	(*mapbuf)->height=screenY;
	(*mapbuf)->hbitmap=CopyDCToBitmap(hDC,&bounds);
	ReleaseDC(hWnd,hDC);
	return;
}

static void SaveBitmapToBmp(HBITMAP hBitmap,char* filename) //����HBITMAP����ͼƬ 
{   
	HDC hDC;                    
	int iBits;                //��ǰ�ֱ�����ÿ������ռ�ֽ��� 
	WORD  wBitCount;          //λͼ��ÿ������ռ�ֽ���   
	DWORD dwPaletteSize=0,
		  dwBmBitsSize=0,
		  dwDIBSize=0,
		  dwWritten=0;        //�����ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С��д���ļ��ֽ���
	BITMAP Bitmap;            //λͼ���Խṹ     
	BITMAPFILEHEADER bmfHdr;  //λͼ�ļ�ͷ�ṹ   
	BITMAPINFOHEADER bi;      //λͼ��Ϣͷ�ṹ        
	LPBITMAPINFOHEADER lpbi;  //ָ��λͼ��Ϣͷ�ṹ       
  	HANDLE  fh,
	  		hDib,
			hPal,
			hOldPal=NULL;     //�����ļ��������ڴ�������ɫ����      
	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);   //����λͼ�ļ�ÿ��������ռ�ֽ��� 
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);     
	DeleteDC(hDC);     
	if (iBits <= 1)wBitCount = 1;     
	else if (iBits <= 4) wBitCount = 4;     
	else if (iBits <= 8) wBitCount = 8;     
	else wBitCount = 24;     
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);   
		bi.biSize = sizeof(BITMAPINFOHEADER);   
		bi.biWidth = Bitmap.bmWidth;   
		bi.biHeight = Bitmap.bmHeight;   
		bi.biPlanes = 1;   
		bi.biBitCount = wBitCount;   
		bi.biCompression = BI_RGB;   
		bi.biSizeImage = 0;   
		bi.biXPelsPerMeter = 0;   
		bi.biYPelsPerMeter = 0;   
		bi.biClrImportant = 0;   
		bi.biClrUsed = 0;   
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;     
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));     //Ϊλͼ���ݷ����ڴ�    
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);     
	*lpbi = bi;      
	hPal = GetStockObject(DEFAULT_PALETTE);   //�����ɫ��         
	if(hPal){     
		hDC = GetDC(NULL);       
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);     
		RealizePalette(hDC);     
	}   
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)     
											+dwPaletteSize,(BITMAPINFO *)lpbi, DIB_RGB_COLORS);     //��ȡ�õ�ɫ�����µ�����ֵ     
	if(hOldPal){ //�ָ���ɫ��   
	  SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
	  RealizePalette(hDC);
	  ReleaseDC(NULL, hDC);
	}    
	fh = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);    //����λͼ�ļ�     
    if(fh == INVALID_HANDLE_VALUE) 
		return;
	bmfHdr.bfType = 0x4D42;   //����λͼ�ļ�ͷ  BM
	dwDIBSize   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize   = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits   = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//д��λͼ�ļ���������
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//���
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
    return;
}   

static HBITMAP CopyDCToBitmap(HDC hScrDC,LPRECT lpRect) // ��Ļ������ת��ΪHBITMAP����   
{   
	HDC  hMemDC;                   //   ��Ļ���ڴ��豸������   
	HBITMAP hBitmap,hOldBitmap;    //   λͼ���   
	int  nX, nY, nX2, nY2;         //   ѡ����������   
	int  nWidth, nHeight;          //   λͼ��Ⱥ͸߶�   
	if(IsRectEmpty(lpRect))		   //   ȷ��ѡ������Ϊ�վ���   
		return NULL;   
	//   ���ѡ����������   
	nX   = lpRect->left;   
	nY   = lpRect->top;   
	nX2  = lpRect->right;   
	nY2  = lpRect->bottom;   
	nWidth  = nX2 - nX;   
	nHeight = nY2 - nY; 
	hMemDC = CreateCompatibleDC(hScrDC);                          //   Ϊ��Ļ�豸�����������ݵ��ڴ��豸������   
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);    //   ����һ������Ļ�豸��������ݵ�λͼ,������Ҫ��CreateCompatibleDC���ʹ��    
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);          //   ����λͼѡ���ڴ��豸�������� 
	//   ����Ļ�豸�����������ڴ��豸��������
	//   StretchBlt������Դ�����и���һ��λͼ��Ŀ����Σ���Ҫʱ��ĿǰĿ���豸���õ�ģʽ����ͼ��������ѹ����
	StretchBlt(hMemDC,0,0,nWidth,nHeight,hScrDC,nX,nY,nWidth,nHeight,SRCCOPY);  // SRCCOPY
	//BitBlt(hMemDC,   0,   0,   nWidth,   nHeight,hScrDC,   nX,   nY,   SRCCOPY);   
	//   �õ���Ļλͼ�ľ��     
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);   
	//   ���     
	//DeleteDC(hScrDC);   
	DeleteDC(hMemDC);   
	DeleteObject(hOldBitmap);   
	//   ����λͼ���   
	return hBitmap;   
}

typedef struct{
	int iWidth;
	int iHeight;
	int *array;
} MyMatrix;

static void NewMyMatrix(MyMatrix* stru,int iW ,int iH){
	stru->iWidth=iW;
	stru->iHeight=iH;
	stru->array=(int*)malloc(iW*iH*sizeof(int));
}

static int* GetLine(MyMatrix* stru,int i){
	return stru->array + i * stru->iWidth;
}

static void DeleteMyMatrix(MyMatrix* stru){
	free(stru->array);
	stru->array = NULL;
}

static void fastblur(int *pix,int w,int h,int radius)
{
	if (radius<1) return;
	int wm=w-1;
	int hm=h-1;
	int wh=w*h;
	int div=radius+radius+1;
	int *r=(int*)malloc(wh*sizeof(int));
	int *g=(int*)malloc(wh*sizeof(int));
	int *b=(int*)malloc(wh*sizeof(int));
	int rsum,gsum,bsum,x,y,i,p,yp,yi,yw;
	int *vmin=(int*)malloc(max(w,h)*sizeof(int));
	int divsum=(div+1)>>1;
	divsum*=divsum;
	int *dv=(int*)malloc(256*divsum*sizeof(int));
	for (i=0;i<256*divsum;i++) dv[i]=(i/divsum);
	yw=yi=0;
	MyMatrix stack;
	NewMyMatrix(&stack,3,div);
	int stackpointer;
	int stackstart;
	int *sir;
	int rbs;
	int r1=radius+1;
	int routsum,goutsum,boutsum;
	int rinsum,ginsum,binsum;
	for (y=0;y<h;y++){
		rinsum=ginsum=binsum=routsum=goutsum=boutsum=rsum=gsum=bsum=0;
		for(i=-radius;i<=radius;i++){
			p=pix[yi+min(wm,max(i,0))];
			sir=GetLine(&stack,i+radius);
			sir[0]=(p & 0xFF0000)>>16;
			sir[1]=(p & 0x00FF00)>>8;
			sir[2]=(p & 0x0000FF);
			rbs=r1-abs(i);
			rsum+=sir[0]*rbs;
			gsum+=sir[1]*rbs;
			bsum+=sir[2]*rbs;
			if (i>0){
				rinsum+=sir[0];
				ginsum+=sir[1];
				binsum+=sir[2];
			} else {
				routsum+=sir[0];
				goutsum+=sir[1];
				boutsum+=sir[2];
			}
		}
		stackpointer=radius;
		for (x=0;x<w;x++){
			r[yi]=dv[rsum];
			g[yi]=dv[gsum];
			b[yi]=dv[bsum];
			rsum-=routsum;
			gsum-=goutsum;
			bsum-=boutsum;
			stackstart=stackpointer-radius+div;
			sir=GetLine(&stack,stackstart%div);
			routsum-=sir[0];
			goutsum-=sir[1];
			boutsum-=sir[2];
			if(y==0){
				vmin[x]=min(x+radius+1,wm);
			}
			p=pix[yw+vmin[x]];
			sir[0]=(p & 0xFF0000)>>16;
			sir[1]=(p & 0x00FF00)>>8;
			sir[2]=(p & 0x0000FF);
			rinsum+=sir[0];
			ginsum+=sir[1];
			binsum+=sir[2];
			rsum+=rinsum;
			gsum+=ginsum;
			bsum+=binsum;
			stackpointer=(stackpointer+1)%div;
			sir=GetLine(&stack,(stackpointer)%div);
			routsum+=sir[0];
			goutsum+=sir[1];
			boutsum+=sir[2];
			rinsum-=sir[0];
			ginsum-=sir[1];
			binsum-=sir[2];
			yi++;
		}
		yw+=w;
	}
	for (x=0;x<w;x++){
		rinsum=ginsum=binsum=routsum=goutsum=boutsum=rsum=gsum=bsum=0;
		yp=-radius*w;
		for(i=-radius;i<=radius;i++){
			yi=max(0,yp)+x;
			sir=GetLine(&stack,i+radius);
			sir[0]=r[yi];
			sir[1]=g[yi];
			sir[2]=b[yi];
			rbs=r1-abs(i);
			rsum+=r[yi]*rbs;
			gsum+=g[yi]*rbs;
			bsum+=b[yi]*rbs;
			if (i>0){
				rinsum+=sir[0];
				ginsum+=sir[1];
				binsum+=sir[2];
			} else {
				routsum+=sir[0];
				goutsum+=sir[1];
				boutsum+=sir[2];
			}
			if(i<hm){
				yp+=w;
			}
		}
		yi=x;
		stackpointer=radius;
		for (y=0;y<h;y++){
			pix[yi]=0xFF000000 | (dv[rsum]<<16) | (dv[gsum]<<8) | dv[bsum];
			rsum-=routsum;
			gsum-=goutsum;
			bsum-=boutsum;
			stackstart=stackpointer-radius+div;
			sir=GetLine(&stack,stackstart%div);
			routsum-=sir[0];
			goutsum-=sir[1];
			boutsum-=sir[2];
			if(x==0){
				vmin[y]=min(y+r1,hm)*w;
			}
			p=x+vmin[y];
			sir[0]=r[p];
			sir[1]=g[p];
			sir[2]=b[p];
			rinsum+=sir[0];
			ginsum+=sir[1];
			binsum+=sir[2];
			rsum+=rinsum;
			gsum+=ginsum;
			bsum+=binsum;
			stackpointer=(stackpointer+1)%div;
			sir=GetLine(&stack,stackpointer);
			routsum+=sir[0];
			goutsum+=sir[1];
			boutsum+=sir[2];
			rinsum-=sir[0];
			ginsum-=sir[1];
			binsum-=sir[2];
			yi+=w;
		}
	}
	free(r);
	free(g);
	free(b);
	free(vmin);
	free(dv);
	DeleteMyMatrix(&stack);
}

void DrawBlurPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,int radius)// ͼƬģ�� 
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	/*ͼ�θ�ʽ����*/
	BITMAP bitmap;
	GetObject(pImage->hbitmap,sizeof(BITMAP),&bitmap);
	LPBITMAPINFO lpbmih = (LPBITMAPINFO)malloc(sizeof(BITMAPINFO));
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = screenX;
	lpbmih->bmiHeader.biHeight = screenY;
	lpbmih->bmiHeader.biPlanes = 1;
	lpbmih->bmiHeader.biBitCount = bitmap.bmBitsPixel;
	lpbmih->bmiHeader.biCompression = BI_RGB;
	lpbmih->bmiHeader.biSizeImage = 0;
	lpbmih->bmiHeader.biXPelsPerMeter = 0;
	lpbmih->bmiHeader.biYPelsPerMeter = 0;
	lpbmih->bmiHeader.biClrUsed = 0;
	lpbmih->bmiHeader.biClrImportant = 0;
	/*����λͼ����*/
	BYTE *pBits;
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HDC hTempMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hNewBitMap=CreateDIBSection(hTempMemoryDC,lpbmih,DIB_RGB_COLORS,(void **)&pBits,NULL,0);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
	HBITMAP hOldTempBitMap=SelectObject(hTempMemoryDC,hNewBitMap);
	/*��ȡ��������*/
	SetStretchBltMode(hTempMemoryDC,COLORONCOLOR);
	StretchBlt(hTempMemoryDC,0,0,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
	fastblur((int*)pBits,screenX,screenY,radius);
	/*��ʼ����ͼ����*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*����ͼ��*/
    StretchBlt(hMDC,x,y,screenX,screenY,hTempMemoryDC,0,0,screenX,screenY,SRCCOPY);
    /*�ͷ�*/ 
    free(lpbmih);
    SelectObject(hMemoryDC,hOldBitMap);
    DeleteObject(SelectObject(hTempMemoryDC,hOldTempBitMap));
	DeleteDC(hMemoryDC);
	DeleteDC(hTempMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void MakeBlurPicture(LPIMAGE *mapbuf,LPIMAGE pImage,HWND hWnd,int radius) // ��ͼƬ����ͼƬģ�� 
{ 
	if(!hWnd) hWnd=graphicsWindow;
	int screenX=pImage->width;
	int screenY=pImage->height;
	/*ͼ�θ�ʽ����*/
	BITMAP bitmap;
	GetObject(pImage->hbitmap,sizeof(BITMAP),&bitmap);
	LPBITMAPINFO lpbmih = (LPBITMAPINFO)malloc(sizeof(BITMAPINFO));
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = screenX;
	lpbmih->bmiHeader.biHeight = screenY;
	lpbmih->bmiHeader.biPlanes = 1;
	lpbmih->bmiHeader.biBitCount = bitmap.bmBitsPixel;
	lpbmih->bmiHeader.biCompression = BI_RGB;
	lpbmih->bmiHeader.biSizeImage = 0;
	lpbmih->bmiHeader.biXPelsPerMeter = 0;
	lpbmih->bmiHeader.biYPelsPerMeter = 0;
	lpbmih->bmiHeader.biClrUsed = 0;
	lpbmih->bmiHeader.biClrImportant = 0;
	/*����λͼ����*/
	BYTE *pBits;
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HDC hTempMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hNewBitMap=CreateDIBSection(hTempMemoryDC,lpbmih,DIB_RGB_COLORS,(void **)&pBits,NULL,0);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
	HBITMAP hOldTempBitMap=SelectObject(hTempMemoryDC,hNewBitMap);
	/*��ȡ��������*/
	SetStretchBltMode(hTempMemoryDC,COLORONCOLOR);
	StretchBlt(hTempMemoryDC,0,0,screenX,screenY,hMemoryDC,0,0,screenX,screenY,SRCCOPY);
	fastblur((int*)pBits,screenX,screenY,radius);
    /*�ͷ�*/ 
    free(lpbmih);
    SelectObject(hMemoryDC,hOldBitMap);
    *mapbuf = (LPIMAGE)malloc(sizeof(IMAGE));
	(*mapbuf)->width=screenX;
	(*mapbuf)->height=screenY;
    (*mapbuf)->hbitmap=SelectObject(hTempMemoryDC,hOldTempBitMap);
	DeleteDC(hMemoryDC);
	DeleteDC(hTempMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void MakeAlphaPicture(LPIMAGE *mapbuf,LPIMAGE pImage,HWND hWnd,char alpha)// ����͸��ͼƬ  
{
	if(!hWnd) hWnd=graphicsWindow;
	int screenX=pImage->width;
	int screenY=pImage->height;
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HDC hAlphaMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
	HBITMAP hOldAlphaBitMap=SelectObject(hAlphaMemoryDC,(*mapbuf)->hbitmap);
    /*����ͼ��*/
    BLENDFUNCTION Alpha={0x00,0,alpha,0x00};
    AlphaBlend(hAlphaMemoryDC,0,0,screenX,screenY,hMemoryDC,0,0,screenX,screenY,Alpha);
    /*�ͷ�*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	SelectObject(hAlphaMemoryDC,hOldAlphaBitMap);
	DeleteDC(hMemoryDC);
	DeleteDC(hAlphaMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void MakeSizePicture(LPIMAGE *mapbuf,LPIMAGE pImage,int w,int h,HWND hWnd)// ����ָ����СͼƬ  
{
	if(!hWnd) hWnd=graphicsWindow;
	HDC hDC=GetDC(hWnd);
	HDC hMDC=CreateCompatibleDC(hDC);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	*mapbuf = (LPIMAGE)malloc(sizeof(IMAGE));
	(*mapbuf)->width=w;
	(*mapbuf)->height=h;
	HBITMAP hBitmap=CreateCompatibleBitmap(hDC,w,h);
	HBITMAP hOldBitMap=SelectObject(hMDC,hBitmap);
	HBITMAP hOldMemBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
    /*����ͼ��*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
	StretchBlt(hMDC,0,0,w,h,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    /*�ͷ�*/ 
    SelectObject(hMemoryDC,hOldMemBitMap);
	(*mapbuf)->hbitmap=SelectObject(hMDC,hOldBitMap);
	DeleteDC(hMDC);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void WriteBMP(LPIMAGE image,FILE *p){
	HDC hDC;
	HBITMAP hBitmap = image->hbitmap;                    
	int iBits;                //��ǰ�ֱ�����ÿ������ռ�ֽ��� 
	WORD  wBitCount;          //λͼ��ÿ������ռ�ֽ���   
	DWORD dwPaletteSize=0,
		  dwBmBitsSize=0,
		  dwDIBSize=0,
		  dwWritten=0;        //�����ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С��д���ļ��ֽ���
	BITMAP Bitmap;            //λͼ���Խṹ     
	BITMAPFILEHEADER bmfHdr;  //λͼ�ļ�ͷ�ṹ   
	BITMAPINFOHEADER bi;      //λͼ��Ϣͷ�ṹ        
	LPBITMAPINFOHEADER lpbi;  //ָ��λͼ��Ϣͷ�ṹ       
  	HANDLE  hDib,hPal,hOldPal=NULL;     //��������ڴ�������ɫ����      
	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);   //����λͼ�ļ�ÿ��������ռ�ֽ��� 
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);     
	DeleteDC(hDC);     
	if (iBits <= 1)wBitCount = 1;     
	else if (iBits <= 4) wBitCount = 4;     
	else if (iBits <= 8) wBitCount = 8;     
	else wBitCount = 24;     
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);   
		bi.biSize = sizeof(BITMAPINFOHEADER);   
		bi.biWidth = Bitmap.bmWidth;   
		bi.biHeight = Bitmap.bmHeight;   
		bi.biPlanes = 1;   
		bi.biBitCount = wBitCount;   
		bi.biCompression = BI_RGB;   
		bi.biSizeImage = 0;   
		bi.biXPelsPerMeter = 0;   
		bi.biYPelsPerMeter = 0;   
		bi.biClrImportant = 0;   
		bi.biClrUsed = 0;   
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;     
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));     //Ϊλͼ���ݷ����ڴ�    
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);     
	*lpbi = bi;      
	hPal = GetStockObject(DEFAULT_PALETTE);   //�����ɫ��         
	if(hPal){     
		hDC = GetDC(NULL);       
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);     
		RealizePalette(hDC);     
	}   
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)     
											+dwPaletteSize,(BITMAPINFO *)lpbi, DIB_RGB_COLORS);     //��ȡ�õ�ɫ�����µ�����ֵ     
	if(hOldPal){ //�ָ���ɫ��   
	  SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
	  RealizePalette(hDC);
	  ReleaseDC(NULL, hDC);
	}    
	bmfHdr.bfType = 0x4D42;   //����λͼ�ļ�ͷ  BM
	dwDIBSize   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize   = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits   = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//д��λͼ�ļ�ͷ
	fwrite((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), 1, p);
	//д��λͼ�ļ���������
	fwrite((LPSTR)lpbi, dwDIBSize, 1, p);
	//���
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	return;
}

bool ReadBMP(LPIMAGE *mapbuf,FILE *p,DWORD bytes){
	/*Ĭ��ָ���Ѿ�ƫ����ͼƬ���ݿ�ʼ��*/
	HDC hDC = gdc;
	*mapbuf = (LPIMAGE)malloc(sizeof(IMAGE));
	HGLOBAL global = NULL;
	global = GlobalAlloc(GMEM_MOVEABLE, bytes); 
	LPVOID data = NULL;
	data = GlobalLock(global);
	fread(data,sizeof(char),bytes,p);
	GlobalUnlock(global); 	
	IStream *istream;
	CreateStreamOnHGlobal( global, TRUE,  &istream); 
	IPicture *ipicture; //����һ��IPicture�ӿ�ָ�룬��ʾͼƬ����
	OleLoadPicture( istream,  bytes,  TRUE,  &IID_IPicture,  (LPVOID*)&ipicture);	
	OLE_XSIZE_HIMETRIC width;
	OLE_YSIZE_HIMETRIC height;
	ipicture->lpVtbl->get_Width(ipicture, &width);
	ipicture->lpVtbl->get_Height(ipicture, &height);
	(*mapbuf)->width = 1 + (int)(width / PIXELPERMMX);
	(*mapbuf)->height = 1 + (int)(height / PIXELPERMMY);
	HDC hmapdc;
	hmapdc = CreateCompatibleDC(hDC);
	if ((*mapbuf)->hbitmap != NULL)
		DeleteObject((*mapbuf)->hbitmap);
	(*mapbuf)->hbitmap = CreateCompatibleBitmap(hDC, (*mapbuf)->width, (*mapbuf)->height);
	SelectObject(hmapdc, (*mapbuf)->hbitmap);
    ipicture->lpVtbl->Render(ipicture, hmapdc, 0, 0, (*mapbuf)->width, (*mapbuf)->height, 0, height, width, -height, NULL);
	ipicture->lpVtbl->Release(ipicture);
	istream->lpVtbl->Release(istream);
	DeleteDC(hmapdc);
	GlobalFree(global);
} 
