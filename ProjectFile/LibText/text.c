#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#include <graphics.h>
#include <extgraph.h>
#include <text.h>
#include <public.h>

extern double X,Y;

extern HDC gdc; // 主窗体屏幕上下文 
extern HDC osdc; // 主窗体内存上下文 
extern HWND graphicsWindow; // 主窗体句柄 


/*全局字体格式*/ 
static int Point=0;
static char* FontsList[MAX_FONTS+1]; 

bool AddFonts(char *fontspath); //临时安装新字体 
static bool HasFonts(char *fontsname);
void LoadFonts(char *fontsname,char *fontspath,LPFONTS* fonts){
	if(!HasFonts(fontsname))
		if(!AddFonts(fontspath)) {
			printf("Fonts Installation Failed\n");
			ErrorMsg("字体文件安装失败！");
			return;
		}
	LPFONTS lf = (LPFONTS)malloc(sizeof(LOGFONT)); 
    lf->lfHeight         = 200 ; //以逻辑单位指定字体字符元(character cell)或字符的高度,当该参数为0时，字体映射程度将使用一个匹配的默认高度值
    lf->lfWidth          = 0 ; //以逻辑单位指定字体字符的平均宽度,如果lfWidth的值为0，则根据设备的纵横比从可用字体的数字转换纵横中选取最接近的匹配值
    lf->lfEscapement     = 0 ; //以十分之一度为单位指定每一行文本输出时相对于页面底端的角度
    lf->lfOrientation    = 0 ; //以十分之一度为单位指定字符基线相对于页面底端的角度
    lf->lfWeight         = 700 ; //字体的粗细程度。lfWeight的范围为0到1000，正常情况下的字体重量为400，粗体为700。如果lfWeight为0，则使用默认的字体重量。
    lf->lfItalic         = 0 ; //当lfItalic为TRUE时使用斜体
    lf->lfUnderline      = 0 ; //当lfUnderline为TRUE时给字体添加下划线
    lf->lfStrikeOut      = 0 ; //当lfStrikeOut为TRUE时给字体添加删除线
    lf->lfCharSet        = DEFAULT_CHARSET ; //指定字符集,DEFAULT_CHARSET 表示字符集基于本地操作系统
    lf->lfOutPrecision   = 0 ; //指定输出精度,输出与所要求高、宽、方向等接近度,可为：OUT_CHARACTER_PRECIS、OUT_DEFAULT_PRECIS、OUT_STRING_PRECIS、OUT_STROKE_PRECIS
    lf->lfClipPrecision  = 0 ; //指定剪辑精度,超过剪辑区域时对字符的剪辑方式,可为：CLIP_CHARACTER_PRECIS、CLIP_DEFAULT_PRECIS、CLIP_STROKE_PRECIS
    lf->lfQuality        = 0 ; //输出质量,逻辑字体属性到实际的物理字体的所使用的方式，可为：DEFAULT_QUALITY (默认质量)、DRAFT_QUALITY (草稿质量)、PROOF_QUALITY (正稿质量)
    lf->lfPitchAndFamily = 0 ; //
    TCHAR *fn = fontsname;
    lstrcpy(lf->lfFaceName,fn) ; //一个指定以NULL结尾的字符串，它指定的所用的字体名。该字符串的长度不得超过32个字符，如果lfFaceName为NULL，图形设备接口将使用默认的字体名
    *fonts = lf;
}

void PutText(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC){ //绘制字体 
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	lf->lfHeight = height;
	HFONT font = CreateFontIndirect(&(*lf));
    SelectObject(hMDC,font);
    SetTextColor(hMDC,RGB(R,G,B));
    SetBkMode(hMDC,TRANSPARENT); // 透明背景 
    RECT r;
    SetRect(&r,x1,y1,x2,y2);
    /*初始化绘图区域*/
    InvalidateRect(hWnd, &r, false);
    DrawText(hMDC, text, strlen(text), &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER); 
    //TextOut(hMDC,0,0,text,strlen(text));
    DeleteObject(font); 
}

void PutTextB(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC){ //绘制左对齐字体 
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	lf->lfHeight = height;
	HFONT font = CreateFontIndirect(&(*lf));
    SelectObject(hMDC,font);
    SetTextColor(hMDC,RGB(R,G,B));
    SetBkMode(hMDC,TRANSPARENT); // 透明背景 
    RECT r;
    SetRect(&r,x1,y1,x2,y2);
    /*初始化绘图区域*/
    InvalidateRect(hWnd, &r, false);
    DrawText(hMDC, text, strlen(text), &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER); 
    //TextOut(hMDC,0,0,text,strlen(text));
    DeleteObject(font); 
}

