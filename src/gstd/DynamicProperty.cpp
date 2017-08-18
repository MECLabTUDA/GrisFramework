#include "dynamicProperty.h"

#include <boost/format.hpp>

#include <stdlib.h>
#include <algorithm>

#include "Exception.h"

namespace gris
{
  namespace gstd
  {

    const PropertyDefinition IProperty::DEFAULTS[5] = 
    {
      { PropertyDefinition::Unknown, ""},
      { PropertyDefinition::String, "" },
      { PropertyDefinition::Boolean, "" },
      { PropertyDefinition::Float, "" },
      { PropertyDefinition::Integer, "" }
    };

    template <> 
    const PropertyDefinition* Property<double>::DEFAULT_DEFINITION()
    {
      return &DEFAULTS[3];
    }

    template <>
    const PropertyDefinition* Property<int>::DEFAULT_DEFINITION()
    {
      return &DEFAULTS[4];
    }

    template <>
    const PropertyDefinition* Property<std::string>::DEFAULT_DEFINITION()
    {
      return &DEFAULTS[1];
    }

    template <>
    const PropertyDefinition* Property<bool>::DEFAULT_DEFINITION()
    {
      return &DEFAULTS[0];
    }

    /**
    */
    IProperty::IProperty(const PropertyDefinition& propdef)
      : mPropertyDefinition(propdef)
    {
    }

    /**
    */
    IProperty::~IProperty() 
    {
    }


    /**
    */
    const std::string& IProperty::truthValueTo01Str(const std::string& value)
    {
      static const std::string falseValue = "0"; // boost::lexical_cast can only cast "0" to (bool) false 
      static const std::string trueValue  = "1"; // boost::lexical_cast can only cast "1" to (bool) true 
      return (value.empty() || value == falseValue ||
        value == "false" || value == "FALSE" || value == "False" || value == "f" || value == "F") ? falseValue : trueValue;
    }
    
    bool DynamicProperty::setProperty(const std::string& key, const std::string& value)
    {
      PropertyMap::const_iterator it = mProperties.find(key);
      if (it != mProperties.end())
        return it->second->setValue(value);
      else
      {
        throw std::exception( (boost::format("setProperty: Property with key '%s' was not found") % key.c_str()).str().c_str() );        
      }
      return true;
    }
    
    bool DynamicProperty::getProperty(const std::string& key, std::string& value) const
    {
      PropertyMap::const_iterator it = mProperties.find(key);
      if (it != mProperties.end())
      {
        value = it->second->getValue();
      }
      else
      {
        throw std::exception( (boost::format("getProperty: Property with key '%s' was not found") % key.c_str()).str().c_str() );
      }
      return true;
    }

    void DynamicProperty::getPropertyNames(std::vector<std::string> &props) const
    {
      props.clear();
      props.reserve(mProperties.size());
      for (PropertyMap::const_iterator it = mProperties.begin() ; it != mProperties.end() ; ++it)
        props.push_back(it->first);
    }

    std::vector<std::string>  DynamicProperty::propertyNames() const
    {
      std::vector<std::string>  props(mProperties.size());
      for (PropertyMap::const_iterator it = mProperties.begin(); it != mProperties.end(); ++it)
        props.push_back(it->first);
      return props;
    }

    void DynamicProperty::getPropertyValues(std::vector<std::string> &vals) const
    {
      std::vector<std::string> names;
      getPropertyNames(names);
      vals.resize(names.size());
      for (std::size_t i = 0 ; i < names.size() ; ++i)
        vals[i] = mProperties.find(names[i])->second->getValue();
    }

    const DynamicProperty::PropertyMap& DynamicProperty::getPropertyMap() const
    {
      return mProperties;
    }

    bool DynamicProperty::hasProperty(const std::string &key) const
    {
      return mProperties.find(key) != mProperties.end();
    }

    void DynamicProperty::includeProperty(const DynamicProperty &other, const std::string &prefix)
    {
      const PropertyMap &p = other.getPropertyMap();
      if (prefix.empty())
        for (PropertyMap::const_iterator it = p.begin() ; it != p.end() ; ++it)
          mProperties[it->first] = it->second;
      else
        for (PropertyMap::const_iterator it = p.begin() ; it != p.end() ; ++it)
          mProperties[prefix + "." + it->first] = it->second;
    }

    void DynamicProperty::removeProperty(const std::string &name)
    {
      mProperties.erase(name);
    }

    void DynamicProperty::clearProperties()
    {
      mProperties.clear();
    }

    void DynamicProperty::printProperties(std::ostream &out) const
    {
      for (PropertyMap::const_iterator it = mProperties.begin() ; it != mProperties.end() ; ++it)
        out << it->first << " = " << it->second->getValue() << std::endl;
    }

    IProperty & DynamicProperty::operator[](const std::string & name)
    {
      auto it = mProperties.find(name);
      if (it == mProperties.end())
        throw GSTD_EXCEPTION_FORMAT("The property '%s' does not exist.", name);

      return *(it->second);
    }

    PropertyDefinition::PropertyDefinition(const EnHints hint, const char * descriptor)
      : mHint(hint)
      , mDescriptor(descriptor)
    {
    }

}
}