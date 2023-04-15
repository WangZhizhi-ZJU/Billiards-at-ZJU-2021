#include <windows.h>
#include <stdbool.h>
#include <commdlg.h>
#include <stdio.h>
#include <time.h>

#include <gui.h>
#include <user.h>
#include <text.h>
#include <stdio.h>
#include <media.h>
#include <physics.h>
#include <stdbool.h>
#include <pictures.h>
#include <graphics.h>
#include <extgraph.h> 

#include <sets.h>
#include <public.h>
#include <callback.h>

#include <openfile.h> 
#include <error.h> 

#include <test.h>

extern HWND graphicsWindow;

// 返回值: 成功 1, 失败 0
// 通过 path 返回获取的路径
bool FileDialog(char *path,char *title,char *filter)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); // 结构大小
    ofn.lpstrFile = path; // 路径
    ofn.nMaxFile = MAX_PATH; // 路径大小
    ofn.lpstrFilter = filter; // 文件类型
	ofn.lpstrTitle = title; // 窗口标题 
	ofn.hwndOwner = graphicsWindow;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; //OFN_NOCHANGEDIR防止改变相对路径 
	return GetOpenFileName(&ofn);
}

bool FileCopy(char *pSrc, char *pDest)
{
	FILE *fp1 = NULL; //指向源文件
	FILE *fp2 = NULL; //指向目的文件
	char byBuff[1024*2] = {0}; //缓存
	long long fileBytes = 0; //文件大小
	if(pSrc == NULL || pDest == NULL)	return false;
	if((fp1 = fopen(pSrc, "rb")) != NULL){
		if((fp2 = fopen(pDest, "wb+")) != NULL){
			while((fileBytes = fread(byBuff, 1, sizeof(byBuff), fp1)) > 0){
				fwrite(byBuff, fileBytes, 1, fp2);
				memset(byBuff, 0, sizeof(byBuff));
			}
			fclose(fp1);
			fclose(fp2);
			return true;
		}
	}
	return false;
}

#include <winver.h>

bool GetFileVersion(char **version)
{
	char *str=EXE_NAME;
	printf("FileName: [%s]\n",str); // 获取文件名 
	typedef struct{
		WORD wLanguageID;
		WORD wCodePage;
	} LANGUAGE;
	DWORD dwSize = GetFileVersionInfoSize(str,NULL);
	if(0 == dwSize){
		ErrorMsg("游戏版本获取失败！");
		return false;
	} 
	LPBYTE pBlock = (BYTE*)malloc(dwSize),subBlock = (BYTE*)malloc(dwSize);
	GetFileVersionInfo(str,0,dwSize,pBlock);
	char* lpBuffer = NULL;
	UINT dwBytes = 0;
	VerQueryValue(pBlock,"VarFileInfo\\Translation",(LPVOID*)&lpBuffer,&dwBytes); //080404b0为中文
	LANGUAGE lpTranslate = *((LANGUAGE *)lpBuffer);
	printf("Language: %04X%04X\n",lpTranslate.wLanguageID,lpTranslate.wCodePage);
	//获取版本属性
	wsprintf(subBlock,TEXT("StringFileInfo\\%04x%04x\\FileVersion"),lpTranslate.wLanguageID,lpTranslate.wCodePage);
	VerQueryValue(pBlock,subBlock,(LPVOID*)&lpBuffer,&dwBytes);
	printf("FileVersion: %s\n",*version=lpBuffer);
	sprintf(subBlock,TEXT("StringFileInfo\\%04x%04x\\CompanyName"),lpTranslate.wLanguageID,lpTranslate.wCodePage);
	VerQueryValue(pBlock, subBlock, (LPVOID*)&lpBuffer, &dwBytes); 
	printf("CompanyName: %s\n",lpBuffer);
	free(pBlock);
	free(subBlock);
	return true;
}

bool LoadDB(char *databasename, LPDATABASE *data) // 读取数据库结构 
{
	int i;
	FILE *p;
	char msg[256];
	if((p=fopen(databasename,"rb"))==NULL){
		ErrorMsg("数据库文件读取失败！");
		return false;
	} 
	printf("[Loading %s]\n",databasename);
	fread(msg,sizeof(char)*256,1,p);
	printf("[I] %s\n",msg);
	(*data)=(LPDATABASE)malloc(sizeof(DATABASE));
	(*data)->basename=(char*)calloc(sizeof(char),strlen(databasename)+1);
	strcpy((*data)->basename,databasename);
	printf("[S] sgn：%s\n",(*data)->basename);
	fread(&(*data)->num,sizeof(long),1,p);
	printf("[N] num: %ld\n",(*data)->num);
	(*data)->datapos=(LPDATAPOS)calloc(sizeof(DATAPOS),(*data)->num);
	for(i=0;i<(*data)->num;++i){
		fread(((*data)->datapos+i),sizeof(DATAPOS),1,p);
		printf("[%d] pos:%X size:%X\n",i+1,((*data)->datapos+i)->pos,((*data)->datapos+i)->filesize);
	}
	fclose(p);
} 

bool FreeDB(LPDATABASE *data) // 释放数据库结构 
{
	free((*data)->basename);
	(*data)->basename=NULL;
	free((*data)->datapos);
	(*data)->datapos=NULL;
	free((*data));
	(*data)=NULL;
} 

