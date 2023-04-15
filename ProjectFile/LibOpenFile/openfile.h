#ifndef _openfile_h
#define _openfile_h

#include <stdbool.h>
#include <public.h>

bool FileDialog(char *path,char *title,char *filter);

bool FileCopy(char *pSrc, char *pDest);

bool GetFileVersion(char **version);

bool LoadDB(char *databasename, LPDATABASE *data); // 读取数据库结构 
bool FreeDB(LPDATABASE *data); // 释放数据库结构 

#include <save.h>

#endif
