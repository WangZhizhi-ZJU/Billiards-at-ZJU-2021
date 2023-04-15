#ifndef _save_h
#define _save_h

#include <stdbool.h>

#define SAVE_SIZE 201800

typedef struct{
	bool flag;
	char Time[50];
	char ModeName[20];
	unsigned int Uid1; 
	unsigned int Uid2;
	LPIMAGE IM_Save_Cover; 
	LPIMAGE IM_Save_Blur;
} SAVE,*LPSAVE;

_save_h void SaveSave(int i);
_save_h void ReadSave(int i); 
_save_h void DlteSave(int i);
_save_h void LoadSave(int i,LPSAVE data);

#endif 
