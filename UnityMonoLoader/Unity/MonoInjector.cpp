#include "MonoInjector.h"
#include <stdio.h>
#include <TlHelp32.h>
#include <psapi.h>

#pragma comment( lib, "psapi.lib")

const BYTE MemCode[] = {
	0xe8, 0, 0, 0, 0, 80, 0xe8, 0, 0, 0, 0, 0xb8, 0, 0, 0, 0, 
	0x6a, 0, 80, 0xe8, 0, 0, 0, 0, 80, 0xe8, 0, 0, 0, 0, 0xba, 0, 
	0, 0, 0, 0x52, 0xb9, 0, 0, 0, 0, 0x51, 80, 0xe8, 0, 0, 0, 0, 
	0x6a, 0, 0xb9, 0, 0, 0, 0, 0x51, 80, 0xe8, 0, 0, 0, 0, 0x6a, 0, 
	0x6a, 0, 0x6a, 0, 80, 0xe8, 0, 0, 0, 0, 0x83, 0xc4, 0x38, 0xc3
};

typedef HMODULE (WINAPI* _GetModuleHandleA)(LPCSTR );
typedef LPVOID  (WINAPI* _VirtualAlloc)(LPVOID,SIZE_T,DWORD,DWORD);
typedef FARPROC (WINAPI* _GetProcAddress)(HMODULE,LPCSTR);
typedef DWORD   (WINAPI* _WaitForSingleObject)(HANDLE,DWORD);
typedef HANDLE  (WINAPI* _CreateThread)(LPSECURITY_ATTRIBUTES,SIZE_T,LPTHREAD_START_ROUTINE,LPVOID,DWORD,LPDWORD);
typedef BOOL    (WINAPI* _VirtualFree)(LPVOID,SIZE_T,DWORD);

static BOOL WINAPI MemFunc1(CMonoInjector * Data)
{
	if (Data)
	{
		_GetModuleHandleA GetModHandle = (_GetModuleHandleA)(Data->dwGetModuleHandle);
		_VirtualAlloc VirtAlloc = (_VirtualAlloc)(Data->dwVirtualAlloc);
		_GetProcAddress GetProcAdr = (_GetProcAddress)(Data->dwGetProcAdrees);
		_WaitForSingleObject WaiteThread = (_WaitForSingleObject)(Data->dwWaitForSingleObject);
		_CreateThread CreatThred = (_CreateThread)(Data->dwCreateThread);
		_VirtualFree FreeAlloc = (_VirtualFree)(Data->dwVirtualFree);

		if (GetModHandle != NULL && VirtAlloc != NULL && GetProcAdr != NULL && \
			WaiteThread != NULL && CreatThred != NULL && FreeAlloc != NULL)
		{
			Data->dwMono1 = (DWORD)GetProcAdr(GetModHandle(Data->MonoName),Data->MonoFunc1);
			Data->dwMono2 = (DWORD)GetProcAdr(GetModHandle(Data->MonoName),Data->MonoFunc2);
			Data->dwMono3 = (DWORD)GetProcAdr(GetModHandle(Data->MonoName),Data->MonoFunc3);
			Data->dwMono4 = (DWORD)GetProcAdr(GetModHandle(Data->MonoName),Data->MonoFunc4);
			Data->dwMono5 = (DWORD)GetProcAdr(GetModHandle(Data->MonoName),Data->MonoFunc5);
			Data->dwMono6 = (DWORD)GetProcAdr(GetModHandle(Data->MonoName),Data->MonoFunc6);
			Data->dwMono7 = (DWORD)GetProcAdr(GetModHandle(Data->MonoName),Data->MonoFunc7);

			if (Data->dwMono1 && Data->dwMono2 && Data->dwMono3 && Data->dwMono4 && \
				Data->dwMono5 && Data->dwMono6 && Data->dwMono7)
			{
				BYTE* MemoryInject = (BYTE*)VirtAlloc(0,500, MEM_COMMIT,PAGE_EXECUTE_READWRITE);

				if (MemoryInject)
				{
					for(DWORD i = 0; i < 500; i++){
						MemoryInject[i] = Data->ShellCode[i];
					}

					*(DWORD*)(&MemoryInject[1]) = (DWORD)((Data->dwMono1 - (DWORD)(&MemoryInject[1])) - 4);
					*(DWORD*)(&MemoryInject[7]) = (DWORD)((Data->dwMono2 - (DWORD)(&MemoryInject[7])) - 4);
					*(DWORD*)(&MemoryInject[12]) = (DWORD)(&Data->Path);
					*(DWORD*)(&MemoryInject[20]) = (DWORD)((Data->dwMono3 - (DWORD)(&MemoryInject[20])) - 4);
					*(DWORD*)(&MemoryInject[0x1a]) = (DWORD)((Data->dwMono4 - (DWORD)(&MemoryInject[0x1a])) - 4);
					*(DWORD*)(&MemoryInject[0x1f]) = (DWORD)(&Data->NameClass);
					*(DWORD*)(&MemoryInject[0x25]) = (DWORD)(&Data->NameSpase);
					*(DWORD*)(&MemoryInject[0x2c]) = (DWORD)((Data->dwMono5 - (DWORD)(&MemoryInject[0x2c])) - 4);
					*(DWORD*)(&MemoryInject[0x33]) = (DWORD)(&Data->NameFunc);
					*(DWORD*)(&MemoryInject[0x3a]) = (DWORD)((Data->dwMono6 - (DWORD)(&MemoryInject[0x3a])) - 4);
					*(DWORD*)(&MemoryInject[70]) = (DWORD)((Data->dwMono7 - (DWORD)(&MemoryInject[70])) - 4);


					HANDLE hThread = CreatThred(0,0,(LPTHREAD_START_ROUTINE)MemoryInject,0,0,0);

					if (hThread){
						WaiteThread(hThread,INFINITE);
						FreeAlloc((LPVOID)MemoryInject,500,MEM_COMMIT);
						return TRUE;
					}

					FreeAlloc((LPVOID)MemoryInject,500,MEM_COMMIT);

					return FALSE;
				}

			}
		}

	}

	return FALSE;
}

