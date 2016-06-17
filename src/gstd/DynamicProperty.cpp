#include "private/gris.pch"
#include "dynamicProperty.h"

#include <boost/format.hpp>

#include <stdlib.h>
#include <algorithm>

namespace gris
{
  namespace gstd
  {
    //typedef std::map<std::string, std::unique_ptr<IProperty>> PropertyMap;
    //PropertyMap mProperties;

    DynamicProperty::DynamicProperty(const DynamicProperty& o)
    {
      /*std::for_each(o.mProperties.begin(), o.mProperties.end(), [&] (const auto& pair)
      {
        mProperties.insert(std::make_pair(pair.first, std::make_unique<*pair.second));
      });*/
    }

    DynamicProperty::DynamicProperty(const DynamicProperty&& o)
      //: mProperties(std::move(o.mProperties))
    {
    }

    DynamicProperty& DynamicProperty::operator=(const DynamicProperty& o)
    {
     // swap(o);
      return *this;
    }

    void DynamicProperty::swap(DynamicProperty& o)
    {
    }

    void DynamicProperty::get(const char* name, std::ostream& os) const
    {
      auto iter = mProperties.find(name);
      if ( mProperties.end() == iter )
      {
        throw std::exception( (boost::format("no such key: %s") % name).str().c_str() );
      }
      *(iter->second.get()) << os;
    }

    void DynamicProperty::set(const char* name, const char* val)
    {
      auto iter = mProperties.find(name);
      if ( mProperties.end() == iter )
      {
        throw std::exception( (boost::format("no such key: %s") % name).str().c_str() );
      }
      std::istringstream is(val);
      *(iter->second.get()) >> is;
    }

    std::vector<const char*> DynamicProperty::getProperties() const
    {
      std::vector<const char*> props;
      for (const auto& pair : mProperties)
        props.push_back(pair.first.c_str());
      return props;
    }
  }
}