#include "parser.h"

CXmlParser::CXmlParser(std::string filePath) : _result(_doc.load_file(filePath.c_str()))
{
  if (!_result.status)
  {
    _first_parent = *_doc.document_element().begin();
  }
}

std::list<OffsetSettings> CXmlParser::ParseAll()
{
  std::list<OffsetSettings> data;
  OffsetSettings node;

  std::string name;
  std::string pattern;

  for (auto it = this->_doc.document_element().begin(); it != this->_doc.document_element().end(); ++it)
  {
    node._offset = 0;

    node._name = it->attribute("desc").value();
    node._pattern = it->attribute("pattern").value();

    node._add_bytes = it->child("Add").attribute("value").empty() ? 0
      : std::stoull(it->child("Add").attribute("value").value(), NULL, 16);
    
    node._rel_size = it->child("Rel").attribute("size").empty() ? 0
      : std::stoull(it->child("Rel").attribute("size").value(), NULL, 16);
    
    node._rel_offs = it->child("Rel").attribute("offset").empty() ? 0
      : std::stoull(it->child("Rel").attribute("offset").value(), NULL, 16);
    
    node._sub_bytes = it->child("Sub").attribute("value").empty() ? 0
      : std::stoull(it->child("Sub").attribute("value").value(), NULL, 16);

    data.push_back(node);
  }

  return std::move(data);
}