static BOOL WINAPI MemFunc2(){
	return FALSE;
}



const char * ConvertToUTF8(const wchar_t * pStr) { 
	static char szBuf[1024]; 
	WideCharToMultiByte(CP_UTF8, 0, pStr, -1, szBuf, sizeof(szBuf), NULL, NULL); 
	return szBuf; 
}


CMonoInjector::CMonoInjector(void)
{
	ZeroMemory(&Path,200);
	ZeroMemory(&NameSpase,200);
	ZeroMemory(&NameClass,200);
	ZeroMemory(&NameFunc,200);
	ZeroMemory(&MonoName,200);
	ZeroMemory(&MonoFunc1,200);
	ZeroMemory(&MonoFunc2,200);
	ZeroMemory(&MonoFunc3,200);
	ZeroMemory(&MonoFunc4,200);
	ZeroMemory(&MonoFunc5,200);
	ZeroMemory(&MonoFunc6,200);
	ZeroMemory(&MonoFunc7,200);
	ZeroMemory(&ShellCode,500);


	dwGetModuleHandle = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"GetModuleHandleA");
	dwGetProcAdrees   = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"GetProcAddress");
	dwVirtualAlloc    = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"VirtualAlloc");
	dwWaitForSingleObject    = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"WaitForSingleObject");
	dwCreateThread    = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"CreateThread");
	dwVirtualFree    = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"VirtualFree");


	memcpy(&ShellCode,&MemCode,sizeof(MemCode));

	strcpy_s(MonoFunc1,200,"mono_get_root_domain");
	strcpy_s(MonoFunc2,200,"mono_thread_attach");
	strcpy_s(MonoFunc3,200,"mono_assembly_open");
	strcpy_s(MonoFunc4,200,"mono_assembly_get_image");
	strcpy_s(MonoFunc5,200,"mono_class_from_name");
	strcpy_s(MonoFunc6,200,"mono_class_get_method_from_name");
	strcpy_s(MonoFunc7,200,"mono_runtime_invoke");

}

CMonoInjector::~CMonoInjector(void)
{
}



