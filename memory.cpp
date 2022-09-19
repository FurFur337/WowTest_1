#include "memory.h"

#include <vector>

DWORD CMemory::GetProcessId(std::wstring procName)
{
		PROCESSENTRY32 procEntry = { 0 };
		HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (!snap)
				return NULL;

		procEntry.dwSize = sizeof(procEntry);

		if (!Process32First(snap, &procEntry))
		{
				CloseHandle(snap);
				return NULL;
		}

		DWORD result = 0;

		do
		{
				if (!wcscmp(procEntry.szExeFile, procName.c_str()))
				{
						result = procEntry.th32ProcessID;
						break;
				}
		} while (Process32Next(snap, &procEntry));

		CloseHandle(snap);

		return result;
}

MODULEENTRY32 CMemory::GetModuleEntry(DWORD procId, std::wstring moduleName)
{
		MODULEENTRY32 modEntry = { 0 };
		HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

		uintptr_t baseAddr = 0;

		if (snap != INVALID_HANDLE_VALUE)
		{
				modEntry.dwSize = sizeof(MODULEENTRY32);

				if (Module32First(snap, &modEntry))
				{
						do 
						{
								if (!wcscmp(modEntry.szModule, moduleName.c_str()))
								{
										break;
								}

						} while (Module32Next(snap, &modEntry));
				}

				CloseHandle(snap);
		}

		return modEntry;
}

uintptr_t CMemory::ChunkPatternScan(byte* base, size_t size, std::string pattern, std::string mask) {

		size_t patternLength = mask.length();
		bool found = false;

		for (uintptr_t i = 0; i < size - patternLength; i++)
		{
				found = true;

				for (uintptr_t j = 0; j < patternLength; j++)
				{
						if (mask[j] != '?' && pattern[j] != (char)base[i + j])
						{
								found = false;
								break;
						}
				}

				if (found)
						return (uintptr_t)base + i;
		}

		return 0;
}

uintptr_t CMemory::FullPatternScan(HANDLE procHandle, MODULEENTRY32 modEntry, std::string pattern, std::string mask, unsigned int add_bytes)
{

		uintptr_t Start = (uintptr_t)modEntry.modBaseAddr;
		uintptr_t End = Start + modEntry.modBaseSize;
		uintptr_t Chunk = Start;

		SIZE_T bytesRead;
		std::vector<BYTE> buffer(4096);

		while (Chunk < End)
		{
				ReadProcessMemory(procHandle, (void*)Chunk, &buffer.front(), 4096, &bytesRead);

				if (bytesRead == 0)
						return 0;

				uintptr_t InternalAddress = ChunkPatternScan(&buffer.front(), bytesRead, pattern, mask);

				if (InternalAddress != 0)
				{
						uintptr_t Offset = InternalAddress - (uintptr_t)&buffer.front();
						return Chunk + Offset + add_bytes;
				}
				else
				{
						Chunk += bytesRead;
				}
		}
}
