#pragma once
DEFINE_COMMAND_PLUGIN(GetFileSize, "Get file size", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(GetPCTrait, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetMousePosition, "", 0, 2, kParams_Tomm_TwoScriptVars)
DEFINE_COMMAND_PLUGIN(FakeMouseMovement, "", 0, 2, kParams_Tomm_TwoFloats)
DEFINE_COMMAND_PLUGIN(IsProgramRunning, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(GetFileTime, "", 0, 2, kParams_Tomm_OneString_OneInt)
DEFINE_COMMAND_PLUGIN(GetFileTimeSTR, "", 0, 3, kParams_Tomm_OneString_TwoInts)
DEFINE_COMMAND_PLUGIN(StringToClipboard, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(ClipboardToString, "", 0, 0, NULL)







bool Cmd_GetFileSize_Execute(COMMAND_ARGS)
{
	alignas(16) char s_strArgTemp[0x4000];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp))
	{
		if (strstr(s_strArgTemp, ":\\"))
		{
			*result = -1;
			return true;
		}
		if (strstr(s_strArgTemp, "..\\")) // thanks C6 
		{
			*result = -1;
			return true;
		}
		if (strstr(s_strArgTemp, "://"))
		{
			*result = -1;
			return true;
		}
		if (strstr(s_strArgTemp, "..//"))
		{
			*result = -1;
			return true;
		}
		FileStream srcFile;
		if (!srcFile.Open(s_strArgTemp))
		{
			*result = -1;
			return true;
		}
		UInt32 length = srcFile.GetLength();
		*result = length;
		srcFile.Close();
		return true;
	}
}


WINBASEAPI
DWORD
WINAPI
GetTickCount(VOID);

bool Cmd_GetPCTrait_Execute(COMMAND_ARGS) //https://github.com/quantumcore/supercharge/issues/1
{
	int iTrait;
	*result = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iTrait))
	{
		if (iTrait == 0 || iTrait == 1)
		{
			NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
			OSVERSIONINFOEXW osInfo;
			*reinterpret_cast<FARPROC*>(&RtlGetVersion) = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
			if (nullptr != RtlGetVersion)
			{
				osInfo.dwOSVersionInfoSize = sizeof osInfo;
				RtlGetVersion(&osInfo);
				if (iTrait == 0)
				{
					*result = osInfo.dwMajorVersion;
				}
				else { *result = osInfo.dwMinorVersion; }
			}
			return true;
		}
		else if (iTrait == 2 || iTrait == 3 || iTrait == 4)
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof(statex);
			GlobalMemoryStatusEx(&statex);
			if (iTrait == 2)
			{
				*result = statex.ullTotalPhys / 1024;
			}
			else if (iTrait == 3)
			{
				*result = statex.ullAvailPhys / 1024;
			}
			else if (iTrait == 4)
			{
				*result = statex.dwMemoryLoad;
			}
		}
		else if (iTrait == 5)
		{
			*result = GetTickCount();
		}
	}
	return true;
}



void MouseMove(float x, float y)
{
	INPUT  Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE;
	Input.mi.dx = x;
	Input.mi.dy = y;
	::SendInput(1, &Input, sizeof(INPUT));
}


bool Cmd_FakeMouseMovement_Execute(COMMAND_ARGS)
{
	float ChangeX, ChangeY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ChangeX, &ChangeY))
	{
		MouseMove(ChangeX, ChangeY);
		return true;
	}
}

bool Cmd_GetMousePosition_Execute(COMMAND_ARGS)
{
	ScriptVar* outX, * outY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outX, &outY))
	{
		POINT p;
		if (GetCursorPos(&p))
		{
			outX->data.num = p.x;
			outY->data.num = p.y;
		}
	}
	return true;
}


#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

bool IsProcessRunning(const TCHAR* const executableName) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!Process32First(snapshot, &entry)) {
		CloseHandle(snapshot);
		return false;
	}

	do {
		if (!_tcsicmp(entry.szExeFile, executableName)) {
			CloseHandle(snapshot);
			return true;
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return false;
}

bool Cmd_IsProgramRunning_Execute(COMMAND_ARGS)
{
	alignas(16) char s_strArgTemp[0x4000];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp))
	{
		*result = IsProcessRunning(s_strArgTemp);
	}
	return true;
}



#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "atlstr.h"


