#ifndef _gui_h
#define _gui_h

#include <stdbool.h>

typedef void (*NEXTFUNC)();

void ResetCursors();

void InitWndTitle();
void DrawWndTitle();
void DrawBKGround();
void DrawAllBalls();
void DrawHintMsge();

void DrawStartBtn();
void RsetShakeMnu();
void AddShakePage();
void DrawShakeMnu();
void AddLoadTexts();
void AddLoadPages();
void DrawLoadPage();
void AddMaskPages();
void DrawBlkMasks();
void OriginalSide();
void DrawWinerBtn();

void DrawGmodeBtn();
void DrawLdSavBtn();
void DrawPauseBtn();
void DrawRegstBtn();

void DrawForceBar();
void UpdtForceBar();
void UpdateHitPos();
void DrawStickBar();
void UpdtStickBar();
void TargetPoints();
void DrawACueBall();
void DrawThetaBar();
void HideThetaBar();
void DrawAimLines();

void DrawUsersBtn();
void DrawStickBtn();
void FinishResets();

void ActiveUsrText();
void StartUserMenu();
void EndUserMenu();
void NewUserAvarar(char* file);
void NewUserBackground(char* file);

void StartGetPower();
void EndGetPower();
void AddTheta();
void CutTheta();

void StartGetHit();
void EndGetHit();

void StartFreeResetCueBall();
void EndFreeResetCueBall();

void StartHit();
void EndHit();

void StartStartMenu();
void EndStartMenu();

void StartLoad();
void EndLoad();

void StartMasks(NEXTFUNC fun);
void StartWinMasks(NEXTFUNC fun);
void EndMasks();

void StartGmodeMenu();
void EndGmodeMenu();

void StartLdSavMenu();
void EndLdSavMenu();
void UpdtLdSavMsg();

void StartRegstMenu();
void EndRegstMenu();
void ActiveRegText();
void FinishRegist();
void NewAvarar(char* file);

void PrepareStick();
void SetPrepareMode(bool mode);
void StartPrepare();
void EndPrepare();

void StartPauseMenu(NEXTFUNC fun);
void EndPauseMenu();
void EndPauseMenuWithoutCallBack();
void StartPauseMenuState();
void EndPauseMenuState();

void EndWinMenu();
void StartWinMenu();
void SaveWinPage();

void UpdtSticks();
void StartChooseStick();
void EndChooseStick();

void ShowHintMsge(char* Msg);
void HideHintMsge();

void PlayDlteMusic();
void PlaySaveMusic();
void PlayOnBtMusic();
void PlayQuitMusic();
void PlayClosMusic();
void PlayCtDnMusic();
void PlayWinnMusic();

enum{
	LS_LOAD,
	LS_SAVE,
};

#endif
