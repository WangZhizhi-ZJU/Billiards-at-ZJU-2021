#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#include <graphics.h>
#include <extgraph.h>
#include <text.h>
#include <public.h>

extern double X,Y;

extern HDC gdc; // ��������Ļ������ 
extern HDC osdc; // �������ڴ������� 
extern HWND graphicsWindow; // �������� 


/*ȫ�������ʽ*/ 
static int Point=0;
static char* FontsList[MAX_FONTS+1]; 

bool AddFonts(char *fontspath); //��ʱ��װ������ 
static bool HasFonts(char *fontsname);
void LoadFonts(char *fontsname,char *fontspath,LPFONTS* fonts){
	if(!HasFonts(fontsname))
		if(!AddFonts(fontspath)) {
			printf("Fonts Installation Failed\n");
			ErrorMsg("�����ļ���װʧ�ܣ�");
			return;
		}
	LPFONTS lf = (LPFONTS)malloc(sizeof(LOGFONT)); 
    lf->lfHeight         = 200 ; //���߼���λָ�������ַ�Ԫ(character cell)���ַ��ĸ߶�,���ò���Ϊ0ʱ������ӳ��̶Ƚ�ʹ��һ��ƥ���Ĭ�ϸ߶�ֵ
    lf->lfWidth          = 0 ; //���߼���λָ�������ַ���ƽ�����,���lfWidth��ֵΪ0��������豸���ݺ�ȴӿ������������ת���ݺ���ѡȡ��ӽ���ƥ��ֵ
    lf->lfEscapement     = 0 ; //��ʮ��֮һ��Ϊ��λָ��ÿһ���ı����ʱ�����ҳ��׶˵ĽǶ�
    lf->lfOrientation    = 0 ; //��ʮ��֮һ��Ϊ��λָ���ַ����������ҳ��׶˵ĽǶ�
    lf->lfWeight         = 700 ; //����Ĵ�ϸ�̶ȡ�lfWeight�ķ�ΧΪ0��1000����������µ���������Ϊ400������Ϊ700�����lfWeightΪ0����ʹ��Ĭ�ϵ�����������
    lf->lfItalic         = 0 ; //��lfItalicΪTRUEʱʹ��б��
    lf->lfUnderline      = 0 ; //��lfUnderlineΪTRUEʱ����������»���
    lf->lfStrikeOut      = 0 ; //��lfStrikeOutΪTRUEʱ���������ɾ����
    lf->lfCharSet        = DEFAULT_CHARSET ; //ָ���ַ���,DEFAULT_CHARSET ��ʾ�ַ������ڱ��ز���ϵͳ
    lf->lfOutPrecision   = 0 ; //ָ���������,�������Ҫ��ߡ�������Ƚӽ���,��Ϊ��OUT_CHARACTER_PRECIS��OUT_DEFAULT_PRECIS��OUT_STRING_PRECIS��OUT_STROKE_PRECIS
    lf->lfClipPrecision  = 0 ; //ָ����������,������������ʱ���ַ��ļ�����ʽ,��Ϊ��CLIP_CHARACTER_PRECIS��CLIP_DEFAULT_PRECIS��CLIP_STROKE_PRECIS
    lf->lfQuality        = 0 ; //�������,�߼��������Ե�ʵ�ʵ������������ʹ�õķ�ʽ����Ϊ��DEFAULT_QUALITY (Ĭ������)��DRAFT_QUALITY (�ݸ�����)��PROOF_QUALITY (��������)
    lf->lfPitchAndFamily = 0 ; //
    TCHAR *fn = fontsname;
    lstrcpy(lf->lfFaceName,fn) ; //һ��ָ����NULL��β���ַ�������ָ�������õ������������ַ����ĳ��Ȳ��ó���32���ַ������lfFaceNameΪNULL��ͼ���豸�ӿڽ�ʹ��Ĭ�ϵ�������
    *fonts = lf;
}

void PutText(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC){ //�������� 
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	lf->lfHeight = height;
	HFONT font = CreateFontIndirect(&(*lf));
    SelectObject(hMDC,font);
    SetTextColor(hMDC,RGB(R,G,B));
    SetBkMode(hMDC,TRANSPARENT); // ͸������ 
    RECT r;
    SetRect(&r,x1,y1,x2,y2);
    /*��ʼ����ͼ����*/
    InvalidateRect(hWnd, &r, false);
    DrawText(hMDC, text, strlen(text), &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER); 
    //TextOut(hMDC,0,0,text,strlen(text));
    DeleteObject(font); 
}

void PutTextB(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC){ //������������� 
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	lf->lfHeight = height;
	HFONT font = CreateFontIndirect(&(*lf));
    SelectObject(hMDC,font);
    SetTextColor(hMDC,RGB(R,G,B));
    SetBkMode(hMDC,TRANSPARENT); // ͸������ 
    RECT r;
    SetRect(&r,x1,y1,x2,y2);
    /*��ʼ����ͼ����*/
    InvalidateRect(hWnd, &r, false);
    DrawText(hMDC, text, strlen(text), &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER); 
    //TextOut(hMDC,0,0,text,strlen(text));
    DeleteObject(font); 
}

