#include "game.h"
#include "memory.h"
#include "parser.h"

#include <iostream>

CGame::CGame(std::wstring procName)
{
  this->_procId = CMemory::GetProcessId(procName);
  this->_modEntry = CMemory::GetModuleEntry(this->_procId, procName);
  this->_procHandle = OpenProcess(PROCESS_VM_READ, 0, this->_procId);
}

void CGame::InitOffsets()
{
  CXmlParser parser("dep/db/Test.xml");
  std::list<OffsetSettings> settings = parser.ParseAll();

  DWORD dw_temp;

  for (auto& it : settings)
  {
    it._offset = CMemory::FullPatternScan(this->_procHandle, this->_modEntry, it._pattern, "", it._add_bytes);

    // has Modifiers
    if (it._add_bytes)
    {
      ReadProcessMemory(this->_procHandle, (void*)it._offset, &dw_temp, sizeof(DWORD), NULL);

      if (!it._rel_call)
      {
        it._offset += dw_temp + it._rel_size + it._rel_offs - it._sub_bytes;
      }
      else
      {
        it._offset += static_cast<long>(dw_temp) + it._rel_size + it._rel_offs - it._sub_bytes;
      }
    }

    std::cout << it._name << ": 0x" << std::hex << it._offset - (uintptr_t)this->_modEntry.modBaseAddr << std::endl;
  }

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
