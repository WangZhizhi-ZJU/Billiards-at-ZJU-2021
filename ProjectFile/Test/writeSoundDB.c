#include <windows.h>
#include <stdbool.h>
#include <commdlg.h>
#include <stdio.h>

#include <gui.h>
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

#include <test.h>

#define __N 16
static long num = __N;
static char* filename[__N]={
				"Source/$Sound/begin.wav",
				"Source/$Sound/Pause.wav",
				"Source/$Sound/Saved.wav",	
				"Source/$Sound/Recycle.wav",	
				"Source/$Sound/Collide.wav",
				"Source/$Sound/Reflect.wav",
				"Source/$Sound/Inhole.wav",
				"Source/$Sound/OnButton.wav",
				"Source/$Sound/SwitchMenu.wav",
				"Source/$Sound/SwitchSide.wav",
				"Source/$Sound/Quit1.wav",
				"Source/$Sound/Quit2.wav",
				"Source/$Sound/Close1.wav",
				"Source/$Sound/Close2.wav",
				"Source/$Sound/CountDown.wav",
				"Source/$Sound/Win.wav"
				};
static LPDATAPOS image[__N];

bool WriteMusicDB(char *dbname) // 写数据库结构 
{
	int i,j;
	char ch,msg[256]="求是台球(R)音效数据库-v2.0";
	FILE *p,*tp;
	for(i=0;i<num;++i)
		image[i]=(LPDATAPOS)malloc(sizeof(DATAPOS));
	if((p=fopen(dbname,"wb+"))==NULL) return false;
	image[0]->pos=sizeof(char)*256+sizeof(long)+sizeof(DATAPOS)*num;
	for(i=0;i<num;++i){
		tp=fopen(filename[i],"rb");
		fseek(tp,0,SEEK_END);   			//将文件指针移动文件结尾
		image[i]->filesize=ftell(tp); 		//求出当前文件指针距离文件开始的字节数
		if(i!=0) image[i]->pos=image[i-1]->pos+image[i-1]->filesize;
		//rewind(tp);
		fclose(tp);
		printf("[%d] pos:%X size:%X\n",i+1,image[i]->pos,image[i]->filesize);
	}
	printf("[%X]\n",ftell(p)); 
	fwrite(&msg,sizeof(char)*256,1,p);
	printf("[%X]\n",ftell(p)); 
	fwrite(&num,sizeof(long),1,p);
	for(i=0;i<num;++i){
		printf("[%X]\n",ftell(p)); 
		fwrite(image[i],sizeof(DATAPOS),1,p);
	}
	for(i=0;i<num;++i){
		printf("[%X][%d]\n",ftell(p),i);
		tp=fopen(filename[i],"rb");
		for(j=1;j<=image[i]->filesize;j++){
			fread(&ch,sizeof(char),1,tp);
			fwrite(&ch,sizeof(char),1,p);
		}
		printf("size:%X\n",--j);
		fclose(tp); 
	}
	fclose(p);
} 
