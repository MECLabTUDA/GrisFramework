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
      static std::unique_ptr<XmlDocument> create(const std::string& rootName);
      static std::unique_ptr<XmlDocument> read(const std::string& filename);
      static void                         save(const std::string& filename, XmlDocument&);
      
    public:
      ~XmlDocument();

    private:
      XmlDocument();

    public:
      XmlNode getRoot();

      void fromString(const std::string& string);

    private:
      struct Impl;
      std::unique_ptr<Impl> mp;
  };

}