void PutTextC(char* text,int x1,int y1,int x2,int y2,char R,char G,char B,int height,LPFONTS lf,HWND hWnd,HDC hMDC){ //�����Ҷ������� 
	if(!hWnd) hMDC=osdc; 
	if(!hWnd) hWnd=graphicsWindow;
	lf->lfHeight = height;
	HFONT font = CreateFontIndirect(&(*lf));
    SelectObject(hMDC,font);
    SetTextColor(hMDC,RGB(R,G,B));
    SetBkMode(hMDC,TRANSPARENT); // ͸������ 
    RECT r;
    SetRect(&r,x1,y1,x2,y2);
    /*��ʼ����ͼ����*/
    InvalidateRect(hWnd, &r, false);
    DrawText(hMDC, text, strlen(text), &r, DT_RIGHT | DT_SINGLELINE | DT_VCENTER); 
    //TextOut(hMDC,0,0,text,strlen(text));
    DeleteObject(font); 
}

void PutTextSide(char* text,double x1,double y1,double x2,double y2,char R,char G,char B,int height,LPFONTS lf,HDC hMDC){ //�������� 
	if(!hMDC) hMDC=osdc;
	lf->lfHeight = height;
	HFONT font = CreateFontIndirect(&(*lf));
    SelectObject(hMDC,font);
    SetBkColor(hMDC,RGB(R,G,B));
    SetTextColor(hMDC,RGB(R,G,B));
    SetBkMode(hMDC,TRANSPARENT); // ͸������ 
    RECT r;
    SetRect(&r,PixelsX(x1),PixelsY(y1),PixelsX(x2),PixelsY(y2));
    /*���Ч��*/ 
    BeginPath(hMDC);
    DrawText(hMDC, text, strlen(text), &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER); 
    EndPath(hMDC);
    StrokePath(hMDC);
    DeleteObject(font); 
}

static bool CALLBACK EnumFontFamExProc(LPLOGFONT lplf,LPTEXTMETRIC lptm,int iType,LPARAM lpData); // ö������ص����� 
static bool HasFonts(char *fontsname){ //�жϸ������Ƿ���� 
	LOGFONT lf;
	HDC hdc = GetDC(NULL);
	int iArrayCount[] = {0,0,0,0};
	memset(&lf,0,sizeof(LOGFONT));
	lf.lfCharSet = DEFAULT_CHARSET; //ö������ 
	strcpy(lf.lfFaceName,fontsname);
	EnumFontFamiliesEx(hdc,&lf,(FONTENUMPROC)EnumFontFamExProc,(LPARAM)iArrayCount,0); //ö�� 
	//EnumFontFamilies(hdc,&lf,(FONTENUMPROC)EnumFontFamEx,(LPARAM)iArrayCount);
	//EnumFonts(hdc,&lf,(FONTENUMPROC)EnumFontFamEx,(LPARAM)iArrayCount);
	//printf("DEVICE_FONTTYPE:%d\n",iArrayCount[0]);
	//printf("RASTER_FONTTYPE:%d\n",iArrayCount[1]);
	//printf("TRUETYPE_FONTTYPE:%d\n",iArrayCount[2]);
	ReleaseDC(NULL,hdc);
	return (iArrayCount[0]+iArrayCount[1]+iArrayCount[2])?true:false;
}

static bool CALLBACK EnumFontFamExProc(LPLOGFONT lplf,LPTEXTMETRIC lptm,int iType,LPARAM lpData){ // ö������ص����� 
	int* iArrayCount;
	iArrayCount = (int*)lpData;
	if(iType & DEVICE_FONTTYPE)          iArrayCount[0]++; //�豸֧������TrueType������������һ��פ���豸������
	else if(iType & RASTER_FONTTYPE)     iArrayCount[1]++; //��դ���� 
	else if(iType & TRUETYPE_FONTTYPE)   iArrayCount[2]++; //TrueType����
	else                                 iArrayCount[3]++;
	printf("Font: %s\n",lplf->lfFaceName);
	return false; //ö���������� 
}

bool AddFonts(char *fontspath){ //��ʱ��װ������ 
	printf("InstallFonts: %s\n",fontspath);
	if(AddFontResource(fontspath)){
		FontsList[++Point]=(char*)malloc((strlen(fontspath)+1)*sizeof(char));
		strcpy(FontsList[Point],fontspath);
		return true;
	}
	return false;
}

void RemoveFonts(){ //ж����ʱ��װ������ 
	for(;Point;Point--){
		RemoveFontResource(FontsList[Point]);
		printf("����ж��:%s\n",FontsList[Point]);
		free(FontsList[Point]);
	}
	return;
}

