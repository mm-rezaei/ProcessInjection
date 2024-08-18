#pragma once

class MessageHelper
{
private:

	static void PrintIntroduction()
	{
		std::wcout << std::endl;
		std::wcout << L"DLL Injection (Injecting a DLL into the target process)" << std::endl;
	}

	static void PrintMethodSpecification()
	{
		std::wcout << std::endl;
		std::wcout << L"\tMethod Specification:" << std::endl;
		std::wcout << std::endl;
		std::wcout << L"\t\t1- Both the DLL and the target process must have the same architecture, either x86 or x64" << std::endl;
		std::wcout << L"\t\t2- Injecting into the target process requires administrator privileges" << std::endl;
		std::wcout << L"\t\t3- The active antivirus may cause the injection failure" << std::endl;
		std::wcout << L"\t\t4- To inject a specific DLL into a target process, it can only be executed once per process instance" << std::endl;
	}

	static void PrintGuidance()
	{
		std::wcout << std::endl;
		std::wcout << L"\tUsage Guide:" << std::endl;
		std::wcout << std::endl;
		std::wcout << L"\t\tYou need to provide two parameters in the following format:" << std::endl;
		std::wcout << std::endl;
		std::wcout << L"\t\t\tProcessInjection.exe \"dllname\" \"processname\"" << std::endl;
		std::wcout << std::endl;
		std::wcout << L"\t\t\tdllname: The full path of the dll file that you want to inject into the target process" << std::endl;
		std::wcout << L"\t\t\tprocessname: The name of the process you want to inject into" << std::endl;
		std::wcout << std::endl;
		std::wcout << L"\t\t\tUsage Example: ProcessInjection.exe \"C:\\path\\to\\inject.dll\" \"explorer.exe\"" << std::endl;
	}

	static void PrintDllFileCriteriaList()
	{
		std::wcout << std::endl;
		std::wcout << L"\t\t\ta. Ensure that the DLL name includes the complete file path" << std::endl;
	}

	static void PrintProcessCriteriaList()
	{
		std::wcout << std::endl;
		std::wcout << L"\t\t\ta. Ensure that the process name contains the extension (*.exe)" << std::endl;
	}

	static void PrintInputCriteriaList()
	{
		std::wcout << std::endl;
		std::wcout << L"\tCriteria List of Inputs:" << std::endl;
		std::wcout << std::endl;
		std::wcout << L"\t\tCriteria List of the Dll File:" << std::endl;

		PrintDllFileCriteriaList();

		std::wcout << std::endl;

		std::wcout << L"\t\tCriteria List of the Process:" << std::endl;

		PrintProcessCriteriaList();
	}

public:

	static void PrintHelp(const wchar_t* inDllname, const wchar_t* inProcessName)
	{
		PrintIntroduction();
		PrintMethodSpecification();
		PrintGuidance();
		PrintInputCriteriaList();

		std::wcout << std::endl;
		std::wcout << L"\tInput Files:" << std::endl;
		std::wcout << std::endl;
		std::wcout << L"\t\tdllname: " << inDllname << std::endl;
		std::wcout << L"\t\tprocessname: " << inProcessName << std::endl;

		std::wcout << std::endl;
		std::wcout << L"\tMessages:" << std::endl;
		std::wcout << std::endl;
	}

	static void PrintMessage(const wchar_t* inMessage)
	{
		std::wcout << L"\t\t" << inMessage << std::endl;
	}
};

