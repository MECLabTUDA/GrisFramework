#pragma once

#include "gstd_api.h"

#include <memory>
#include <ostream>

namespace pugi
{
  class xml_attribute;
}

namespace gris
{
  class XmlNode;
  /**
  */
  class GRIS_GSTD_API XmlAttribute
  {
    friend class XmlNode;

    private:
      XmlAttribute(pugi::xml_attribute* a) : mAtt(a) {}

    public:
     ~XmlAttribute();
      
    public:
      void        setValue(const char* val);
      const char* getValue() const;

      void        setName(const char* val);
      const char* getName() const;

    public:
      std::ostream& print(std::ostream& os) const;

    private:
      pugi::xml_attribute* mAtt;
  };

  GRIS_GSTD_API std::ostream& operator<<(std::ostream& os, const XmlAttribute& att);
}
