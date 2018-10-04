#include "XmlDocument.h"
#include "Exception.h"

#include <pugixml.hpp>

#include <boost/filesystem.hpp>

#include <iostream>

using namespace std;
using namespace boost::filesystem;

namespace gris
{
  /**
  */
  const char* XmlDocument::ErrorStrings[N_Items] =
  {
    "Not a valid XML document",
    "File not found",
    "Saving failed",
  };

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
  std::unique_ptr<XmlDocument> XmlDocument::read(const std::string& filename)
  {
    path fn(filename);
    if ( ! is_regular_file(fn) )
      throw GSTD_EXCEPTION(ErrorStrings[enFileNotFound]);
    std::unique_ptr<XmlDocument> res( new XmlDocument() );
    pugi::xml_parse_result result = res->mp->doc.load_file(filename.c_str());    
    
    if ( ! result)
      throw GSTD_EXCEPTION(ErrorStrings[enInvalidXmlDocument]);
    return res;
  }

  /**
  */
  std::unique_ptr<XmlDocument> XmlDocument::create(const std::string& rootName)
  {
    std::unique_ptr<XmlDocument> res( new XmlDocument() );
    res->mp->doc.append_child(rootName.c_str());
    return res;
  }

  /**
  */
  void XmlDocument::save(const std::string& filename, XmlDocument& doc)
  {
    bool valid = doc.mp->doc.save_file(filename.c_str());
    if ( ! valid)
      throw GSTD_EXCEPTION(ErrorStrings[enSavingFailed]);
  }

  /**
  */
  XmlNode XmlDocument::getRoot()
  {
    XmlNode result;
    result.setRoot(mp->doc.first_child());
    return result;
  }

  /**
  */
  void XmlDocument::fromString(const std::string& string)  
  {
    mp->doc.load_string(string.c_str());
  }
}
