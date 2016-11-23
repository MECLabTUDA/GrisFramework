#include "private/gris.pch"
#include "dynamicProperty.h"

#include <boost/format.hpp>

#include <stdlib.h>
#include <algorithm>

namespace gris
{
  namespace gstd
  {
    /**
    */
    const std::string& IProperty::truthValueTo01Str(const std::string& value)
    {
      static const std::string falseValue = "0"; // boost::lexical_cast can only cast "0" to (bool) false 
      static const std::string trueValue  = "1"; // boost::lexical_cast can only cast "1" to (bool) true 
      return (value.empty() || value == falseValue ||
        value == "false" || value == "FALSE" || value == "False" || value == "f" || value == "F") ? falseValue : trueValue;
    }

    void DynamicProperty::getProperties(std::vector<std::string>& props) const
    {
      for (const auto& pair : mProperties)
        props.push_back(pair.first);
    }

    bool DynamicProperty::setParam(const std::string& key, const std::string& value)
    {
      PropertyMap::const_iterator it = mProperties.find(key);
      if (it != mProperties.end())
        return it->second->setValue(value);
      else
      {
        throw std::exception( (boost::format("Parameter '%s' was not found") % key.c_str()).str().c_str() );        
      }
      return true;
    }
    
    bool DynamicProperty::getParam(const std::string& key, std::string& value) const
    {
      PropertyMap::const_iterator it = mProperties.find(key);
      if (it != mProperties.end())
      {
        value = it->second->getValue();
        return true;
      }
      return false;
    }

  }
}