int GetFileTime_EXECUTE(LPCSTR Filename, int iRequest)
{
	HANDLE hFile;
	TCHAR szBuf[MAX_PATH];


	hFile = CreateFile(Filename, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		Console_Print("%s %s", "INVALID FILE>>", Filename);
		return 0;
	}
	//if (GetLastWriteTime(hFile, szBuf, MAX_PATH))
		//Console_Print("%s %s", "GetLastWriteTime>>", szBuf);
	LPTSTR lpszString; 
	DWORD dwSize;

	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return FALSE;

	if (iRequest == 0)
		FileTimeToSystemTime(&ftCreate, &stUTC);
	else if (iRequest == 1)
		FileTimeToSystemTime(&ftAccess, &stUTC);
	else
		FileTimeToSystemTime(&ftWrite, &stUTC);

	//Console_Print("%s %d", "iRequest>>", iRequest);


	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	CloseHandle(hFile);
	char tempchar[0x4000];
	StringCchPrintf(tempchar, MAX_PATH, TEXT("%d"), stLocal.wYear);
	ArrIfc->SetElement(g_ar_temp, "Year", tempchar);

	StringCchPrintf(tempchar, MAX_PATH,TEXT("%02d"),stLocal.wMonth);
	ArrIfc->SetElement(g_ar_temp, "Month", tempchar);

	StringCchPrintf(tempchar, MAX_PATH, TEXT("%02d"), stLocal.wDay);
	ArrIfc->SetElement(g_ar_temp, "Day", tempchar);

	StringCchPrintf(tempchar, MAX_PATH, TEXT("%02d"), stLocal.wHour);
	ArrIfc->SetElement(g_ar_temp, "Hour", tempchar);

	StringCchPrintf(tempchar, MAX_PATH, TEXT("%02d"), stLocal.wMinute);
	ArrIfc->SetElement(g_ar_temp, "Minute", tempchar);



}


bool Cmd_GetFileTime_Execute(COMMAND_ARGS)
{
	alignas(16) char s_strArgTemp[0x4000];
	int iRequest;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp, &iRequest))
	{
		g_ar_temp = ArrIfc->CreateStringMap(NULL, NULL, 0, scriptObj);
		
		LPCSTR Filename = s_strArgTemp;
		GetFileTime_EXECUTE(Filename, iRequest);
		ArrIfc->AssignCommandResult(g_ar_temp, result);
	}

	return true;
}






bool GetFileTimeSTR_EXECUTE(LPCSTR Filename, int iRequest, int iDataOutPutType)
{
	HANDLE hFile;
	TCHAR szBuf[MAX_PATH];


	hFile = CreateFile(Filename, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		Console_Print("%s %s", "INVALID FILE>>", Filename);
		return 0;
	}
	//if (GetLastWriteTime(hFile, szBuf, MAX_PATH))
		//Console_Print("%s %s", "GetLastWriteTime>>", szBuf);
	LPTSTR lpszString;
	DWORD dwSize = 50;

	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return FALSE;

	if (iRequest == 0)
		FileTimeToSystemTime(&ftCreate, &stUTC);
	else if (iRequest == 1)
		FileTimeToSystemTime(&ftAccess, &stUTC);
	else
		FileTimeToSystemTime(&ftWrite, &stUTC);

	//Console_Print("%s %d", "iRequest>>", iRequest);


	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	CloseHandle(hFile);



	if (iDataOutPutType == 1)
	{
		dwRet = StringCchPrintf(g_FileInfoTempChar, dwSize,
			TEXT("%02d/%02d/%d  %02d:%02d"),
			stLocal.wMonth, stLocal.wDay, stLocal.wYear,
			stLocal.wHour, stLocal.wMinute);
	}
	else
	{
		dwRet = StringCchPrintf(g_FileInfoTempChar, dwSize,
			TEXT("%02d/%02d/%d  %02d:%02d"),
			stLocal.wDay,stLocal.wMonth,stLocal.wYear,
			stLocal.wHour, stLocal.wMinute);
	}


}




bool Cmd_GetFileTimeSTR_Execute(COMMAND_ARGS)
{
	alignas(16) char s_strArgTemp[0x4000];
	int iRequest = 0;
	int iDataOutputType = 0;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp, &iRequest, &iDataOutputType))
	{
		LPCSTR Filename = s_strArgTemp;
		GetFileTimeSTR_EXECUTE(Filename, iRequest, iDataOutputType);
		StrIfc->Assign(PASS_COMMAND_ARGS, g_FileInfoTempChar);
	}

	return true;
}







void toClipboard(const std::string& s) {
	OpenClipboard(0);

	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}



bool Cmd_StringToClipboard_Execute(COMMAND_ARGS)
{
	alignas(16) char s_strArgTemp[0x4000];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp))
	{
		toClipboard(s_strArgTemp);
	}

	return true;
}


bool Cmd_ClipboardToString_Execute(COMMAND_ARGS)
{
	OpenClipboard(0);

	char* pResult = (char*)GetClipboardData(CF_TEXT);

	if (pResult != NULL) {
		StrIfc->Assign(PASS_COMMAND_ARGS, pResult);
		GlobalFree(pResult);
	}
	CloseClipboard();


	return true;
}
