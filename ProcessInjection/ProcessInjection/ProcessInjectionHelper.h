#pragma once

class ProcessInjectionHelper
{
	static bool ExistFile(const wchar_t* inFileAddress)
	{
		bool result = false;

		DWORD fileAttributes = GetFileAttributesW(inFileAddress);

		if (fileAttributes != INVALID_FILE_ATTRIBUTES)
		{
			result = true;
		}

		return result;
	}

	static void CopyMessageToReferenceVariable(const wchar_t* inMessage, wchar_t** refMessageVariable)
	{
		if (inMessage == nullptr)
		{
			*refMessageVariable = nullptr;
		}
		else
		{
			auto messageLength = wcslen(inMessage) + 1;

			*refMessageVariable = new wchar_t[messageLength];

			memcpy(*refMessageVariable, inMessage, messageLength * sizeof(wchar_t));
		}
	}

	static DWORD GetProcessIdByName(const wchar_t* inProcessName)
	{
		DWORD result = 0;

		PROCESSENTRY32W processEntry;

		processEntry.dwSize = sizeof(PROCESSENTRY32W);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (snapshot != INVALID_HANDLE_VALUE)
		{
			if (Process32FirstW(snapshot, &processEntry) == TRUE)
			{
				do {
					if (wcscmp(processEntry.szExeFile, inProcessName) == 0)
					{
						result = processEntry.th32ProcessID;

						break;
					}
				} while (Process32NextW(snapshot, &processEntry));
			}

			CloseHandle(snapshot);
		}

		return result;
	}

public:

	static bool InjectDLL(const wchar_t* inDllAddress, DWORD inProcessId, wchar_t** refMessage)
	{
		bool result = false;

		if (ExistFile(inDllAddress))
		{
			HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, inProcessId);

			if (processHandle != NULL)
			{
				auto dllAddressSize = (wcslen(inDllAddress) + 1) * sizeof(wchar_t);

				auto allocatedMemoryAddress = VirtualAllocEx(processHandle, NULL, dllAddressSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

				if (allocatedMemoryAddress != NULL)
				{
					if (WriteProcessMemory(processHandle, allocatedMemoryAddress, inDllAddress, dllAddressSize, NULL) == TRUE)
					{
						HANDLE threadHandle = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, allocatedMemoryAddress, 0, NULL);

						if (threadHandle != NULL)
						{
							CloseHandle(threadHandle);

							result = true;
						}
						else
						{
							CopyMessageToReferenceVariable(L"Unable to initiate a remote thread in the target process", refMessage);
						}
					}
					else
					{
						CopyMessageToReferenceVariable(L"Failed to write to the allocated memory in the remote process", refMessage);
					}
				}
				else
				{
					CopyMessageToReferenceVariable(L"The memory allocation in the remote process has failed", refMessage);
				}

				CloseHandle(processHandle);
			}
			else
			{
				if (GetLastError() == 5)
				{
					CopyMessageToReferenceVariable(L"The process could not be opened because access was denied", refMessage);
				}
				else
				{
					CopyMessageToReferenceVariable(L"The process could not be opened due to an unknown error", refMessage);
				}
			}
		}
		else
		{
			CopyMessageToReferenceVariable(L"The dll file could not be found", refMessage);
		}

		return result;
	}

	static bool InjectDLL(const wchar_t* dllAddress, const wchar_t* processName, wchar_t** refMessage)
	{
		bool result = false;

		DWORD processId = GetProcessIdByName(processName);

		if (processId != 0)
		{
			result = InjectDLL(dllAddress, processId, refMessage);
		}
		else
		{
			CopyMessageToReferenceVariable(L"The process name could not be found", refMessage);
		}

		return result;
	}
};
