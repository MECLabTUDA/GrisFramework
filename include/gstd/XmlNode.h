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

      ~XmlNode();

    public:
      void            setRoot(pugi::xml_node& root);
      pugi::xml_node* getNode();

    public:
      void                 setName(const char* val);
      const char*          getName() const;
      void                 setValue(const char* val);
      const char*          getValue() const;
      XmlAttribute         getAttribute(const char* name) const;

    public:
      std::vector<XmlNode> getChildren() const;
      XmlNode              getChild    (const char* name) const;
      void                 addChild    (const XmlNode& node);
      XmlNode              addChild    (const char* name);
      void                 addAttribute(const char* name, const char* value="");
      XmlAttribute         addAttribute(const char* name);      

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
      N_Items
    };

    const char* Strings[N_Items] = 
    {
      "Not a valid XML document",
      "File not found!",
      "Child not found!",
      "Attribute not found!",
      "Saving failed!"
    };

    public:
    explicit XmlException(EnType type)
    {
      mType = type;
      mStr = Strings[mType];
    }

    virtual char const* what() const { return mStr.c_str(); }

    private:
    EnType mType;
    std::string mStr;
  };

}

#pragma warning (pop)