#include "private/gris.pch"
#include "XmlDocument.h"

#include <pugixml.hpp>

#include <boost/filesystem.hpp>
#include <iostream>

using namespace std;
using namespace boost::filesystem;

namespace gris
{
  /**
  */
  struct XmlDocument::Impl 
  {
    Impl() {}
    ~Impl() {}

    pugi::xml_document doc;
  };

  /**
  */
  XmlDocument::XmlDocument()
    : mp (std::make_unique<Impl>())
  {
  }

  /**
  */
  XmlDocument::~XmlDocument()
  {
  }

  /**
  */
  std::unique_ptr<XmlDocument> XmlDocument::read(const char* filename)
  {
    path fn(filename);
    if ( ! is_regular_file(fn) )
      throw XmlException(XmlException::FileNotFound);
    auto res = std::make_unique<XmlDocument>();
    pugi::xml_parse_result result = res->mp->doc.load_file(filename);    

    if ( ! result)
      throw XmlException(XmlException::InvalidXmlDocument);    
    return res;
  }

  /**
  */
  std::unique_ptr<XmlDocument> XmlDocument::create(const char* rootName)
  {
    auto res = std::make_unique<XmlDocument>();
    res->mp->doc.append_child(rootName);
    return res;
  }

  /**
  */
  void XmlDocument::save(const char* filename, XmlDocument& doc)
  {
    bool valid = doc.mp->doc.save_file(filename);
    if ( ! valid)
      throw XmlException(XmlException::SavingFailed);
  }

  /**
  */
  XmlNode XmlDocument::getRoot()
  {
    XmlNode result;
    result.setRoot(mp->doc.first_child());
    return result;
  }
}
