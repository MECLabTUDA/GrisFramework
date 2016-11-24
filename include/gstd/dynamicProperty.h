#ifndef GRIS_DYNAMIC_PROPERTY_H
#define GRIS_DYNAMIC_PROPERTY_H

#include "gstd_api.h"

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <map>


namespace gris {
	namespace gstd  // gris std
	{

    class GRIS_GSTD_API IProperty
    {
      public:
        IProperty() {}
        virtual ~IProperty() {}

	    public:
		    /** \brief Set the value of the parameter. The value is taken in as a string, but converted to the type of that parameter. */
		    virtual bool setValue(const std::string& value) = 0;
		    /** \brief Retrieve the value of the parameter, as a string. */
		    virtual std::string getValue() const = 0;

      protected:
        /** \brief Bool values such as "false" cannot be converted to bool using lexical_cast. We need to
        map those to "0" or "1". */
        template<typename T>
        const std::string& maybeWrapBool(const std::string &value) const
        {
          return boost::is_same<T, bool>::value ? truthValueTo01Str(value) : value;
        }

      private:
        /** \brief Map "false", "False", "FALSE",  "F", "f", "0" to "0" and everything else to "1". */
        static const std::string& truthValueTo01Str(const std::string &value);
    };


    template<typename T>
    class Property : public IProperty
    {
      public:
        /** \brief The type for the 'setter' function for this parameter */
        typedef std::function<void(T)> SetterFn;
        /** \brief The type for the 'getter' function for this parameter */
        typedef std::function<T(void)> GetterFn;

      public:
        Property(const SetterFn& setter, const GetterFn& getter) 
          : IProperty(), mSetter(setter), mGetter(getter)
        {
          if (!mSetter && !mGetter)
            throw std::exception("At least one setter or getter function must be specified for parameter");
        }

        virtual ~Property()
        {
        }

        virtual bool setValue(const std::string& value)
        {
          bool result = true;
          try
          {
            if (mSetter)
              mSetter(boost::lexical_cast<T>(IProperty::maybeWrapBool<T>(value)));
          }
          catch (boost::bad_lexical_cast &e)
          {
            result = false;
            throw std::exception( (boost::format("Invalid value format specified: %s") % e.what() ).str().c_str() );
          }
          return result;
        }

        virtual std::string getValue() const
        {
          if (mGetter)
            try
          {
            return boost::lexical_cast<std::string>(mGetter());
          }
          catch (boost::bad_lexical_cast &e)
          {
            throw std::exception((boost::format("Unable to cast Property to string: %s") % e.what()).str().c_str());
            return "";
          }
          else
            return "";
        }
        
      protected:
        SetterFn mSetter;
        GetterFn mGetter;
    };

    class GRIS_GSTD_API DynamicProperty
    {
      friend class DynamicPropertyManager;

      public:
        DynamicProperty() {}
        ~DynamicProperty() {}
        
      public:
        /** \brief This function declares a parameter \e name, and specifies the \e setter and \e getter functions. */
        template<typename T>
        void declareProperty(const std::string& name, const typename Property<T>::SetterFn& setter,
          const typename Property<T>::GetterFn& getter)
        {
          mProperties.insert(std::make_pair(name, std::make_unique<Property<T>>(setter, getter)));
        }
      
        /** \brief List the names of the known parameters */
        void getProperties(std::vector<std::string>& params) const;

        /** \brief Algorithms in OMPL often have parameters that
        can be set externally. While each algorithm will have
        their own getter and setter functions specifically for
        those parameters, this function allows setting
        parameters generically, for any algorithm that
        declares parameters, by specifying the parameter name
        \e key and its value \e value (both as string, but \e
        value is cast to the type desired by the corresponding
        setter). Under the hood, this calls SpecificParam::setValue().
        This ability makes it easy to automatically configure
        using external sources (e.g., a configuration
        file). The function returns true if the parameter was
        parsed and set successfully and false otherwise. */
        bool setParam(const std::string& key, const std::string& value);

        /** \brief Get the value of the parameter named \e key. Store the value as string in \e value and return true if the parameter was found. Return false otherwise. */
        bool getParam(const std::string& key, std::string& value) const;

        /** \brief Get the number of parameters maintained by this instance */
        std::size_t size() const        { return mProperties.size(); }
                        
      private:
        using PropertyMap = std::map<std::string, std::shared_ptr<IProperty>>;
        PropertyMap mProperties;
    };


	}
}
#endif
