#include "parser.h"

#include <iostream>

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

    node._add_bytes = 0;
    node._rel_size = 0;
    node._rel_offs = 0;
    node._sub_bytes = 0;
    node._rel_call = false;

    pugi::xml_node RelNode;

    if (!it->child("Modifiers").empty())
    {
      // Add
      node._add_bytes = it->child("Modifiers").first_child().attribute("value").empty() ? 0
        : std::stoull(it->child("Modifiers").first_child().attribute("value").value(), NULL, 10);

      if (!it->child("Modifiers").child("RelCall").empty())
      {
        node._rel_call = true;
        RelNode = it->child("Modifiers").child("RelCall");
      }
      else
      {
        RelNode = it->child("Modifiers").child("Rel");
      }

      if (!RelNode.empty())
      {
        // <Rel size="N" /> set 4 by default if not exists
        node._rel_size = RelNode.attribute("size").empty() ? 4
          : std::stoull(RelNode.attribute("size").value(), NULL, 10);

        // <Add value="N" /> after Rel, if exists
        node._rel_offs = RelNode.next_sibling().attribute("value").empty() ? 0
          : std::stoull(RelNode.next_sibling().attribute("value").value(), NULL, 10);
      }

      node._sub_bytes = it->child("Modifiers").child("Sub").attribute("value").empty() ? 0
        : std::stoull(it->child("Modifiers").child("Sub").attribute("value").value(), NULL, 10);
    }

    data.push_back(node);
  }

  return std::move(data);
}
