#include <stdio.h>
#include <error.h> 

static FILE* pSets;

static char LEN_1[50];

extern int S_Player1_ID;
extern int S_Player2_ID;
extern int BGMVolume;
extern int RefreshRate;
extern float ForceBarSpeed;
extern float ForceBarStep;
extern int ShowCourse;

void InitSets(){
	printf("[Config.ini]\n");
	pSets = fopen("Config.ini","r+");
	if(!pSets){
		ErrorMsg("配置设置文件读取失败！");
		return;
	}
	rewind(pSets);
	fgets(LEN_1,sizeof(LEN_1),pSets);
	fscanf(pSets,"FirstOpen: %d\n",&ShowCourse);
	fscanf(pSets,"Player1_ID: %d\n",&S_Player1_ID);
	fscanf(pSets,"Player2_ID: %d\n",&S_Player2_ID);
	printf("S_Player1_ID: %d\nS_Player2_ID: %d\n", S_Player1_ID, S_Player2_ID);
	fscanf(pSets,"BGM_Volume: %d\n",&BGMVolume);
	printf("BGM_Volume: %d\n",BGMVolume);
	fscanf(pSets,"Refresh_Rate: %d\n", &RefreshRate);
	printf("Refresh_Rate: %d\n", RefreshRate);
	fscanf(pSets,"ForceBar_Speed: %f\n", &ForceBarSpeed);
	if(ForceBarSpeed > 100)
		ForceBarSpeed = 100;
	else if(ForceBarSpeed < 0)
		ForceBarSpeed = 0;
	ForceBarStep = 1.5 + ForceBarSpeed / 40;
	printf("ForceBar_Speed: %.4f\n", ForceBarSpeed);
	rewind(pSets);
}

void ResetSets(){
	rewind(pSets);
	fputs(LEN_1,pSets);
	fprintf(pSets,"FirstOpen: %d\n",ShowCourse);
	fprintf(pSets,"Player1_ID: %d\n",S_Player1_ID);
	fprintf(pSets,"Player2_ID: %d\n",S_Player2_ID);
	printf("S_Player1_ID: %d\nS_Player2_ID: %d\n",S_Player1_ID,S_Player2_ID);
	fprintf(pSets,"BGM_Volume: %d\n",BGMVolume);
	printf("BGM_Volume: %d\n",BGMVolume);
	fprintf(pSets,"Refresh_Rate: %d\n", RefreshRate);
	printf("Refresh_Rate: %d\n", RefreshRate);
	fprintf(pSets,"ForceBar_Speed: %.4f\n", ForceBarSpeed);
	printf("ForceBar_Speed: %.4f\n", ForceBarSpeed);
	ForceBarStep = 1.5 + ForceBarSpeed / 40;
	/*获取文件描述符&强制写硬盘*/
	_commit(_fileno(pSets)); 
	rewind(pSets);
}
