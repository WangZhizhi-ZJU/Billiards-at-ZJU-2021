#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <graphics.h> 
#include <pictures.h>

#include <error.h>
#include <public.h>//公用宏定义 

#include <olectl.h>
#pragma comment(lib,"Msimg32.lib")

static long long picturetimes = 0;

extern HDC gdc, osdc;
extern HWND graphicsWindow;

extern double PIXELPERMMX;
extern double PIXELPERMMY;

bool LoadPicture(char *filename, LPIMAGE *mapbuf, HDC hDC) // 加载图片 (*.BMP/*.JPG/*.GIF) 
{
	if(!hDC) hDC = gdc;
	HANDLE file = NULL;
	file = CreateFile(				//从指定的路径 filename 中读取文件句柄
			filename, 				//要打开的文件的名或设备名。这个字符串的最大长度在ANSI版本中为MAX_PATH，在unicode版本中为32767。
			GENERIC_READ,			//GENERIC_READ 读访问；GENERIC_WRITE 写访问（可组合使用）；零，表示只允许获取与一个设备有关的信息 。
			FILE_SHARE_READ,		//零 不共享；FILE_SHARE_DELETE只有删除访问请求的权限；FILE_SHARE_READ只有请求读访问的权限；FILE_SHARE_WRITE 只有请求写访问的权限。
			NULL,					//指向一个SECURITY_ATTRIBUTES结构的指针，定义了文件的安全特性（如果操作系统支持的话）
			OPEN_EXISTING,			//CREATE_NEW 创建,存在则出错;CREATE_ALWAYS 创建,改写;OPEN_EXISTING 必须存在;OPEN_ALWAYS 如不存在则创建;TRUNCATE_EXISTING 将现有文件缩短为零长度
			FILE_ATTRIBUTE_NORMAL,	//FILE_ATTRIBUTE_NORMAL 默认属性
			NULL					//通常这个参数设置为NULL，为空表示不使用模板，一般为空。
			);
	if(file == INVALID_HANDLE_VALUE){ 
		ErrorMsg("图片文件读取失败！");
		printf("Fail to load image, File not exist\n");
		return false;
	} 
	*mapbuf = (LPIMAGE)malloc(sizeof(IMAGE));
	DWORD filesize = 0;
	filesize = GetFileSize(file, NULL);	//该函数用于获取指定文件的大小（长度），获取的大小限制在 0xFFFFFFFF(4GB) 以内。若超过则使用 GetFileSizeEx 函数。
	HGLOBAL global = NULL;
	global = GlobalAlloc(GMEM_MOVEABLE, filesize); //该函数从堆中分配一定数目的字节数。GMEM_MOVEABLE 分配可移动的内存,返回内存对象句柄,用函数 GlobalLock 转为指针。 
	LPVOID data = NULL;
	data = GlobalLock(global);
	DWORD bytes;
	ReadFile(			//注：句柄是一种指向指针的指针。 
		 file,			//需要读入数据的文件句柄,这个句柄指向的文件必须是GENERIC_READ 访问属性的文件。 
		 data, 			//接收数据的缓冲区。
		 filesize, 		//指定要读取的字节数。
		 &bytes, 		//指向一个DWORD类型变量的指针，用来接收读取的字节数。如果下一个参数为NULL，那么一定要传入这个参数。
		 NULL);			//OVERLAPPED结构体指针,如果文件是以FILE_FLAG_OVERLAPPED方式打开的话,那么这个指针就不能为NULL。
	GlobalUnlock(global); //GlobalUnlock函数解除锁定的内存块，使指向该内存块的指针无效，GlobalLock锁定的内存，一定要用GlobalUnlock解锁。
	IStream *istream;
	CreateStreamOnHGlobal(		//从指定内存创建流对象。
					global, 	//由GlobalAlloc函数分配的内存句柄。
					TRUE, 		//指明制定的内存在该对象被释放后是否也自动释放。FALSE，必须显式的释放hGlobal。TRUE，hGlobal最终会自动释放。
					&istream); 	//IStream指针的地址，该指针在该函数执行后指向新创建的流对象。该参数不能为NULL。
	IPicture *ipicture; //创建一个IPicture接口指针，表示图片对象
	OleLoadPicture(
			istream, 				//指向包含有图像数据的流的指针。 
			filesize, 				//从流中读取的字节数; 如果应读取整个流，则为零。
			TRUE, 					//图像属性对应的初值 与KeepOriginalFormat属性的初始值相反。如果为TRUE，则将KeepOriginalFormat设置为FALSE，反之亦然。
			&IID_IPicture, 			//引用接口的标识符，该标识符描述要在ppvObj中返回的接口指针的类型。IPicture
			(LPVOID*)&ipicture);	//获得IPicture接口指针
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

bool LoadPictureFromDB(LPDATABASE database, int datapos, LPIMAGE *mapbuf, HDC hDC) // 从数据库读取图片 (*.BMP/*.JPG/*.GIF) 
{
	if(!hDC) hDC = gdc;
	HANDLE file = NULL;
	file = CreateFile(database->basename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(file == INVALID_HANDLE_VALUE){ 
		ErrorMsg("图片文件读取失败！");
		printf("Fail to load database, File not exist\n");
		return false;
	} 
	*mapbuf = (LPIMAGE)malloc(sizeof(IMAGE));
	SetFilePointer(file,(database->datapos+datapos)->pos,NULL,FILE_BEGIN); //偏移至图片数据开始处 
	HGLOBAL global = NULL;
	global = GlobalAlloc(GMEM_MOVEABLE, (database->datapos+datapos)->filesize); 
	LPVOID data = NULL;
	data = GlobalLock(global);
	DWORD bytes;
	ReadFile(file, data, (database->datapos+datapos)->filesize, &bytes, NULL);				
	GlobalUnlock(global); 	
	IStream *istream;
	CreateStreamOnHGlobal( global, TRUE,  &istream); 
	IPicture *ipicture; //创建一个IPicture接口指针，表示图片对象
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

bool FreePicture(LPIMAGE *mapbuf) // 释放图片 (*.BMP/*.JPG/*.GIF) 
{
	if(!(*mapbuf)->hbitmap) return false;
	DeleteObject((*mapbuf)->hbitmap);
	free(*mapbuf);
	mapbuf = NULL;
	return true;
}

void DrawPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// 显示图片3.0
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
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
	StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    /*释放*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawAvatar(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// 显示头像 
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
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    SetStretchBltMode(hMDC,HALFTONE);
    //SetStretchBltMode(hMDC,COLORONCOLOR);
	StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    /*释放*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawAreaPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,int oriX,int oriY,int areaX,int areaY,HWND hWnd,HDC hMDC)// 显示图片指定范围2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HDC hTempMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+areaX,y+areaY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    if(!screenX || !screenY){
		StretchBlt(hMDC,x,y,areaX,areaY,hMemoryDC,oriX,oriY,areaX,areaY,SRCCOPY);
	}else{
		HBITMAP hTempBitBMP=CreateCompatibleBitmap(hDC,screenX,screenY); // 宽度、高度、颜色位面数目、区分单点颜色的位数
		HBITMAP hOldTempBitMap=SelectObject(hTempMemoryDC,hTempBitBMP);
		/*复制图像*/
		//SetStretchBltMode(hTempMemoryDC,HALFTONE);
		SetStretchBltMode(hTempMemoryDC,COLORONCOLOR);
		StretchBlt(hTempMemoryDC,0,0,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    	StretchBlt(hMDC,x,y,areaX,areaY,hTempMemoryDC,oriX,oriY,areaX,areaY,SRCCOPY);
    	/*释放*/ 
    	DeleteObject(SelectObject(hTempMemoryDC,hOldTempBitMap));
    	DeleteDC(hTempMemoryDC);
	}
    /*释放*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawInvertPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// 反色显示图片2.0
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
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
    StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,NOTSRCCOPY);
    /*释放*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawAlphaPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,char alpha)// 指定透明度alpha显示图片2.0
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
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    BLENDFUNCTION Alpha={0x00,0,alpha,0x00};
    AlphaBlend(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,Alpha);
    /*释放*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void Draw32BitPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,char alpha)// 显示32位含透明通道图片2.0
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
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    BLENDFUNCTION Alpha={0x00,0,alpha,0x01};
    AlphaBlend(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,Alpha);
    /*释放*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawMaskPicture(LPIMAGE pImage,LPIMAGE pMaskImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// 遮罩法显示透明图片2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*去掉遮罩黑色部分，如果遮罩反向则遮罩取AND图片取OR*/ 
    //SetStretchBltMode(hMDC,HALFTONE);
	SetStretchBltMode(hMDC,COLORONCOLOR);
    /*将屏蔽图与背景图做"OR"运算  */
    HBITMAP hOldBitMap=SelectObject(hMemoryDC,pMaskImage->hbitmap);
    StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pMaskImage->width,pMaskImage->height,SRCPAINT);
    /*将前景图与背景图做"AND"运算*/
	SelectObject(hMemoryDC,pImage->hbitmap);
	StretchBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCAND);
    /*释放*/ 
    SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawKeyColorPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// 色彩法显示透明图片2.0
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
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
    TransparentBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,MARKED_COLOR);//透明色为MARKED_COLOR
    /*释放*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void DrawLimitedPicture(LPIMAGE pImage,LPIMAGE pBackImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC)// 将非透明区域恢复为背景图片2.0
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
	/*建立单色位图*/
	HBITMAP hMaskBMP = CreateBitmap(screenX,screenY,1,1,NULL);
    /*建立遮罩绘图区域*/ 
    HBITMAP hOldMaskBMP = SelectObject(hMaskDC,hMaskBMP);
	HBITMAP hOldBitMap = SelectObject(hImageDC,pImage->hbitmap);
	HBITMAP hOldBackBitMap = SelectObject(hMemoryDC,pBackImage->hbitmap);
	/*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*生成透明区域为白色，其它区域为黑色的掩码位图*/
	SetBkColor(hImageDC,MARKED_COLOR); 
	//SetStretchBltMode(hMaskDC,HALFTONE);
	SetStretchBltMode(hMaskDC,COLORONCOLOR);
    StretchBlt(hMaskDC,0,0,screenX,screenY,hImageDC,0,0,pImage->width,pImage->height,SRCCOPY);
	/*生成透明区域为黑色，其它区域保持不变的位图*/
	SetBkColor(hMemoryDC,RGB(0,0,0));
	SetTextColor(hMemoryDC,RGB(255,255,255));
	BitBlt(hMemoryDC,x,y,screenX,screenY,hMaskDC,0,0,SRCAND);
	/*透明部分保持屏幕不变，其它部分变成黑色*/
	SetBkColor(hMDC,RGB(255,255,255));
	SetTextColor(hMDC,RGB(0,0,0));
	BitBlt(hMDC,x,y,screenX,screenY,hMaskDC,0,0,SRCAND);
	/*"或"运算,生成最终效果*/
	BitBlt(hMDC,x,y,screenX,screenY,hMemoryDC,x,y,SRCPAINT);
    /*释放*/ 
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
void RotatePictureA(LPIMAGE pImage,int x,int y,int screenX,int screenY,float degree,HWND hWnd,HDC hMDC)// 绕中心旋转图片&指定度数2.0
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hNewBitMap=(HBITMAP)GetRotatedBitmapNTA(hWnd,hMDC,pImage->hbitmap,((float)degree)*M_PI/180,MARKED_COLOR);//旋转图片 
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,hNewBitMap);
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    SetStretchBltMode(hMDC,HALFTONE);
    //SetStretchBltMode(hMDC,COLORONCOLOR);
    TransparentBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,MARKED_COLOR);//扣去透明像素 
    /*释放*/ 
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
	/*计算顶点坐标*/
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
	/*绕中心旋转*/ 
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
	/*矩阵运算*/
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
void RotatePictureB(LPIMAGE pImage,int x,int y,int screenX,int screenY,float cosine,float sine,HWND hWnd,HDC hMDC)// 绕中心旋转图片&已知三角函数 2.0
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
	HBITMAP hNewBitMap=(HBITMAP)GetRotatedBitmapNTB(hWnd,hMDC,pImage->hbitmap,cosine,sine,MARKED_COLOR);//旋转图片 
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,hNewBitMap);
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
    TransparentBlt(hMDC,x,y,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,MARKED_COLOR);//扣去透明像素 
    /*释放*/ 
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
	/*计算顶点坐标*/
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
	/*绕中心旋转*/ 
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
	/*矩阵运算*/
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
void RotatePictureC(LPIMAGE pImage,int x,int y,int screenX,int screenY,float cosine,float sine,HWND hWnd,HDC hMDC)// 绕顶角旋转图片&已知三角函数 1.0
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
	/*缩放图像*/
    //SetStretchBltMode(hTempDC,HALFTONE);
	SetStretchBltMode(hTempDC,COLORONCOLOR);
    StretchBlt(hTempDC,0,0,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    SelectObject(hTempDC,hOldTempMap);
    DeleteDC(hTempDC);
    /*旋转*/ 
    HBITMAP hNewBitMap=(HBITMAP)GetRotatedBitmapNTC(hWnd,hMDC,hTempBitMap,cosine,sine,MARKED_COLOR);
    DeleteObject(hTempBitMap);
	SelectObject(hMemoryDC,hNewBitMap);
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+w,y+h);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    TransparentBlt(hMDC,x,y,w,h,hMemoryDC,0,0,w,h,MARKED_COLOR);//扣去透明像素 
    /*释放*/ 
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
	/*计算顶点坐标*/
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
	/*绕顶角旋转*/ 
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
	/*矩阵运算*/
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

