#ifndef _user_h
#define _user_h

#include <windows.h>

#include <timesys.h>
#include <pictures.h>

typedef struct{
	unsigned int Uid; 
	char Name[13];
	int Level;
	int Exp;
	int Stick;
	int ClassicMultiWin;
	int PoolWin;
	int SnookerWin;
	int Total;
	time_t PlayTime;
	LPIMAGE Avatar;
	LPIMAGE BackGround;
	FILE* file;
	char filename[MAX_PATH]; 
} USER,*LPUSER;

typedef struct UserNode{
	unsigned int nodeid;
	LPUSER user;
	struct UserNode* next;
	struct UserNode* before;
} USERLIST,*LPUSERLIST;

void InitUsers();
void CreateUsers(LPUSER User0,LPUSER* User);
void SaveUsers(LPUSER tempUser);

#endif
