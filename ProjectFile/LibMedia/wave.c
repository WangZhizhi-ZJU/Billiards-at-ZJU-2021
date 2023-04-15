#include <windows.h>
#include <stdbool.h>
#include <mmsystem.h>
#include <process.h>
#include <stdio.h>

#include <callback.h>
#include <public.h>
#include <media.h>

typedef struct{
	/*��ر���*/
	CRITICAL_SECTION waveCriticalSection;
	WAVEHDR* waveBlocks;
	volatile int waveFreeBlockCount;
	int waveCurrentBlock;	
	int BLOCK_COUNT;
	int BLOCK_SIZE; 
} WAVE,*LPWAVE; 

static WAVEHDR* allocateBlocks(int size, int count);
static void freeBlocks(WAVEHDR* blockArray);

static unsigned __stdcall PlayWave(void* data);

static WAVEHDR* allocateBlocks(int size, int count){
	int i;
	WAVEHDR* blocks;
	DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;
	/*�ڴ�����*/
	unsigned char* buffer = (unsigned char*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,totalBufferSize); 
	/*��λ����ָ��*/
	blocks = (WAVEHDR*)buffer;
	buffer += sizeof(WAVEHDR) * count;
	for(i=0;i<count;i++){
		blocks[i].dwBufferLength = size;
		blocks[i].lpData = (LPSTR)buffer;
		buffer += size;
	}
	return blocks;
}

static void freeBlocks(WAVEHDR* blockArray){
	/*�ͷ��ڴ�*/
	HeapFree(GetProcessHeap(), 0, blockArray);
}

static unsigned __stdcall PlayWave(void* data){
	LPMUSIC music = (LPMUSIC)data;
	LPWAVE playdata = (LPWAVE)malloc(sizeof(WAVE));
	/*�豸���*/
	HWAVEOUT hWaveOut; 
	/*��ʼ��*/
	playdata->BLOCK_COUNT = 1;
	playdata->BLOCK_SIZE = music->size;
	playdata->waveBlocks = allocateBlocks(music->size, 1);
	playdata->waveFreeBlockCount = 1;
	playdata->waveCurrentBlock= 0;
	InitializeCriticalSection(&playdata->waveCriticalSection);
	/*����WAVEFORMATEX�ṹ*/
	WAVEFORMATEX wfx;
	wfx.nSamplesPerSec = SAMPLE_RATE; 
	wfx.wBitsPerSample = SAMPLE_SIZE; 
	wfx.nChannels= CHANNELS;
	wfx.cbSize = 0;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	/*��Ĭ������ͨ��*/
	if(waveOutOpen(&hWaveOut,WAVE_MAPPER,&wfx,(DWORD_PTR)GetCurrentThreadId(),(DWORD_PTR)NULL,CALLBACK_THREAD) != MMSYSERR_NOERROR){
		printf("Unable to open wave mapper device\n");
		return;
	}
	//waveOutSetVolume(hWaveOut,0xFFFFFFFF); //��&�� 
	/*����*/
	WAVEHDR* current;
	current = playdata->waveBlocks;
	/*��ʼ��*/
	memcpy(current->lpData, music->data, music->size);
	current->dwBufferLength = music->size;
	waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));
	EnterCriticalSection(&playdata->waveCriticalSection);
	playdata->waveFreeBlockCount--;
	LeaveCriticalSection(&playdata->waveCriticalSection);
	/*��Ϣѭ��*/
	MSG messages; 
	while (GetMessage (&messages, NULL, 0, 0)){
		TranslateMessage(&messages);
        DispatchMessage(&messages);
		if(messages.message == WOM_DONE) break;
    }
	/*�ͷ��ڴ�*/
	waveOutUnprepareHeader(hWaveOut, current, sizeof(WAVEHDR));
	DeleteCriticalSection(&playdata->waveCriticalSection);
	freeBlocks(playdata->waveBlocks);
	waveOutClose(hWaveOut);
	free(playdata);
	_endthreadex(0);
	return;
}

extern HWND TimeSysWindow;
static bool CD = false;

static void SetCastCD(){
	SetTimer(TimeSysWindow,TM_CAST_CD,10,NULL); 
	CD = true;
}

void DelCastCD(){
	CD = false;
}

void CastSound(LPMUSIC music){
	if(CD) return;
	SetCastCD();
	_beginthreadex(NULL,0,PlayWave,music,0,NULL);
}