void RotatePictureD(LPIMAGE pImage0,LPIMAGE pImage1,int cx,int cy,int d,double cosine,double sine,HWND hWnd,HDC hMDC)// 绕顶角旋转图片&已知三角函数 2.0
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
	/*创建*/
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,Bitmap);
    /*旋转*/ 
	BITMAP bm;
	GetObject(Bitmap,sizeof(bm),&bm);
	/*计算顶点坐标*/
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
	/*绕顶角旋转*/ 
	int X;
	int Y;
	int W= (int)(maxx - minx);
	int H= (int)(maxy - miny);
	/*投影点坐标*/
	POINT pt[3];
	if(cosine <= 0 && sine >= 0) //第一象限 
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
	else if(cosine >= 0 && sine >= 0) //第二象限 
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
	else if(cosine >= 0 && sine <= 0) //第三象限
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
	else // 第四象限 
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
	/*投影*/
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
    /*初始化绘图区域*/
    RECT r;
    SetRect(&r,X,Y,X+W,Y+H);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    TransparentBlt(hMDC,X,Y,W,H,hTempDC,0,0,W,H,MARKED_COLOR);//扣去透明像素 
    /*释放*/ 
    SelectObject(hMemoryDC,hOldBitMap);
    DeleteObject(SelectObject(hTempDC,hOldTempMap));
    DeleteDC(hTempDC);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
}

