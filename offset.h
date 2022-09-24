#pragma once

#include <string>

struct Offset
{
  std::string _name;
  std::string _pattern;

  uintptr_t _offset;
};

struct OffsetSettings : Offset
{
  uintptr_t _add_bytes;
  uintptr_t _rel_size;
  uintptr_t _rel_offs;
  uintptr_t _sub_bytes;
};
