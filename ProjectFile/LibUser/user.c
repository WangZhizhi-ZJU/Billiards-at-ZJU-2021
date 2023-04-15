#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <user.h>
#include <timesys.h>
#include <pictures.h>
#include <openfile.h>

extern LPUSER User1,User2;

static int UserNum = 0;
static LPUSERLIST UserList = NULL, pList = NULL;

extern LPIMAGE IM_Player0;

static LPUSERLIST GetNewNode(){
	/*全新的链表*/
	if(UserList == NULL){
		UserList = (LPUSERLIST)malloc(sizeof(USERLIST));
		UserList->nodeid = ++UserNum;
		UserList->user = (LPUSER)malloc(sizeof(USER));
		UserList->next = NULL;
		UserList->before = NULL;
		pList = UserList;
	} 
	else{
		LPUSERLIST newList = (LPUSERLIST)malloc(sizeof(USERLIST));
		newList->nodeid = ++UserNum;
		newList->user = (LPUSER)malloc(sizeof(USER));
		newList->next = NULL;
		newList->before = pList;
		pList->next = newList;
		pList = newList;
	}
	return pList;
}

static LPUSERLIST FindNode(unsigned int id){
	LPUSERLIST List = UserList;
	for(;List;List=List->next){
		if(List->user->Uid == id)
			return List; 
	}
	printf("[User] Not Found\n");
	ErrorMsg("指定的用户不存在！\n用户数据文件读取失败！");
	return NULL;
}

static void LoadUsers(char* file){
	LPUSERLIST tempNode = GetNewNode();
	LPUSER tempUser = tempNode->user;
	strcpy(tempUser->filename,file);
	tempUser->file = fopen(tempUser->filename,"rb+");
	if(!tempUser->file){
		ErrorMsg("指定的用户不存在！\n用户数据文件读取失败！");
		return;
	}
	fread(&tempUser->Uid,sizeof(tempUser->Uid),1,tempUser->file);
	fread(&tempUser->Name,sizeof(tempUser->Name),1,tempUser->file);
	fread(&tempUser->Level,sizeof(tempUser->Level),1,tempUser->file);
	fread(&tempUser->Exp,sizeof(tempUser->Exp),1,tempUser->file);
	fread(&tempUser->Stick,sizeof(tempUser->Stick),1,tempUser->file);
	fread(&tempUser->ClassicMultiWin,sizeof(tempUser->ClassicMultiWin),1,tempUser->file);
	fread(&tempUser->PoolWin,sizeof(tempUser->PoolWin),1,tempUser->file);
	fread(&tempUser->SnookerWin,sizeof(tempUser->SnookerWin),1,tempUser->file);
	fread(&tempUser->Total,sizeof(tempUser->Total),1,tempUser->file);
	fread(&tempUser->PlayTime,sizeof(tempUser->PlayTime),1,tempUser->file);
	ReadBMP(&tempUser->Avatar,tempUser->file,0xB71F4);
	ReadBMP(&tempUser->BackGround,tempUser->file,0x2A3044);
	rewind(tempUser->file);
}

void SaveUsers(LPUSER tempUser){
	LPUSERLIST tempNode = FindNode(tempUser->Uid);
	tempNode->user = tempUser;
	rewind(tempUser->file);
	LPIMAGE user_tempImage;
	MakeSizePicture(&user_tempImage,tempUser->Avatar,500,500,NULL);
	if(tempUser->Avatar != IM_Player0) 
		FreePicture(&tempUser->Avatar);
	tempUser->Avatar = user_tempImage;
	MakeSizePicture(&user_tempImage,tempUser->BackGround,1280,720,NULL);
	FreePicture(&tempUser->BackGround);
	tempUser->BackGround = user_tempImage;
	fwrite(&tempUser->Uid,sizeof(tempUser->Uid),1,tempUser->file);
	fwrite(&tempUser->Name,sizeof(tempUser->Name),1,tempUser->file);
	fwrite(&tempUser->Level,sizeof(tempUser->Level),1,tempUser->file);
	fwrite(&tempUser->Exp,sizeof(tempUser->Exp),1,tempUser->file);
	fwrite(&tempUser->Stick,sizeof(tempUser->Stick),1,tempUser->file);
	fwrite(&tempUser->ClassicMultiWin,sizeof(tempUser->ClassicMultiWin),1,tempUser->file);
	fwrite(&tempUser->PoolWin,sizeof(tempUser->PoolWin),1,tempUser->file);
	fwrite(&tempUser->SnookerWin,sizeof(tempUser->SnookerWin),1,tempUser->file);
	fwrite(&tempUser->Total,sizeof(tempUser->Total),1,tempUser->file);
	fwrite(&tempUser->PlayTime,sizeof(tempUser->PlayTime),1,tempUser->file);
	printf("%d~",ftell(tempUser->file));
	WriteBMP(tempUser->Avatar,tempUser->file);
	printf("%d\n",ftell(tempUser->file));
	printf("%d~",ftell(tempUser->file));
	WriteBMP(tempUser->BackGround,tempUser->file);
	printf("%d\n",ftell(tempUser->file));
	/*获取文件描述符&强制写硬盘*/
	_commit(_fileno(tempUser->file)); 						
	rewind(tempUser->file);
}

void CreateUsers(LPUSER User0,LPUSER* User){
	LPUSERLIST tempNode = GetNewNode();
	LPUSER tempUser = tempNode->user;
	sprintf(tempUser->filename,"User/User%02d.ud",UserNum);
	tempUser->file = fopen(tempUser->filename,"wb+");
	if(!tempUser->file){
		ErrorMsg("用户数据文件写入失败！");
		return;
	}
	tempUser->Uid = UserNum;
	strcpy(tempUser->Name,User0->Name);
	tempUser->Level = 0;
	tempUser->Exp = 0;
	tempUser->Stick = 0;
	tempUser->ClassicMultiWin = 0;
	tempUser->PoolWin = 0;
	tempUser->SnookerWin = 0;
	tempUser->Total = 0;
	tempUser->PlayTime = 0;
	tempUser->Avatar = User0->Avatar;
	tempUser->BackGround = User0->BackGround;
	printf("[USER%02d]CREATE\n",UserNum);
	SaveUsers(tempUser);
	printf("[USER%02d]FINISH\n",UserNum);
	(*User) = tempUser;
}

static bool FindUsers(){
	long handle; 
	struct _finddata_t fileinfo; 
	char filename[MAX_PATH];
	char file[MAX_PATH] = "User/*.UD";
	/*第一次查找*/
    handle=_findfirst(file,&fileinfo);  
    if(handle==-1) return false;
    /*加载*/
    sprintf(filename,"User/%s",fileinfo.name);
    LoadUsers(filename);
    printf("[USER%02d] %s\n",UserNum,filename);
    /*循环查找*/
    while(!_findnext(handle,&fileinfo)){ 
    	sprintf(filename,"User/%s",fileinfo.name);
		LoadUsers(filename);
		printf("[USER%02d] %s\n",UserNum,filename);
	}
    _findclose(handle); 
    return true;
}

/*玩家信息*/
extern LPUSER User1, User2;

extern bool f_register_1;
extern bool f_register_2;

extern int S_Player1_ID;
extern int S_Player2_ID;

void InitUsers(){
	FindUsers();
	if(S_Player1_ID != 0){
		f_register_1 = true;
		User1 = FindNode(S_Player1_ID)->user;
	}
	else{
		f_register_1 = false;
	}
	if(S_Player2_ID != 0){
		f_register_2 = true;
		User2 = FindNode(S_Player2_ID)->user;
	}
	else{
		f_register_2 = false;
	}
}