static HBITMAP CopyDCToBitmap(HDC hScrDC,LPRECT lpRect); // 屏幕上下文转换为HBITMAP对象
static void SaveBitmapToBmp(HBITMAP hBitmap,char* filename); //保存HBITMAP对象到图片
void SaveScrBmp(char* filename,HWND hWnd) //获取当前屏幕客户区截图 
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

void SaveDskBmp(char* filename) //获取当前桌面截图
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

void LoadPictureFromWnd(LPIMAGE *mapbuf,int x,int y,int screenX,int screenY,HWND hWnd) //从当前显示界面加载图片 
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

static void SaveBitmapToBmp(HBITMAP hBitmap,char* filename) //保存HBITMAP对象到图片 
{   
	HDC hDC;                    
	int iBits;                //当前分辨率下每象素所占字节数 
	WORD  wBitCount;          //位图中每象素所占字节数   
	DWORD dwPaletteSize=0,
		  dwBmBitsSize=0,
		  dwDIBSize=0,
		  dwWritten=0;        //定义调色板大小，位图中像素字节大小，位图文件大小，写入文件字节数
	BITMAP Bitmap;            //位图属性结构     
	BITMAPFILEHEADER bmfHdr;  //位图文件头结构   
	BITMAPINFOHEADER bi;      //位图信息头结构        
	LPBITMAPINFOHEADER lpbi;  //指向位图信息头结构       
  	HANDLE  fh,
	  		hDib,
			hPal,
			hOldPal=NULL;     //定义文件，分配内存句柄，调色板句柄      
	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);   //计算位图文件每个像素所占字节数 
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
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));     //为位图内容分配内存    
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);     
	*lpbi = bi;      
	hPal = GetStockObject(DEFAULT_PALETTE);   //处理调色板         
	if(hPal){     
		hDC = GetDC(NULL);       
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);     
		RealizePalette(hDC);     
	}   
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)     
											+dwPaletteSize,(BITMAPINFO *)lpbi, DIB_RGB_COLORS);     //获取该调色板下新的像素值     
	if(hOldPal){ //恢复调色板   
	  SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
	  RealizePalette(hDC);
	  ReleaseDC(NULL, hDC);
	}    
	fh = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);    //创建位图文件     
    if(fh == INVALID_HANDLE_VALUE) 
		return;
	bmfHdr.bfType = 0x4D42;   //设置位图文件头  BM
	dwDIBSize   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize   = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits   = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//写入位图文件头
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//清除
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
    return;
}   

