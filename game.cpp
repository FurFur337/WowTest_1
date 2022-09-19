#include "game.h"
#include "memory.h"

#include <iostream>

CGame::CGame(std::wstring procName)
{
  this->_procId = CMemory::GetProcessId(procName);
  this->_modEntry = CMemory::GetModuleEntry(this->_procId, procName);
  this->_procHandle = OpenProcess(PROCESS_VM_READ, 0, this->_procId);
}

void CGame::InitOffsets()
{
  SIZE_T numBytes = 0;
  DWORD buffer = 0;

  this->_IsPlayerInGameOffs = CMemory::FullPatternScan(
      this->_procHandle
    , this->_modEntry
    , "\x48\x8D\x0D\x01\x01\x01\x01\xE8\x01\x01\x01\x01\x8B\x05\x01\x01\x01\x01\x48\x8D\x54\x24\x01\x48\x8B\x1D\x01\x01\x01\x01\x48\x8D\x0D\x01\x01\x01\x01\x89\x44\x24\x30\xE8\x01\x01\x01\x01\xF3\x0F\x10\x05\x01\x01\x01\x01"
    , "xxx????x????xx????xxxx?xxx????xxx????xxxxx????"
    , 3
  );

  ReadProcessMemory(this->_procHandle, (void*)this->_IsPlayerInGameOffs, &buffer, sizeof(DWORD), &numBytes);
  this->_IsPlayerInGameOffs += buffer + (BYTE)0x04 + (BYTE)0x10;
  // std::cout << "_IsPlayerInGameOffs: " << std::hex << _IsPlayerInGameOffs << std::endl;


  this->_IsLoadingOrConnectingOffs = CMemory::FullPatternScan(
      this->_procHandle
    , this->_modEntry
    , "\x40\x38\x35\x01\x01\x01\x01\x0F\x84\x01\x01\x01\x01\x40\x88\x35\x01\x01\x01\x01"
    , "xxx????xx????xxx????"
    , 3
  );

  ReadProcessMemory(this->_procHandle, (void*)this->_IsLoadingOrConnectingOffs, &buffer, sizeof(DWORD), &numBytes);
  this->_IsLoadingOrConnectingOffs += buffer + (BYTE)0x04;
  // std::cout << "_IsLoadingOrConnectingOffs: " << std::hex << _IsLoadingOrConnectingOffs << std::endl;
}

bool CGame::IsPlayerInGame()
{
  BYTE result = 0;
  ReadProcessMemory(this->_procHandle, (void*)this->_IsPlayerInGameOffs, &result, sizeof(BYTE), NULL);
  return result;
}

bool CGame::IsPlayerLoading()
{
  BYTE result = 0;
  ReadProcessMemory(this->_procHandle, (void*)this->_IsLoadingOrConnectingOffs, &result, sizeof(BYTE), NULL);
  return result;
}
