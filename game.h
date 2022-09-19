#pragma once

#include <string>

#include <Windows.h>
#include <TlHelp32.h>

class CGame
{
public:
  CGame(std::wstring procName);
  void InitOffsets();

  bool IsPlayerInGame();
  bool IsPlayerLoading();

private:
  MODULEENTRY32 _modEntry;
  HANDLE _procHandle;
  DWORD _procId;


  uintptr_t _IsPlayerInGameOffs;
  uintptr_t _IsLoadingOrConnectingOffs;
};