#pragma once

#include "gstd_api.h"
#include "XmlNode.h"

#include <memory>

namespace pugi
{
  class xml_document;
  class xml_node;
}

namespace gris
{
  /**
  */
  class GRIS_GSTD_API XmlDocument
  {
    public:
      static std::unique_ptr<XmlDocument> create(const char* rootName);
      static std::unique_ptr<XmlDocument> read(const char* filename);
      static void                         save(const char* filename, XmlDocument&);

    public:
      XmlDocument();      
      ~XmlDocument();

    public:
      XmlNode getRoot();

    private:
      struct Impl;
      std::unique_ptr<Impl> mp;
  };

}