DWORD CMonoInjector::GetSystemProcessID(const CHAR* Process,const CHAR* Module) 
{
	HANDLE snap;
	PROCESSENTRY32 pentry32;

	snap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if(snap==INVALID_HANDLE_VALUE) return 0;

	pentry32.dwSize=sizeof(PROCESSENTRY32);

	if(!Process32First(snap,&pentry32)) {CloseHandle(snap);return 0;}
	do
	{
		if(!lstrcmpi(Process,&pentry32.szExeFile[0]))
		{
			if (GetProcessModules(pentry32.th32ProcessID,Module))
			{
				CloseHandle(snap);
				return pentry32.th32ProcessID;
			}
		}
	}

	while(Process32Next(snap,&pentry32));

	CloseHandle(snap);

	return 0;
}


BOOL CMonoInjector::GetProcessModules( DWORD processID ,const CHAR* Module)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID );
	if (NULL == hProcess)
		return FALSE;

	if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
		{
			char szModName[MAX_PATH];

			if ( GetModuleBaseName( hProcess, hMods[i], szModName,
				sizeof(szModName)))
			{
				if (_strcmpi(szModName,Module)==0)
				{
					return TRUE;
				}
			}
		}
	}

	CloseHandle( hProcess );

	return FALSE;
}



BOOL CMonoInjector::InjectMono(const CHAR* Process,const CHAR* Mono,const WCHAR* PathDll,const CHAR* sNameSpase,const CHAR* sNameClass,const CHAR* sNameFunc)
{
	if (Process)
	{
		DWORD PID = 0;

		strcpy_s(MonoName,200,Mono);
		strcpy_s(Path,200,ConvertToUTF8(PathDll));
		strcpy_s(NameSpase,200,sNameSpase);
		strcpy_s(NameClass,200,sNameClass);
		strcpy_s(NameFunc,200,sNameFunc);


		while(PID == NULL){
			PID = GetSystemProcessID(Process,Mono);
		}

		if (PID)
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,PID);

			if (hProcess)
			{

				DWORD Size = (DWORD)(&MemFunc2)-(DWORD)(&MemFunc1);

				LPVOID MemoryInject = VirtualAllocEx(hProcess,0,Size+200, MEM_COMMIT,PAGE_EXECUTE_READWRITE);
				LPVOID MemoryParm = VirtualAllocEx(hProcess,0,sizeof(CMonoInjector), MEM_COMMIT,PAGE_EXECUTE_READWRITE);

				if (MemoryInject != NULL && MemoryParm != NULL)
				{
					if (WriteProcessMemory(hProcess,MemoryInject,&MemFunc1,Size,0))
					{
						if (WriteProcessMemory(hProcess,MemoryParm,this,sizeof(CMonoInjector),0))
						{
							HANDLE hThread = CreateRemoteThread(hProcess,0,0,(LPTHREAD_START_ROUTINE)MemoryInject,MemoryParm,0,0);

							if (hThread)
							{
								WaitForSingleObject(hThread,INFINITE);

								VirtualFreeEx(hProcess,MemoryInject,Size+200,MEM_COMMIT);
								VirtualFreeEx(hProcess,MemoryInject,sizeof(CMonoInjector),MEM_COMMIT);

								CloseHandle(hThread);
								CloseHandle(hProcess);

								return TRUE;
							}
							else{
								MessageBox(0,"Ошибка создания потока!",0,MB_OK | MB_SYSTEMMODAL);
							}
							
						}else{
							MessageBox(0,"Ошибка записи в памяти №2!",0,MB_OK | MB_SYSTEMMODAL);
						}
					}
					else{
						MessageBox(0,"Ошибка записи в памяти №1!",0,MB_OK | MB_SYSTEMMODAL);
					}


					VirtualFreeEx(hProcess,MemoryInject,Size+200,MEM_COMMIT);
					VirtualFreeEx(hProcess,MemoryInject,sizeof(CMonoInjector),MEM_COMMIT);

				}
				else
				{
					MessageBox(0,"Ошибка выдиления памяти!",0,MB_OK | MB_SYSTEMMODAL);
				}

				CloseHandle(hProcess);
			}
			else
			{
				MessageBox(0,"Ошибка открытия процесса!",0,MB_OK | MB_SYSTEMMODAL);
			}

		}
		else{
			MessageBox(0,"Ошибка поиска процесса!",0,MB_OK | MB_SYSTEMMODAL);
		}

	}

	return FALSE;
}