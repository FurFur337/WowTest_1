#pragma once

#include <string>

#include <Windows.h>
#include <TlHelp32.h>

class CMemory
{
public:

  static DWORD GetProcessId(std::wstring procName);
  static MODULEENTRY32 GetModuleEntry(DWORD procId, std::wstring moduleName);

  static uintptr_t ChunkPatternScan(byte* base, size_t size, std::string bytePattern, std::string mask);
  static uintptr_t FullPatternScan(HANDLE procHandle, MODULEENTRY32 modEntry, std::string hexPattern, std::string mask, unsigned int add_bytes = 0);

};