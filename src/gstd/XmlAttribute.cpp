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
  void XmlAttribute::setValue(const char* val)
  {
    mAtt->set_value(val);
  }

  /**
  */
  const char* XmlAttribute::getValue() const
  {
    return mAtt->value();
  }

  /**
  */
  void XmlAttribute::setName(const char* val)
  {
    mAtt->set_name(val);
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