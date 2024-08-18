#include "Pch.h"
#include <windows.h>
#include <tchar.h>
#include <string>

using namespace std;

wstring GetLocationMessage()
{
	wchar_t path[MAX_PATH];

	GetModuleFileName(NULL, path, MAX_PATH);

	wstring pathStr = path;

	wstring message = L"I was injected to '" + pathStr + L"'!";

	return message;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		wstring message = GetLocationMessage();

		MessageBox(NULL, message.c_str(), L"Location Message", MB_OK | MB_ICONINFORMATION);
	}
	break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

