#ifndef _tooltips_h
#define _tooltips_h

typedef struct{
	HWND hTTWnd;
	HWND hwndParent;
}TOOLTIPS,*LPTOOLTIPS;

typedef void* UID;

_tooltips_h void InitToolTips(LPTOOLTIPS* tooltips,HWND hwndParent);
_tooltips_h void AddToolTips(LPTOOLTIPS tooltips,UID Id,int x,int y,int w,int h,char* Text);
_tooltips_h void DelToolTips(LPTOOLTIPS tooltips,UID Id);
_tooltips_h void DestroyToolTips(LPTOOLTIPS* tooltips);

#endif
