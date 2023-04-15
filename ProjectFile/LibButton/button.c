#include <stdio.h> 
#include <stdbool.h>
#include <windows.h>

#include <button.h>
#include <graphics.h>

static int x, int y, int button, int event;

void InitButton(int _x, int _y, int _button, int _event){
	x = _x;
	y = _y;
	button = _button;
	event = _event;
}

bool OnButton(LPBUTTON button,int left,int top,int right,int bottom){
	int flag;
	if(x>1238 && x<1266){
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
				TBT_button=1;
			if(!TBT_button) TBT_close=1; else TBT_close=2;
			if(button == LEFT_BUTTON && event == BUTTON_UP){
				ResetCursors();
				ExitWindow();//¹Ø±ÕÈ·ÈÏ 
				TBT_close=0;
			}
		}
}

