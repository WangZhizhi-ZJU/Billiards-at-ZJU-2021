#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#include <graphics.h>
#include <extgraph.h>

#include <public.h>
#include <text.h>

extern HCURSOR CS_Cursor; 

static bool CONTROL = false;
static bool TBX_Button = false;

static int boxpoint = 0;
static LPTEXT activetext;
static LPTEXT *textboxtable[MAX_TEXT_BOX + 1]; 

extern HDC gdc; // 主窗体屏幕上下文 
extern HDC osdc; // 主窗体内存上下文 
extern HWND graphicsWindow; // 主窗体句柄 

typedef void (*REDRAWFUNC)();
static REDRAWFUNC func = NULL;

static int MIN(int a,int b){
	return a<b?a:b;
}

static int MAX(int a,int b){
	return a>b?a:b;
}

static int GetBoxID(LPTEXT text){
	if(!boxpoint) 
		return -1;
	int i;
	for(i=1;i<=boxpoint;i++){
		if(*textboxtable[i]==text)
			return i;
	}
	return -1;
}

static void GetCurPos(){
	RECT tr;
	if(activetext->pos){
		HGDIOBJ tempfont = SelectObject(activetext->hMDC,activetext->font);
		DrawText(activetext->hMDC, activetext->text, activetext->pos, &tr, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);
		SelectObject(activetext->hMDC,tempfont);
		activetext->curpos = tr.right - tr.left;
	}
	else{
		activetext->curpos = 0;
	}
}

static void GetPos(int x){
	int p;
	int pos = 0,dpos=abs(activetext->x - x);
	RECT tr;
	HGDIOBJ tempfont = SelectObject(activetext->hMDC,activetext->font);
	for(p=1;p<=strlen(activetext->text);p++){
		if(activetext->text[p-1]<0) p++;
		DrawText(activetext->hMDC, activetext->text, p, &tr, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);
		if(abs(activetext->x + tr.right - tr.left - x) < dpos){
			pos = p;
			dpos = abs(activetext->x + tr.right - tr.left - x);
		}
	}
	activetext->pos = pos;
	SelectObject(activetext->hMDC,tempfont);
}

void RegistRedrawFunc(REDRAWFUNC f){
	func = f;
}

void CancelRedrawFunc(){
	func = NULL;
}

