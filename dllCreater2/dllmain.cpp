// dllmain.cpp : 定義 DLL 應用程式的進入點。
#include "windows.h"
#include <stdio.h> 
#include "pch.h"
#include <cstdio>

//製作dll，是一個dll檔
DWORD hack_thread(PVOID pram)
{
	HMODULE game_base = GetModuleHandle(0);//取得 game_image base (必須要在dll中)

	char Buff[100] = { 0 };
	OutputDebugStringA("hello");
	printf("Find Game Windows Hwnd\n");
	DWORD game_pid = NULL;
	HWND hWnd = FindWindow(NULL, L"Plants vs. Zombies");
	GetWindowThreadProcessId(hWnd, &game_pid);//hwnd to PID
	printf("game_pid:%d\n", game_pid);
	DWORD game_image_base = GetWindowLongPtr(hWnd, GWLP_HINSTANCE);// Hwnd to 遊戲 image address
	printf("game_image_base:0x%x\n", game_image_base);

	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, game_pid);
	if (game_base)
	{
		while (TRUE)
		{
			//ppj2dd.exe + 12B948
			//DWORD bullet = ((DWORD)game_base + 768+ 5560); //讀取到子彈的記憶體位置(game_base + offset)
			DWORD dwSunny = 0;//陽光記憶體位置
			SIZE_T dwReaded = 0;
			DWORD dwOffset1 = 0x00000768; //指向第一級指標的 offset
			DWORD dwOffset2 = 0x00005560; //指向第二級指標的 offset
			DWORD target_addr = game_image_base + 0x002a9f38;//欲讀取的記憶體位置
			ReadProcessMemory(pHandle, (LPVOID)target_addr, &dwSunny, 4, &dwReaded);
			target_addr = dwSunny + dwOffset1;//加上第一級指標 offset
			ReadProcessMemory(pHandle, (LPVOID)target_addr, &dwSunny, 4, &dwReaded);
			target_addr = dwSunny + dwOffset2;//加上第二級指標 offset
			ReadProcessMemory(pHandle, (LPVOID)target_addr, &dwSunny, 4, &dwReaded);
			DWORD bullet = target_addr;
			SIZE_T dwWrited = 0;
			WriteProcessMemory(pHandle, (LPVOID)target_addr, &dwSunny, 4, &dwWrited);
			//DWORD bullet2p = ((DWORD)game_base + 0x12B8F8);//2p(鍵盤)
			sprintf_s(Buff, "sun target addr:0x%x", (DWORD)bullet);
			OutputDebugStringA(Buff);
			sprintf_s(Buff, "sun value:0x%x", *(DWORD*)bullet);
			OutputDebugStringA(Buff);

			*(DWORD*)bullet = 999;//寫入的數值(子彈數)
			Sleep(200);//不要讓遊戲太忙,0.2秒寫入一次
		}
	}

	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH://當Dll 被載入時觸發
		/*
			HANDLE CreateThread(
			LPSECURITY_ATTRIBUTES   lpThreadAttributes, //SECURITY_ATTRIBUTES 結構 , 預設填0即可
			SIZE_T                  dwStackSize,        //調整堆疊大小(填0讓系統自行調整)
			LPTHREAD_START_ROUTINE  lpStartAddress,     //新建立的執行緒要執行程式碼函數 (基本上只關心這個參數)
			__drv_aliasesMem LPVOID lpParameter,        //程式碼函數的參數(指標類型)
			DWORD                   dwCreationFlags,    //填0則立即啟動
			LPDWORD                 lpThreadId          //取的執行緒ID
			);
		*/
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hack_thread, 0, 0, 0);//啟動新執行緒執行程式碼
	case DLL_PROCESS_DETACH://當Dll 被解除時觸發
		break;
	}
	return TRUE;
}
