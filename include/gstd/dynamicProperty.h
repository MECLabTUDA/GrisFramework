#ifndef GRIS_DYNAMIC_PROPERTY_H
#define GRIS_DYNAMIC_PROPERTY_H

#include "gstd_api.h"

#include <string>
#include <map>
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>


namespace gris {
	namespace gstd  // gris std
	{

    class IProperty
    {
      public:
        IProperty() {}
        virtual ~IProperty() {}

      public:
        virtual std::ostream& operator<<(std::ostream& os) = 0;
        virtual void          operator>>(std::istream& is) = 0;
    };


    template<typename T>
    class Property : public IProperty
    {
      friend class DynamicPropertyManager;

      public:        
        explicit Property(T& d) : data(d) {}               
                
        virtual std::ostream& operator<<(std::ostream& os) override
        {
          return os << data;
        }

        virtual void operator>>(std::istream& is) override
        {
          is >> data;
        }

      private:
        T& data;
    };

    class GRIS_GSTD_API DynamicProperty
    {
      friend class DynamicPropertyManager;

      public:
          DynamicProperty() {}
          ~DynamicProperty() {}
          DynamicProperty(const DynamicProperty&);
          DynamicProperty(const DynamicProperty&&);
          DynamicProperty& operator=(const DynamicProperty&);

      public:
        void swap(DynamicProperty& o);

      public:
        template<typename T>
        void append(const char* tag, T& data)
        {
          std::pair<std::string, std::unique_ptr<IProperty>> pair
            = std::make_pair(tag, std::make_unique<Property<T>>(data));          
          mProperties.insert(std::move(pair));
        }

        std::vector<const char*> getProperties() const;

        void get(const char* name, std::ostream& os) const;
        void set(const char* name, const char* val);
        
                
      private:
        typedef std::map<std::string, std::unique_ptr<IProperty>> PropertyMap;
        PropertyMap mProperties;
    };


	}
}

#include "dynamicPropertyTemplates.hxx"

#endif