static HBITMAP CopyDCToBitmap(HDC hScrDC,LPRECT lpRect) // 屏幕上下文转换为HBITMAP对象   
{   
	HDC  hMemDC;                   //   屏幕和内存设备描述表   
	HBITMAP hBitmap,hOldBitmap;    //   位图句柄   
	int  nX, nY, nX2, nY2;         //   选定区域坐标   
	int  nWidth, nHeight;          //   位图宽度和高度   
	if(IsRectEmpty(lpRect))		   //   确保选定区域不为空矩形   
		return NULL;   
	//   获得选定区域坐标   
	nX   = lpRect->left;   
	nY   = lpRect->top;   
	nX2  = lpRect->right;   
	nY2  = lpRect->bottom;   
	nWidth  = nX2 - nX;   
	nHeight = nY2 - nY; 
	hMemDC = CreateCompatibleDC(hScrDC);                          //   为屏幕设备描述表创建兼容的内存设备描述表   
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);    //   创建一个与屏幕设备描述表兼容的位图,函数需要与CreateCompatibleDC配合使用    
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);          //   把新位图选到内存设备描述表中 
	//   把屏幕设备描述表拷贝到内存设备描述表中
	//   StretchBlt函数从源矩形中复制一个位图到目标矩形，必要时按目前目标设备设置的模式进行图像的拉伸或压缩。
	StretchBlt(hMemDC,0,0,nWidth,nHeight,hScrDC,nX,nY,nWidth,nHeight,SRCCOPY);  // SRCCOPY
	//BitBlt(hMemDC,   0,   0,   nWidth,   nHeight,hScrDC,   nX,   nY,   SRCCOPY);   
	//   得到屏幕位图的句柄     
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);   
	//   清除     
	//DeleteDC(hScrDC);   
	DeleteDC(hMemDC);   
	DeleteObject(hOldBitmap);   
	//   返回位图句柄   
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

