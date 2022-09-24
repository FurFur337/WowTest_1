#pragma once

#include <list>
#include <pugixml/pugixml.hpp>

#include "offset.h"

class IOffsetParser
{
protected:
  virtual std::list<OffsetSettings> ParseAll() = 0;
};

class CXmlParser : public IOffsetParser
{
public:
  
  CXmlParser() = delete;
  CXmlParser(std::string filePath);

  std::list<OffsetSettings> ParseAll() override;

private:

  pugi::xml_document _doc;
  pugi::xml_parse_result _result;
  pugi::xml_node _first_parent;

};