#ifndef GRIS_DYNAMIC_PROPERTY_H
#define GRIS_DYNAMIC_PROPERTY_H

//#include "private/std_api.h"

#include <string>
#include <map>
#include <iostream>
#include <memory>


namespace gris {
	namespace gstd  // gris std
	{

    class BaseProperty
    {
      public:
        virtual ~BaseProperty()
        {
        }
        
        virtual std::ostream& get(std::ostream&) = 0;
        virtual void          set(std::istream&) = 0;
    };

    template<typename T>
    class Property : public BaseProperty
    {
      friend class DynamicPropertyManager;

      public:
        virtual ~Property()
        {
        }

        Property(T& d) : data(d)
        {
        }

        T& getData() { return data; }
                
        virtual std::ostream& get(std::ostream& os) override
        {
          return os << data;
        }

        virtual void set(std::istream& s) override
        {
          s >> data;
        }


      private:
        T& data;
    };

    class DynamicProperty
    {
      friend class DynamicPropertyManager;

      public:
        template<typename T>
        void append(const std::string& tag, T& data)
        {
          std::shared_ptr<BaseProperty> pProp (new Property<T>(data));        
          props.insert(propsType_t::value_type(tag, pProp));
        }
        
      private:
        typedef std::map<const std::string, std::shared_ptr<BaseProperty>> propsType_t;
        propsType_t props;
    };
    
	}
}


#endif
