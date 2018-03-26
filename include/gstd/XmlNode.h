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
      void                 addAttribute(const std::string& name, const std::string& value="");
      XmlAttribute         addAttribute(const std::string& name);

    public:
      std::vector<XmlNode> XPathNodes  (const std::string& name) const;
      XmlNode              XPathNode   (const std::string& name) const;

    public:
      std::ostream& print(std::ostream& os) const;

    private:
      struct Impl;
      std::unique_ptr<Impl> mp;
  };

  GRIS_GSTD_API std::ostream& operator<<(std::ostream& os, const XmlNode& node);

  class GRIS_GSTD_API XmlException : public std::exception
  {
    public:
      enum EnType
      {
        InvalidXmlDocument,
        FileNotFound,
        ChildNotFound,
        AttributeNotFound,
        SavingFailed,
        XPathNotFound,
        XPathInvalid,
        N_Items
      };

      const char* Strings[N_Items] = 
      {
        "Not a valid XML document",
        "File not found",
        "Child not found",
        "Attribute not found",
        "XPath not found",
        "XPath invalid",
        "Saving failed"
      };

    public:
      explicit XmlException(EnType type)
      {
        mType = type;
        mStr = Strings[mType];
      }

      explicit XmlException(EnType type, const std::string& info)
      {
        mType = type;
        mStr = Strings[mType];
        mInfo = info;
      }

      void addInfo(const std::string& str) { mInfo = str; }

      virtual char const* what() const;

    private:
      EnType mType;
      std::string mStr;
      std::string mInfo;
  };

}

#pragma warning (pop)