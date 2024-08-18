#include <iostream>
#include <windows.h>
#include <string>
#include <TlHelp32.h>

#include "MessageHelper.h"
#include "ProcessInjectionHelper.h"

using namespace std;

bool IsRunningAsAdmin()
{
	bool result = false;

	PSID adminGroup = nullptr;
	SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;

	if (AllocateAndInitializeSid(&authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup))
	{
		BOOL isAdmin = FALSE;

		if (!CheckTokenMembership(NULL, adminGroup, &isAdmin))
		{
			isAdmin = FALSE;
		}

		result = (isAdmin == TRUE);
	}

	if (adminGroup != nullptr)
	{
		FreeSid(adminGroup);
	}

	return result;
}

int main()
{
	//
	auto cmdLine = GetCommandLineW();

	int argc;

	auto argv = CommandLineToArgvW(cmdLine, &argc);

	if (argv != nullptr && argc > 2)
	{
		MessageHelper::PrintHelp(argv[1], argv[2]);

		wchar_t* internalMessage = nullptr;

		if (IsRunningAsAdmin())
		{
			if (ProcessInjectionHelper::InjectDLL(argv[1], argv[2], &internalMessage))
			{
				MessageHelper::PrintMessage((L"'" + wstring(argv[1]) + L"' was injected to '" + wstring(argv[2]) + L"' successfully!").c_str());
				MessageHelper::PrintMessage(L"");
				MessageHelper::PrintMessage(L"If your DLL hasn't executed, one of the following reasons might be the cause:");
				MessageHelper::PrintMessage(L"");
				MessageHelper::PrintMessage(L"\ta. The provided DLL path seems to be incomplete or incorrect");
				MessageHelper::PrintMessage(L"\tb. The architecture of the DLL and the process do not match; they must both be either x86 or x64");
				MessageHelper::PrintMessage(L"\tc. Error either in loading the DLL or in executing the instructions within the DLL you provided");
			}
			else
			{
				if (internalMessage != nullptr)
				{
					MessageHelper::PrintMessage(internalMessage);
				}

				MessageHelper::PrintMessage(L"The injection process was failed!");
			}

			if (internalMessage != nullptr)
			{
				delete internalMessage;
			}
		}
		else
		{
			MessageHelper::PrintMessage(L"The current process lacks administrator privileges");
			MessageHelper::PrintMessage(L"The injection process was failed!");
		}
	}
	else
	{
		MessageHelper::PrintHelp(L"unknown!", L"unknown!");
		MessageHelper::PrintMessage(L"Please read the usage guide, as the usage was incorrect!");
	}

	std::wcout << std::endl;

	//
	LocalFree(argv);

	return 0;
}
