#include "XmlAttribute.h"

#include <pugixml.hpp>

namespace gris
{
  /**
  */
  XmlAttribute::~XmlAttribute()
  {
  }

  /**
  */
  void XmlAttribute::setValue(const std::string& val)
  {
    mAtt->set_value(val.c_str());
  }

  /**
  */
  const char* XmlAttribute::getValue() const
  {
    return mAtt->value();
  }

  /**
  */
  void XmlAttribute::setName(const std::string& val)
  {
    mAtt->set_name(val.c_str());
  }

  /**
  */
  const char* XmlAttribute::getName() const
  {
    return mAtt->name();
  }

  /**
  */
  std::ostream& XmlAttribute::print(std::ostream& os) const
  {
    return os << mAtt->name() << ": " << mAtt->value();
  }

  /**
  */
  std::ostream& operator<<(std::ostream& os, const XmlAttribute& att)
  {
    return att.print(os);
  }

}