void PutTextC(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC){ //绘制右对齐字体 
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	lf->lfHeight = height;
	HFONT font = CreateFontIndirect(&(*lf));
    SelectObject(hMDC,font);
    SetTextColor(hMDC,RGB(R,G,B));
    SetBkMode(hMDC,TRANSPARENT); // 透明背景 
    RECT r;
    SetRect(&r,x1,y1,x2,y2);
    /*初始化绘图区域*/
    InvalidateRect(hWnd, &r, false);
    DrawText(hMDC, text, strlen(text), &r, DT_RIGHT | DT_SINGLELINE | DT_VCENTER); 
    //TextOut(hMDC,0,0,text,strlen(text));
    DeleteObject(font); 
}

void PutTextSide(char* text,double x1,double y1,double x2,double y2,char R,char G,char B,int height,LPFONTS lf,HDC hMDC){ //绘制字体 
	if(!hMDC) hMDC=osdc;
	lf->lfHeight = height;
	HFONT font = CreateFontIndirect(&(*lf));
    SelectObject(hMDC,font);
    SetBkColor(hMDC,RGB(R,G,B));
    SetTextColor(hMDC,RGB(R,G,B));
    SetBkMode(hMDC,TRANSPARENT); // 透明背景 
    RECT r;
    SetRect(&r,PixelsX(x1),PixelsY(y1),PixelsX(x2),PixelsY(y2));
    /*描边效果*/ 
    BeginPath(hMDC);
    DrawText(hMDC, text, strlen(text), &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER); 
    EndPath(hMDC);
    StrokePath(hMDC);
    DeleteObject(font); 
}

static bool CALLBACK EnumFontFamExProc(LPLOGFONT lplf,LPTEXTMETRIC lptm,int iType,LPARAM lpData); // 枚举字体回调函数 
static bool HasFonts(char *fontsname){ //判断该字体是否存在 
	LOGFONT lf;
	HDC hdc = GetDC(NULL);
	int iArrayCount[] = {0,0,0,0};
	memset(&lf,0,sizeof(LOGFONT));
	lf.lfCharSet = DEFAULT_CHARSET; //枚举所有 
	strcpy(lf.lfFaceName,fontsname);
	EnumFontFamiliesEx(hdc,&lf,(FONTENUMPROC)EnumFontFamExProc,(LPARAM)iArrayCount,0); //枚举 
	//EnumFontFamilies(hdc,&lf,(FONTENUMPROC)EnumFontFamEx,(LPARAM)iArrayCount);
	//EnumFonts(hdc,&lf,(FONTENUMPROC)EnumFontFamEx,(LPARAM)iArrayCount);
	//printf("DEVICE_FONTTYPE:%d\n",iArrayCount[0]);
	//printf("RASTER_FONTTYPE:%d\n",iArrayCount[1]);
	//printf("TRUETYPE_FONTTYPE:%d\n",iArrayCount[2]);
	ReleaseDC(NULL,hdc);
	return (iArrayCount[0]+iArrayCount[1]+iArrayCount[2])?true:false;
}

static bool CALLBACK EnumFontFamExProc(LPLOGFONT lplf,LPTEXTMETRIC lptm,int iType,LPARAM lpData){ // 枚举字体回调函数 
	int* iArrayCount;
	iArrayCount = (int*)lpData;
	if(iType & DEVICE_FONTTYPE)          iArrayCount[0]++; //设备支持下载TrueType字体或该字体是一种驻留设备的字体
	else if(iType & RASTER_FONTTYPE)     iArrayCount[1]++; //光栅字体 
	else if(iType & TRUETYPE_FONTTYPE)   iArrayCount[2]++; //TrueType字体
	else                                 iArrayCount[3]++;
	printf("Font: %s\n",lplf->lfFaceName);
	return false; //枚举所有字体 
}

bool AddFonts(char *fontspath){ //临时安装新字体 
	printf("InstallFonts: %s\n",fontspath);
	if(AddFontResource(fontspath)){
		FontsList[++Point]=(char*)malloc((strlen(fontspath)+1)*sizeof(char));
		strcpy(FontsList[Point],fontspath);
		return true;
	}
	return false;
}

void RemoveFonts(){ //卸载临时安装的字体 
	for(;Point;Point--){
		RemoveFontResource(FontsList[Point]);
		printf("正在卸载:%s\n",FontsList[Point]);
		free(FontsList[Point]);
	}
	return;
}

