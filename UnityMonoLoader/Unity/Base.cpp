#include "Base.h"

#define FG_RED "\033[31m"
#define FG_PURPLE "\033[35m"
#define FG_GREEN "\033[32m"
#define FG_YELLOW "\033[33m"
#define FG_WHITE "\033[0m"

BOOL SetPrivilege(
HANDLE hToken,          // token handle
	LPCTSTR Privilege,BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);


	if (!LookupPrivilegeValue(NULL, Privilege, &luid)) 
		return FALSE;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious);

	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE;

	tpPrevious.PrivilegeCount = 1;
	tpPrevious.Privileges[0].Luid = luid;

	if (bEnablePrivilege) {
		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
	}
	else {
		tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
			tpPrevious.Privileges[0].Attributes);
	}

	AdjustTokenPrivileges(hToken, FALSE, &tpPrevious, cbPrevious, NULL, NULL);
	
	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE;

	return TRUE;
};

enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

int main()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) { MoveWindow(hwnd, 800, 200, 520, 600, TRUE); }
	SetLayeredWindowAttributes(hwnd, 1000, 1000, LWA_ALPHA);
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	system("cls");
	std::cout << FG_WHITE << "" "[" << FG_YELLOW << "DEVELOPER" << FG_WHITE << "]" << " Mono Injector BCWARE" << std::endl;
	std::cout << FG_WHITE << "" "[" << FG_PURPLE << "SITE" << FG_WHITE << "]" << " UnKnoWnCheaTs.me" << std::endl;
	std::cout << FG_WHITE << "" "[" << FG_RED << "GAME" << FG_WHITE << "]" << " ContractWars" << std::endl;
	std::cout << "--------------------------------------------------------------- " << std::endl;
	std::cout << FG_WHITE << "" "[" << FG_GREEN << "!" << FG_WHITE << "]" << " Token" << std::endl;
	
	
	HANDLE mainToken;

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &mainToken))
	{
		if (GetLastError() == ERROR_NO_TOKEN)
		{
			if (!ImpersonateSelf(SecurityImpersonation))
			{

			}

			if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &mainToken)) {
				std::cout << FG_WHITE << "" "[" << FG_RED << "ERROR" << FG_WHITE << "]" << (" Error code: %d\n", GetLastError()) << std::endl;
			}
		}
	}
	if (!SetPrivilege(mainToken, SE_DEBUG_NAME, true))
	{
		CloseHandle(mainToken);
		std::cout << FG_WHITE << "" "[" << FG_RED << "ERROR" << FG_WHITE << "]" << (" Couldn't set DEBUG MODE: %d\n", GetLastError())<< std::endl;
	};
	std::cout << FG_WHITE << "" "[" << FG_GREEN << "PRIVILEGES" << FG_WHITE << "]" << " YES!\n" << std::endl;

		CMonoInjector MonoInject;
		WCHAR BUFFER[500];
		GetCurrentDirectoryW(500, BUFFER);
		wcscat_s(BUFFER, 500, L"\\CWHack.dll");
		std::cout << FG_WHITE << "" "[" << FG_GREEN << "WAIT GAME" << FG_WHITE << "]" << " ContractWars\n" << std::endl;

		//Anti Cheat OFF
		//BOOL AntiCheatOff = MonoInject.InjectMono("Sunrise.exe", "mono.dll", L"Assembly-CSharp.dll", "CodeStage.AntiCheat.Detectors", "InjectionDetector", "StopDetection");
		//BOOL AntiCheatsOff1 = MonoInject.InjectMono("Sunrise.exe", "mono.dll", L"Assembly-CSharp.dll", "CodeStage.AntiCheat.Detectors", "SpeedHackDetector", "StopDetection");
		//
		
		if (MonoInject.InjectMono("CWClient.exe", "mono.dll", BUFFER, "JsonFx.Rare", "BCHOOK", "BCHACK"))
		{
			std::cout << FG_WHITE << "" "[" << FG_GREEN << "INJECT" << FG_WHITE << "]" << " Successfully\n" << std::endl;
		}
		else
		{
			std::cout << FG_WHITE << "" "[" << FG_GREEN << "INJECT" << FG_WHITE << "]" << " Falied\n" << std::endl;
		}

		system("pause");
	
	FreeConsole();
	return 0;
}