void DrawBlurPicture(LPIMAGE pImage,int x,int y,int screenX,int screenY,HWND hWnd,HDC hMDC,int radius)// 图片模糊 
{
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	if(!screenX || !screenY){
		screenX=pImage->width;
		screenY=pImage->height;
	}
	/*图形格式参数*/
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
	/*创建位图数据*/
	BYTE *pBits;
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HDC hTempMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hNewBitMap=CreateDIBSection(hTempMemoryDC,lpbmih,DIB_RGB_COLORS,(void **)&pBits,NULL,0);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
	HBITMAP hOldTempBitMap=SelectObject(hTempMemoryDC,hNewBitMap);
	/*获取像素数据*/
	SetStretchBltMode(hTempMemoryDC,COLORONCOLOR);
	StretchBlt(hTempMemoryDC,0,0,screenX,screenY,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
	fastblur((int*)pBits,screenX,screenY,radius);
	/*初始化绘图区域*/
    RECT r;
    SetRect(&r,x,y,x+screenX,y+screenY);
    InvalidateRect(hWnd, &r, false);
    /*复制图像*/
    StretchBlt(hMDC,x,y,screenX,screenY,hTempMemoryDC,0,0,screenX,screenY,SRCCOPY);
    /*释放*/ 
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

void MakeBlurPicture(LPIMAGE *mapbuf,LPIMAGE pImage,HWND hWnd,int radius) // 从图片创建图片模糊 
{ 
	if(!hWnd) hWnd=graphicsWindow;
	int screenX=pImage->width;
	int screenY=pImage->height;
	/*图形格式参数*/
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
	/*创建位图数据*/
	BYTE *pBits;
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HDC hTempMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hNewBitMap=CreateDIBSection(hTempMemoryDC,lpbmih,DIB_RGB_COLORS,(void **)&pBits,NULL,0);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
	HBITMAP hOldTempBitMap=SelectObject(hTempMemoryDC,hNewBitMap);
	/*获取像素数据*/
	SetStretchBltMode(hTempMemoryDC,COLORONCOLOR);
	StretchBlt(hTempMemoryDC,0,0,screenX,screenY,hMemoryDC,0,0,screenX,screenY,SRCCOPY);
	fastblur((int*)pBits,screenX,screenY,radius);
    /*释放*/ 
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

void MakeAlphaPicture(LPIMAGE *mapbuf,LPIMAGE pImage,HWND hWnd,char alpha)// 叠加透明图片  
{
	if(!hWnd) hWnd=graphicsWindow;
	int screenX=pImage->width;
	int screenY=pImage->height;
	HDC hDC=GetDC(hWnd);
	HDC hMemoryDC=CreateCompatibleDC(hDC);
	HDC hAlphaMemoryDC=CreateCompatibleDC(hDC);
	HBITMAP hOldBitMap=SelectObject(hMemoryDC,pImage->hbitmap);
	HBITMAP hOldAlphaBitMap=SelectObject(hAlphaMemoryDC,(*mapbuf)->hbitmap);
    /*复制图像*/
    BLENDFUNCTION Alpha={0x00,0,alpha,0x00};
    AlphaBlend(hAlphaMemoryDC,0,0,screenX,screenY,hMemoryDC,0,0,screenX,screenY,Alpha);
    /*释放*/ 
	SelectObject(hMemoryDC,hOldBitMap);
	SelectObject(hAlphaMemoryDC,hOldAlphaBitMap);
	DeleteDC(hMemoryDC);
	DeleteDC(hAlphaMemoryDC);
	ReleaseDC(hWnd,hDC);
    //picturetimes++;
	//printf("[picturetimes] %ld\n",picturetimes);
    return; 
}

void MakeSizePicture(LPIMAGE *mapbuf,LPIMAGE pImage,int w,int h,HWND hWnd)// 制作指定大小图片  
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
    /*复制图像*/
    //SetStretchBltMode(hMDC,HALFTONE);
    SetStretchBltMode(hMDC,COLORONCOLOR);
	StretchBlt(hMDC,0,0,w,h,hMemoryDC,0,0,pImage->width,pImage->height,SRCCOPY);
    /*释放*/ 
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
	int iBits;                //当前分辨率下每象素所占字节数 
	WORD  wBitCount;          //位图中每象素所占字节数   
	DWORD dwPaletteSize=0,
		  dwBmBitsSize=0,
		  dwDIBSize=0,
		  dwWritten=0;        //定义调色板大小，位图中像素字节大小，位图文件大小，写入文件字节数
	BITMAP Bitmap;            //位图属性结构     
	BITMAPFILEHEADER bmfHdr;  //位图文件头结构   
	BITMAPINFOHEADER bi;      //位图信息头结构        
	LPBITMAPINFOHEADER lpbi;  //指向位图信息头结构       
  	HANDLE  hDib,hPal,hOldPal=NULL;     //定义分配内存句柄，调色板句柄      
	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);   //计算位图文件每个像素所占字节数 
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
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));     //为位图内容分配内存    
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);     
	*lpbi = bi;      
	hPal = GetStockObject(DEFAULT_PALETTE);   //处理调色板         
	if(hPal){     
		hDC = GetDC(NULL);       
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);     
		RealizePalette(hDC);     
	}   
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)     
											+dwPaletteSize,(BITMAPINFO *)lpbi, DIB_RGB_COLORS);     //获取该调色板下新的像素值     
	if(hOldPal){ //恢复调色板   
	  SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
	  RealizePalette(hDC);
	  ReleaseDC(NULL, hDC);
	}    
	bmfHdr.bfType = 0x4D42;   //设置位图文件头  BM
	dwDIBSize   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize   = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits   = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//写入位图文件头
	fwrite((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), 1, p);
	//写入位图文件其余内容
	fwrite((LPSTR)lpbi, dwDIBSize, 1, p);
	//清除
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	return;
}

bool ReadBMP(LPIMAGE *mapbuf,FILE *p,DWORD bytes){
	/*默认指针已经偏移至图片数据开始处*/
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
	IPicture *ipicture; //创建一个IPicture接口指针，表示图片对象
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
