#pragma once

#include <windows.h>

class CMonoInjector
{
public:
	CHAR  Path[200];
	CHAR  NameSpase[200];
	CHAR  NameClass[200];
	CHAR  NameFunc[200];

	CHAR  MonoName[200];
	CHAR  MonoFunc1[200];DWORD dwMono1;//mono_get_root_domain
	CHAR  MonoFunc2[200];DWORD dwMono2;//mono_thread_attach
	CHAR  MonoFunc3[200];DWORD dwMono3;//mono_assembly_open
	CHAR  MonoFunc4[200];DWORD dwMono4;//mono_assembly_get_image
	CHAR  MonoFunc5[200];DWORD dwMono5;//mono_class_from_name
	CHAR  MonoFunc6[200];DWORD dwMono6;//mono_class_get_method_from_name
	CHAR  MonoFunc7[200];DWORD dwMono7;//mono_runtime_invoke

	DWORD dwGetModuleHandle;
	DWORD dwGetProcAdrees;
	DWORD dwVirtualAlloc;
	DWORD dwWaitForSingleObject;
	DWORD dwCreateThread;
	DWORD dwVirtualFree;
	BYTE  ShellCode[500];
public:
	CMonoInjector(void);
	~CMonoInjector(void);


	DWORD GetSystemProcessID(const CHAR* Process,const CHAR* Module);
	BOOL  GetProcessModules( DWORD processID ,const CHAR* Module);
	BOOL  InjectMono(const CHAR* Process,const CHAR* Mono,const WCHAR* PathDll,const CHAR* sNameSpase,const CHAR* sNameClass,const CHAR* sNameFunc);
};
