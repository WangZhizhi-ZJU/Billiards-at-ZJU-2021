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

#define __N (36+10+18+6+10+19+3+19+45+2+4) //172
static long num = __N;
static char* filename[__N]={
				/*台球*/ 
				"Source/$Billiards/Billiards0.bmp",
				"Source/$Billiards/Billiards1.bmp",
				"Source/$Billiards/Billiards2.bmp",
				"Source/$Billiards/Billiards3.bmp",
				"Source/$Billiards/Billiards4.bmp",
				"Source/$Billiards/Billiards5.bmp",
				"Source/$Billiards/Billiards6.bmp",
				"Source/$Billiards/Billiards7.bmp",
				"Source/$Billiards/Billiards8.bmp",
				"Source/$Billiards/Billiards9.bmp",
				"Source/$Billiards/PoolBilliards0.bmp",
				"Source/$Billiards/PoolBilliards1.bmp",
				"Source/$Billiards/PoolBilliards2.bmp",
				"Source/$Billiards/PoolBilliards3.bmp",
				"Source/$Billiards/PoolBilliards4.bmp",
				"Source/$Billiards/PoolBilliards5.bmp",
				"Source/$Billiards/PoolBilliards6.bmp",
				"Source/$Billiards/PoolBilliards7.bmp",
				"Source/$Billiards/PoolBilliards8.bmp",
				"Source/$Billiards/PoolBilliards9.bmp",
				"Source/$Billiards/PoolBilliards10.bmp",
				"Source/$Billiards/PoolBilliards11.bmp",
				"Source/$Billiards/PoolBilliards12.bmp",
				"Source/$Billiards/PoolBilliards13.bmp",
				"Source/$Billiards/PoolBilliards14.bmp",
				"Source/$Billiards/PoolBilliards15.bmp",
				"Source/$Billiards/BilliardsEmoji00.bmp", 
				"Source/$Billiards/BilliardsEmoji01.bmp",
				"Source/$Billiards/BilliardsEmoji02.bmp",
				"Source/$Billiards/BilliardsEmoji03.bmp",
				"Source/$Billiards/BilliardsEmoji04.bmp",
				"Source/$Billiards/BilliardsEmoji05.bmp",
				"Source/$Billiards/BilliardsEmoji06.bmp",
				"Source/$Billiards/BilliardsEmoji07.bmp",
				"Source/$Billiards/BilliardsEmoji08.bmp",
				"Source/$Billiards/BilliardsEmoji09.bmp", 
				/*菜单*/ 
				"Source/$StartMenu/StartMenu00.bmp",
				"Source/$StartMenu/StartMenu01.bmp",
				"Source/$StartMenu/StartMenu02.bmp",
				"Source/$StartMenu/StartMenu03.bmp",
				"Source/$StartMenu/StartMenu04.bmp",
				"Source/$StartMenu/StartMenu10.bmp",
				"Source/$StartMenu/StartMenu11.bmp",
				"Source/$StartMenu/StartMenu12.bmp",
				"Source/$StartMenu/StartMenu13.bmp",
				"Source/$StartMenu/StartMenu14.bmp",
				/*标题栏*/
				"Source/$TitleButton/CloseButton0.bmp",
				"Source/$TitleButton/CloseButton1.bmp",
				"Source/$TitleButton/CloseButton2.bmp",
				"Source/$TitleButton/MaximizeButton0.bmp",
				"Source/$TitleButton/MinimizeButton0.bmp",
				"Source/$TitleButton/MinimizeButton1.bmp",
				"Source/$TitleButton/MinimizeButton2.bmp",
				"Source/$TitleButton/InfoButton0.bmp",
				"Source/$TitleButton/InfoButton1.bmp",
				"Source/$TitleButton/InfoButton2.bmp",
				"Source/$TitleButton/SettingButton0.bmp",
				"Source/$TitleButton/SettingButton1.bmp",
				"Source/$TitleButton/SettingButton2.bmp",
				"Source/$TitleButton/GameButton0.bmp",
				"Source/$TitleButton/GameButton1.bmp",
				"Source/$TitleButton/GameButton2.bmp",
				"Source/$TitleButton/Split.bmp",
				"Source/$TitleButton/Icon.bmp",
				/*提示*/
				"Source/$Reminder/Violation.bmp",
				"Source/$Reminder/Target.bmp",
				"Source/$Reminder/BilliardsColour.bmp",
				"Source/$Reminder/BlackSide.bmp",
				"Source/$Reminder/StripSide.bmp",
				"Source/$Reminder/SolidSide.bmp",
				/*游戏模式*/
				"Source/$Mode/Pool.jpg",
				"Source/$Mode/PoolBlur.jpg",
				"Source/$Mode/Practice.jpg",
				"Source/$Mode/PracticeBlur.jpg",
				"Source/$Mode/AI.jpg",
				"Source/$Mode/AIBlur.jpg",
				"Source/$Mode/ClassicalNineBalls.jpg",
				"Source/$Mode/ClassicalNineBallsBlur.jpg",
				"Source/$Mode/Snooker.jpg",
				"Source/$Mode/SnookerBlur.jpg",
				/*球杆素材*/
				"Source/$Sticks/stick.bmp",
				"Source/$Sticks/stick00.bmp",
				"Source/$Sticks/stick01.bmp",
				"Source/$Sticks/stick10.bmp",
				"Source/$Sticks/stick11.bmp",
				"Source/$Sticks/stick20.bmp",
				"Source/$Sticks/stick21.bmp",
				"Source/$Sticks/stick30.bmp",
				"Source/$Sticks/stick31.bmp",
				"Source/$Sticks/stick40.bmp",
				"Source/$Sticks/stick41.bmp",
				"Source/$Sticks/stick50.bmp",
				"Source/$Sticks/stick51.bmp",
				"Source/$Sticks/stick60.bmp",
				"Source/$Sticks/stick61.bmp",
				"Source/$Sticks/stick70.bmp",
				"Source/$Sticks/stick71.bmp",
				"Source/$Sticks/stick80.bmp",
				"Source/$Sticks/stick81.bmp",
				/*球桌*/
				"Source/$Game/Table.BMP",
				"Source/$Game/ForceBar.BMP",
				"Source/$Game/RedPoint.bmp",
				/*菜单*/
				"Source/$Menu/Loading.bmp",
				"Source/$Menu/LoadingBar.bmp",
				"Source/$Menu/Masks0.bmp",
				"Source/$Menu/Masks1.bmp",
				"Source/$Menu/Choice.bmp",
				"Source/$Menu/Begin0.bmp",
				"Source/$Menu/Begin1.bmp",
				"Source/$Menu/Begin2.bmp",
				"Source/$Menu/LoadSave1.bmp",
				"Source/$Menu/LoadSave2.bmp",
				"Source/$Menu/LoadSave3.bmp",
				"Source/$Menu/LoadSave4.bmp",
				"Source/$Menu/LoadSave5.bmp",
				"Source/$Menu/Register.bmp",
				"Source/$Menu/CourseBK0.bmp",
				"Source/$Menu/CourseBK1.bmp",
				"Source/$Menu/CourseBK2.bmp",
				"Source/$Menu/CourseBK3.bmp",
				"Source/$Menu/SettingBK.bmp",
				/*按钮*/
				"Source/$Button/GreyBar.bmp",
				"Source/$Button/WhiteBar.bmp",
				"Source/$Button/Frame.bmp",
				"Source/$Button/StartGameButton0.bmp",
				"Source/$Button/StartGameButton1.bmp",
				"Source/$Button/GameModeBK00.bmp",
				"Source/$Button/GameModeBK10.bmp",
				"Source/$Button/GameModeButton00.bmp",
				"Source/$Button/GameModeEscButton0.bmp",
				"Source/$Button/PauseButton0.bmp",
				"Source/$Button/PauseContinue.bmp",
				"Source/$Button/PauseSave.bmp",
				"Source/$Button/PauseQuit.bmp",
				"Source/$Button/SetTheta.bmp",
				"Source/$Button/ExpBar0.bmp",
				"Source/$Button/ExpBar1.bmp",
				"Source/$Button/TextBar.bmp",
				"Source/$Button/RegisterOKButton0.bmp",
				"Source/$Button/Edit.bmp",
				"Source/$Button/Eagle.bmp",
				"Source/$Button/Cloth.bmp",
				"Source/$Button/ScoreBoard.bmp",
				"Source/$Button/Individual.bmp",
				"Source/$Button/AvatarBack.bmp",
				"Source/$Button/UserButton.bmp",
				"Source/$Button/SavePageBtn.bmp",
				"Source/$Button/SaveBtn0.bmp",
				"Source/$Button/SaveBtn1.bmp",
				"Source/$Button/SaveBtn2.bmp",
				"Source/$Button/SaveDel.bmp",
				"Source/$Button/SaveLoad.bmp",
				"Source/$Button/SaveSave.bmp",
				"Source/$Button/Crown.bmp",
				"Source/$Button/WinBtn0.bmp",
				"Source/$Button/WinBtn1.bmp",
				"Source/$Button/Select.bmp",
				"Source/$Button/UnSelect.bmp",
				"Source/$Button/Play.bmp",
				"Source/$Button/Pause.bmp",
				"Source/$Button/Right.bmp",
				"Source/$Button/Left.bmp",
				"Source/$Button/SettingEnter.bmp",
				"Source/$Button/SeekBar.bmp",
				"Source/$Button/SeekBarCover.bmp",
				"Source/$Button/SeekBarDot.bmp",
				/*内置头像*/
				"Source/$Avatar/Player0.BMP",
				"Source/$Avatar/AI.BMP",
				/*开发者相关*/
				"Source/$InternalSource/Developers.bmp",
				"Source/$InternalSource/Music.bmp",
				"Source/$InternalSource/Exit.bmp",
				"Source/$InternalSource/Welcome.bmp"
				};
static LPDATAPOS image[__N];

bool WriteBilliardsDB(char *dbname) // 写数据库结构 
{
	int i,j;
	char ch,msg[256]="求是台球(R)图片数据库-v4.0";
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
