#ifndef _openfile_h
#define _openfile_h

#include <stdbool.h>
#include <public.h>

bool FileDialog(char *path,char *title,char *filter);

bool FileCopy(char *pSrc, char *pDest);

bool GetFileVersion(char **version);

bool LoadDB(char *databasename, LPDATABASE *data); // ��ȡ���ݿ�ṹ 
bool FreeDB(LPDATABASE *data); // �ͷ����ݿ�ṹ 

#include <save.h>

#endif