void DrawTextsBox(){ //绘制 
	if(InitCheck()){
		activetext->GetCurPos();
		SetCaretPos(activetext->x + activetext->curpos,activetext->y);
		ShowCaret(activetext->hWnd);
	}
	int i;
	LPTEXT text;
	for(i=1;i<=boxpoint;i++){
		text = *textboxtable[i];
		HGDIOBJ tempfont = SelectObject(text->hMDC,text->font);
	    /*初始化绘图区域*/
	    RECT r,tr;
	    SetRect(&r,text->x,text->y,text->x+text->w,text->y+text->h);
	    InvalidateRect(text->hWnd, &r, false);
	    /*绘制模式*/
	    if(text->in != -1 && text->out != -1){
	    	int pos;
	    	for(pos=0;text->text[pos];){
	    		//printf("%c",text->text[pos]);
	    		if(((pos>=text->in && pos<text->out) || (pos>=text->out && pos<text->in))){
	    			SetTextColor(text->hMDC,0xffffff-text->color);
	    			SetBkColor(text->hMDC,text->color);
					SetBkMode(text->hMDC,OPAQUE); 
				}else{
					SetTextColor(text->hMDC,text->color);
					SetBkMode(text->hMDC,TRANSPARENT); // 透明背景 
				}
				if(text->text[pos] < 0){
					DrawText(text->hMDC, &text->text[pos], 2, &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
					DrawText(text->hMDC, &text->text[pos], 2, &tr, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);
					pos += 2;
					r.left += tr.right - tr.left;
				}else{
					DrawText(text->hMDC, &text->text[pos], 1, &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
					DrawText(text->hMDC, &text->text[pos], 1, &tr, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);
					pos += 1;
					r.left += tr.right - tr.left;
				}
			}
		}else{
			SetTextColor(text->hMDC,text->color);
			SetBkMode(text->hMDC,TRANSPARENT); // 透明背景 
	    	DrawText(text->hMDC, text->text, strlen(text->text), &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER); 
		} 
	    /*恢复*/ 
	    SelectObject(text->hMDC,tempfont);
	}
}

static void Read(char c){ //读入
	static bool flag = false;
	if(!CONTROL && activetext->IsSelect()){
		activetext->Delete();
	}
	int len = strlen(activetext->text);
	if((c >= 0x30 && c <= 0x39
	 || c >= 0x41 && c <= 0x5A
	 || c >= 0x61 && c <= 0x7A)
	 && (len < activetext->len)){
		activetext->text[len + 1] = 0;
		while(len > activetext->pos)
			activetext->text[len] = activetext->text[--len];
		activetext->text[activetext->pos++] = c;
	}
	else if(flag == false && c < 0 && (len + 1 < activetext->len)){
		flag = true;
		activetext->text[len + 1] = 0;
		while(len > activetext->pos)
			activetext->text[len] = activetext->text[--len];
		activetext->text[activetext->pos++] = c;
	}
	else if(flag == true){
		flag = false;
		activetext->text[len + 1] = 0;
		while(len > activetext->pos)
			activetext->text[len] = activetext->text[--len];
		activetext->text[activetext->pos++] = c;
	}
	else return;
}

static void Delete(){ //删除
	if(activetext->IsSelect()){
		int step = 1;
		step = abs(activetext->out - activetext->in);
		activetext->pos = MIN(activetext->in,activetext->out);
		activetext->UnSelect();
		int pos = activetext->pos;
		while (activetext->text[pos]) 
			activetext->text[pos] = activetext->text[step+pos++];
		activetext->text[pos] = 0;
	}
	else if(activetext->pos >= 0){
		if(activetext->text[activetext->pos] < 0){
			int pos = activetext->pos + 1;
			while (activetext->text[pos]) 
				activetext->text[pos] = activetext->text[++pos];
			activetext->text[pos] = 0;
		}
		int pos = activetext->pos;
		while (activetext->text[pos]) 
			activetext->text[pos] = activetext->text[++pos];
		activetext->text[pos] = 0;
	}
	else return;
}

static void Back(){ //删除
	if(activetext->IsSelect()){
		int step = 1;
		step = abs(activetext->out - activetext->in);
		activetext->pos = MIN(activetext->in,activetext->out);
		activetext->UnSelect();
		int pos = activetext->pos;
		while (activetext->text[pos]) 
			activetext->text[pos] = activetext->text[step+pos++];
		activetext->text[pos] = 0;
	}
	else if(activetext->pos > 0){
		if(activetext->text[activetext->pos - 1] < 0){
			int pos = --activetext->pos;
			while (activetext->text[pos]) 
				activetext->text[pos] = activetext->text[++pos];
			activetext->text[pos] = 0;
		}
		int pos = --activetext->pos;
		while (activetext->text[pos]) 
			activetext->text[pos] = activetext->text[++pos];
		activetext->text[pos] = 0;
	}
	else return;
}

static void Home(){
	if(activetext->IsSelect()){
		activetext->UnSelect(); 
	}
	activetext->pos = 0;
}

static void End(){
	if(activetext->IsSelect()){
		activetext->UnSelect(); 
	}
	activetext->pos = strlen(activetext->text);
}

static void Left(){
	if(activetext->IsSelect()){
		activetext->pos = MIN(activetext->in,activetext->out);
		activetext->UnSelect(); 
		return;
	}
	if(activetext->pos > 0){
		if(activetext->text[activetext->pos - 1] < 0){
			--activetext->pos;
		}
		--activetext->pos;
	}
	else return;
}

static void Right(){ 
	if(activetext->IsSelect()){
		activetext->pos = MAX(activetext->in,activetext->out);
		activetext->UnSelect();
		return;
	}
	if(activetext->pos + 1 <= strlen(activetext->text)){
		if(activetext->text[activetext->pos + 1] < 0){
			++activetext->pos;
		}
		++activetext->pos;
	}
	else return;
}

static void GetClip(){
	if(activetext->IsSelect()){
		activetext->Delete();
	}
	int pos;
	int len = strlen(activetext->text);
	HANDLE hClip; //剪切板句柄
	char *pbuf = NULL; //剪贴板数据 
	if(!OpenClipboard(NULL))
		return;
	if(!IsClipboardFormatAvailable(CF_TEXT)){
		CloseClipboard();
		return;
	}
	hClip=GetClipboardData(CF_TEXT);
	pbuf=(char*)GlobalLock(hClip);
	for(pos=0;pbuf[pos] && len<activetext->len; pos++,len++){
		TextBoxChar(pbuf[pos]);
	}
	GlobalUnlock(hClip);
	CloseClipboard();
}

static void SetClip(){
	if(!activetext->IsSelect()){
		//printf("未选择\n");
		return;
	}
    if(!OpenClipboard(NULL))
		return;
    if(!EmptyClipboard()){ /* 清空剪切板 */
    	CloseClipboard();
		return; 
	}
	int i;
	//printf("I:%d O:%d\n",activetext->in,activetext->out);
	int contentSize = abs(activetext->out - activetext->in) + 1;
	char *data = (char*)malloc(sizeof(char)*(contentSize));
	for(i=0;i<contentSize;++i)
		data[i] = activetext->text[MIN(activetext->in,activetext->out) + i];
	data[i] = 0;
	HGLOBAL hMemory;
	LPTSTR lpMemory;
    hMemory = GlobalAlloc(GMEM_MOVEABLE, contentSize);
    lpMemory = (LPTSTR)GlobalLock(hMemory);
    memcpy(lpMemory, data, contentSize);
    SetClipboardData(CF_TEXT, hMemory);
    free(data);
    GlobalUnlock(hMemory); 
    CloseClipboard();
}

static void SelectAll(){
	activetext->End();
	activetext->in = 0;
	activetext->out = strlen(activetext->text);
}

static void UnSelect(){
	activetext->in = -1;
	activetext->out = -1;
}

static bool IsSelect(){
	if(activetext->in != -1 && activetext->out != -1)
		return true;
	return false;
}

static char* GetText(){
	return activetext->text;
}

static void SetText(char* text){
	if(strlen(text) > activetext->len)
		return;
	strcpy(activetext->text,text);
}

void InitSetText(LPTEXT text,char* _text){
	if(strlen(_text) > text->len)
		return;
	strcpy(text->text,_text);
}

bool InitCheck(){
	return activetext?true:false;
}

void ActiveTextBox(LPTEXT text){
	int i;
	for(i=1;i<=boxpoint;i++){
		if((*textboxtable[i]) == text) 
			break;
	}
	if(!boxpoint || i>boxpoint){
		printf("[TBXACT] Wrong\n");
		return;
	}
	text->pos = strlen(text->text);
	CreateCaret(text->hWnd,NULL,2,text->fontsize);
	activetext = text;
}

static void InactiveTextBox(){
	if(!InitCheck()) return;
	HideCaret(activetext->hWnd);
	DestroyCaret();
	if(activetext->IsSelect()){
		activetext->UnSelect();
	}
	activetext = NULL;
	//重绘 
	if(func) func(); 
}

static void ActiveNextTextBox(){
	int ID = GetBoxID(activetext);
	if(ID == -1){
		printf("[TBXNXT] Wrong\n");
		return;
	}
	InactiveTextBox();
	if(ID == boxpoint){
		return;
	}
	ActiveTextBox((*textboxtable[ID + 1]));
}

void RegistTextBox(LPTEXT* text){
	textboxtable[++boxpoint] = text;
}

void CancelTextBox(){
	InactiveTextBox();
	boxpoint = 0;
}

void InitTextBox(LPTEXT* text,int x,int y,int w,int h,int len,int R,int G,int B,LPFONTS font,int fontsize,HWND hWnd,HDC hMDC){
	/*注册*/
	(*text) = (LPTEXT)malloc(sizeof(TEXT));
	/*属性*/
	(*text)->x = x;
	(*text)->y = y;
	(*text)->w = w;
	(*text)->h = h;
	(*text)->len = len;
	(*text)->pos = 0;
	(*text)->size = (int*)calloc(sizeof(int),len+1);
	(*text)->text = (char*)calloc(sizeof(char),len+1);
	(*text)->curpos = 0;
	(*text)->color = RGB(R,G,B);
	(*text)->in = -1;
	(*text)->out = -1;
	if(!hWnd){
		(*text)->hWnd = graphicsWindow;
		(*text)->hMDC = osdc;
	}else{
		(*text)->hWnd = hWnd;
		(*text)->hMDC = hMDC;
	}
	/*字体*/
	font->lfHeight = fontsize;
	(*text)->fontsize = fontsize;
	(*text)->font = CreateFontIndirect(font);
    HGDIOBJ tempfont = SelectObject((*text)->hMDC,(*text)->font);
	GetTextMetrics((*text)->hMDC, &(*text)->metrics);
	SelectObject((*text)->hMDC,tempfont);
	/*方法*/
	(*text)->End = End;
	(*text)->Home = Home;
	(*text)->Read = Read;
	(*text)->Back = Back;
	(*text)->Left = Left;
	(*text)->Right = Right;
	(*text)->Delete = Delete;
	(*text)->GetPos = GetPos;
	(*text)->SetClip = SetClip;
	(*text)->GetClip = GetClip;
	(*text)->SetText = SetText;
	(*text)->GetText = GetText;
	(*text)->UnSelect = UnSelect;
	(*text)->IsSelect = IsSelect;
	(*text)->SelectAll = SelectAll;
	(*text)->GetCurPos = GetCurPos;
}

void FreeTextBox(LPTEXT* text){
	int i;
	for(i=1;i<=boxpoint;i++){
		if(textboxtable[i] == text) 
			return;
	}
	if(i>boxpoint){
		/*字体*/
		DeleteObject((*text)->font);
		/*文本*/
		free((*text)->text);
		free((*text)->size);
		free((*text));
		(*text) == NULL;
	}
}

void TextBoxKeyboard(int key,int event){
	//printf("[KB]%X\n",key);
	if(!InitCheck()) return;
	if(event == KEY_DOWN){
		switch(key){
			case VK_ESCAPE: //0x1B
			case VK_RETURN: //0x0D
				InactiveTextBox();
				ResetCursors();
				return;
			break;
			case VK_TAB:  //0x09
				ActiveNextTextBox();
				return;
			break;
			case VK_LEFT: //0x25
				activetext->Left();
			break;
			case VK_RIGHT: //0x27
				activetext->Right();
			break;
			case VK_BACK: //0x08
				activetext->Back();
			break;
			case VK_DELETE: //0x2E
				activetext->Delete();
			break;
			case VK_CONTROL: //0x11
				CONTROL = true;
			break;
			case VK_END: //0x23
				activetext->End();
			break;
			case VK_HOME: //0x24
				activetext->Home();
			break;
			case 0x41: //A
				if(CONTROL)
					activetext->SelectAll();
			break;
			case 0x43: //C
				if(CONTROL)
					activetext->SetClip();
			break;
			case 0x56: //V
				if(CONTROL)
					activetext->GetClip();
			break;
			case 0x58: //X
				if(CONTROL){
					activetext->SetClip();
					activetext->Delete();
				}
			break;
		}
	}
	else if(event == KEY_UP){
		switch(key){
			case VK_CONTROL: //0x11
				CONTROL = false;
			break;
		}
	}
	//DisplayClear();
	//DrawTextsBox();
	if(func) func(); 
}

void TextBoxMouse(int x, int y, int button, int event){
	if(!InitCheck()) return;
	if((x > activetext->x && x < activetext->x + activetext->w)
	&& (y > activetext->y && y < activetext->y + activetext->h)){
		DestroyCursor(CS_Cursor);
		CS_Cursor = LoadCursor(NULL, IDC_IBEAM);
		SetCursor(CS_Cursor);
	}
	else{
		ResetCursors();
	}
	if(button == LEFT_BUTTON && event == BUTTON_DOWN){
		if((x < activetext->x || x > activetext->x + activetext->w)
		|| (y < activetext->y || y > activetext->y + activetext->h)){
			ResetCursors();
			InactiveTextBox();
			return;
		}
		else{
			TBX_Button = true;
			if(activetext->IsSelect()){
				activetext->UnSelect();
			}
			activetext->GetPos(x);
			activetext->in = activetext->pos;
			//DisplayClear();
			//DrawTextsBox();
			if(func) func(); 
		}
	}
	else if(button == LEFT_BUTTON && event == BUTTON_UP){
		TBX_Button = false;
		activetext->GetPos(x);
		activetext->out = activetext->pos;
		if(activetext->in == activetext->out)
			activetext->UnSelect();
		//DisplayClear();
		//DrawTextsBox();
		if(func) func(); 
	}
	else if(button == MOUSEMOVE && TBX_Button){
		activetext->GetPos(x);
		activetext->out = activetext->pos;
		//DisplayClear();
		//DrawTextsBox();
		if(func) func(); 
	}
}

void TextBoxChar(char c){
	//printf("0x%02X\n",c);
	if(!InitCheck()) return;
	activetext->Read(c);
	//DisplayClear();
	//DrawTextsBox();
	if(func) func(); 
} 
