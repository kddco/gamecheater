#include "windows.h" //Windows API Library
#include "winuser.h"
#include <stdio.h>


int main()
{
	//直接寫入遊戲並修改
	printf("External Hack by kenny\n");

	//HWND FindWindowA(
//LPCSTR lpClassName, 
//LPCSTR lpWindowName //Game Windows Title);
// 
// 
	//HWND hWnd = FindWindow(NULL, L"忍豆風雲•三(2023-08-01)");//Game Window Title to Hwnd handle
	
	HWND hWnd = FindWindow(NULL, L"Plants vs. Zombies");
	if (hWnd)
	{
		printf("Find Game Windows Hwnd\n");
		DWORD game_pid = NULL;
		GetWindowThreadProcessId(hWnd, &game_pid);//hwnd to PID
		printf("game_pid:%d\n", game_pid);
		DWORD game_image_base = GetWindowLongPtr(hWnd, GWLP_HINSTANCE);// Hwnd to 遊戲 image address
		printf("game_image_base:0x%x\n", game_image_base);



		//HANDLE OpenProcess(
		//DWORD dwDesiredAccess, //要求的 Process Handle 權限  
		//BOOL  bInheritHandle,  //此Handle 是否需要被 child process 繼承
		//DWORD dwProcessId      //欲請求 process Handle 的目標PID);

		//(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION)
		HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, game_pid);
		if (pHandle)
		{
			//開始讀取陽光地址 [[[popcapgame1.exe + 0x002a9f38]+0x768]+0x5560]
			SIZE_T dwReaded = 0;
			DWORD target_addr = game_image_base + 0x002a9f38;//欲讀取的記憶體位置
			DWORD dwSunny = 0;//陽光記憶體位置
			DWORD dwOffset1 = 0x00000768; //指向第一級指標的 offset
			DWORD dwOffset2 = 0x00005560; //指向第二級指標的 offset

			//BOOL WINAPI ReadProcessMemory(
			//_In_  HANDLE  hProcess, //可存取目標的 process HANDLE
			//_In_  LPCVOID lpBaseAddress, //欲讀取的記憶體地址
			//_Out_ LPVOID  lpBuffer,      //讀取後的結果
			//_In_  SIZE_T  nSize,         //欲讀取的記憶體大小
			//_Out_ SIZE_T  *lpNumberOfBytesRead //真正讀取到的記憶體大小);
			if (ReadProcessMemory(pHandle, (LPVOID)target_addr, &dwSunny, 4, &dwReaded))
			{
				target_addr = dwSunny + dwOffset1;//加上第一級指標 offset
				if (ReadProcessMemory(pHandle, (LPVOID)target_addr, &dwSunny, 4, &dwReaded))
				{
					target_addr = dwSunny + dwOffset2;//加上第二級指標 offset
					if (ReadProcessMemory(pHandle, (LPVOID)target_addr, &dwSunny, 4, &dwReaded))
					{
						printf("Get Sunny Memory address:0x%x  Count:%d\n", target_addr, dwSunny);

						SIZE_T dwWrited = 0;
						dwSunny = 9999;
						//BOOL WINAPI WriteProcessMemory(
						//_In_  HANDLE  hProcess, //可存取目標的 process HANDLE
						//_In_  LPVOID  lpBaseAddress, //欲寫入的記憶體地址
						//_In_  LPCVOID lpBuffer, //欲寫入的資料
						//  _In_  SIZE_T  nSize,  //欲寫入的資料大小
						//_Out_ SIZE_T  //真正寫入的記憶體大小*lpNumberOfBytesWritten);
						if (WriteProcessMemory(pHandle, (LPVOID)target_addr, &dwSunny, 4, &dwWrited))
						{
							printf("Write Sunny %d Success!\n", dwSunny);
						}

					}
				}
			}
			else
			{
				printf("Read Memory Error.\n");
			}

			CloseHandle(pHandle);//關閉 process Handle
		}
		else
		{
			printf("OpenProcess Handle Fail\n");
		}
	}
	else
	{
		printf("Please Open Game.\n");
	}

	printf("waiting for end...");
	getchar();//PAUSE for input
	return 0;
}