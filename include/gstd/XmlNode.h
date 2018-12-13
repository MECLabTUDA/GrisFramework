#pragma once


#include "gstd_api.h"
#include "XmlAttribute.h"

#include <memory>
#include <string>
#include <exception>
#include <vector>
#include <ostream>

#pragma warning (push)
#pragma warning (disable : 4275) // std::exception dll interface

namespace pugi
{
  class xml_document;
  class xml_node;  
}

namespace gris
{
  /**
  */
  class GRIS_GSTD_API XmlNode
  {
    public:
      XmlNode();
      explicit XmlNode(pugi::xml_node* node);
      XmlNode(const XmlNode& o);
      XmlNode& operator=(const XmlNode& o);
      XmlNode(XmlNode&& o);
      XmlNode& operator=(XmlNode&& o);

      ~XmlNode();

    public:
      void            setRoot(pugi::xml_node& root);
      pugi::xml_node* getNode();

    public:
      void                 setName(const std::string& val);
      const char*          getName() const;
      void                 setValue(const std::string& val);
      const char*          getValue() const;
      XmlAttribute         getAttribute(const std::string& name) const;

    public:
      std::vector<XmlNode> getChildren() const;
      XmlNode              getChild    (const std::string& name) const;
      bool                 hasChild    (const std::string& name) const;
      void                 addChild    (const XmlNode& node);
      XmlNode              addChild    (const std::string& name);
      void                 removeChild (const std::string& name);
      void                 removeChildren();
      void                 addAttribute(const std::string& name, const std::string& value="");
      XmlAttribute         addAttribute(const std::string& name);

    public:
      std::vector<XmlNode> XPathNodes  (const std::string& name) const;
      XmlNode              XPathNode   (const std::string& name) const;

    public:
      std::ostream& print(std::ostream& os) const;

    private:
      enum EnErrorTypes
      {
        enChildNotFound = 0,
        enAttributeNotFound,
        enXPathNotFound,
        enXPathInvalid,
        N_Items
      };

      static const char* ErrorStrings[N_Items];

    private:
      struct Impl;
      std::unique_ptr<Impl> mp;
  };

  GRIS_GSTD_API std::ostream& operator<<(std::ostream& os, const XmlNode& node);
}

#pragma warning (pop)