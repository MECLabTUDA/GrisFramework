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
    class GRIS_GSTD_API PropertyDefinition
    {
    public:
      enum EnHints
      {
        Unknown,
        String,
        Boolean,
        Float,
        Integer,
        Color, 
        Vector3,
        Matrix4x4,
        Path
      };
    public:
      PropertyDefinition(const EnHints hint = EnHints::String, const char* descriptor = "");
      virtual ~PropertyDefinition() {}

    public:
      const PropertyDefinition::EnHints hint()       const { return mHint; }
      const char*                       descriptor() const { return mDescriptor; }

      std::string hintDescriptor() const { return descriptor(); }

    protected:
      const EnHints  mHint;
      const char*    mDescriptor;
    };

    class GRIS_GSTD_API IProperty
    {
      public:
        IProperty(const PropertyDefinition& propdef);
        virtual ~IProperty();

	    public:
		    /** \brief Set the value of the parameter. The value is taken in as a string, but converted to the type of that parameter. */
		    virtual bool setValue(const ::std::string& value) = 0;
		    /** \brief Retrieve the value of the parameter, as a string. */
		    virtual ::std::string getValue() const = 0;

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
        static const ::std::string& truthValueTo01Str(const ::std::string &value);

    public:
        const PropertyDefinition::EnHints hint()       const { return mPropertyDefinition.hint(); }
        const char*                       descriptor() const { return mPropertyDefinition.descriptor(); }

        std::string hintDescriptor() const { return descriptor(); }


    protected:
      PropertyDefinition mPropertyDefinition;

      static const PropertyDefinition DEFAULTS[5];
    };


    template<typename T>
    class Property : public IProperty
    {
    public:
      static const PropertyDefinition* DEFAULT_DEFINITION() { return &DEFAULTS[0]; }

    public:
      /** \brief The type for the 'setter' function for this parameter */
      typedef std::function<void(T)> SetterFn;
      /** \brief The type for the 'getter' function for this parameter */
      typedef std::function<T(void)> GetterFn;

    public:
      Property(const SetterFn& setter, const GetterFn& getter, const PropertyDefinition* propdef = Property<T>::DEFAULT_DEFINITION())
        // dont allow nullptr for PropertyDefinition
        : IProperty(propdef == nullptr ? *DEFAULT_DEFINITION() : *propdef)
        , mSetter(setter), mGetter(getter)
      {
        if (!mSetter && !mGetter)
          throw std::exception("One setter or getter function must be specified for parameter");
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
          throw std::exception((boost::format("Unable to set property from string: '%s' (error: %s)") % value % e.what()).str().c_str());
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
        catch (boost::bad_lexical_cast& e)
        {
          throw std::exception((boost::format("Unable to cast property to a string (error: %s)") % e.what()).str().c_str());
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
      public:
        DynamicProperty() {}
        virtual ~DynamicProperty() {}

        DynamicProperty(const DynamicProperty&) {}
        DynamicProperty& operator=(const DynamicProperty&) { return *this; }
        DynamicProperty(DynamicProperty&&) {}
        DynamicProperty& operator=(DynamicProperty&&) { return *this; }
        
      public:
        /** \brief This function declares a parameter \e name, and specifies the \e setter and \e getter functions. */
        template<typename T>
        void declareProperty(const std::string& name, const typename Property<T>::SetterFn& setter,
          const typename Property<T>::GetterFn& getter, const PropertyDefinition* propdef = nullptr)
        {
          mProperties.insert(
            std::make_pair(name, 
              std::make_unique<Property<T>>(setter, getter, 
                propdef ? propdef : Property<T>::DEFAULT_DEFINITION())));
        }

        /** \brief Add a parameter to the set */
        void addProperty(const std::shared_ptr<IProperty> & prop);

        /** \brief Remove a parameter from the set */
        void removeProperty(const std::string &name);

        /** \brief Include the params of a different ParamSet into this one. Optionally include a prefix for each of the parameters */
        void includeProperty(const DynamicProperty& other, const std::string& prefix = "");
      
        /** \brief List the names of the known parameters */
        void getPropertyNames(std::vector<std::string>& params) const;

        /** \brief List the names of the known parameters */
        std::vector<std::string>  propertyNames() const;

        /** \brief List the values of the known parameters, in the same order as getParamNames() */
        void getPropertyValues(std::vector<std::string> &vals) const;

        /** \brief Get the map from parameter names to parameter descriptions */
        const std::map<std::string, std::shared_ptr<IProperty>>& getPropertyMap() const;

        /** \brief Tries to cast the string \e value to the specific Property type T. returns true if the cast succeeds, returns false otherwise. */
        bool setProperty(const std::string& key, const std::string& value);

        /** \brief Get the value of the parameter named \e key. Store the value as string in \e value and return true if the parameter was found. Return false otherwise. */
        bool getProperty(const std::string& key, std::string& value) const;

        /** \brief Check whether this set of parameters includes the parameter named \e key */
        bool hasProperty(const std::string &key) const;

        /** \brief Get the number of parameters maintained by this instance */
        std::size_t numberOfProperties() const { return mProperties.size(); }

        /** \brief Clear all the set parameters */
        void clearProperties();

        /** \brief Print the parameters to a stream */
        void printProperties(std::ostream &out) const;

        IProperty& operator[](const std::string& name);
                        
      private:
        using PropertyMap = std::map<std::string, std::shared_ptr<IProperty>>;
        PropertyMap mProperties;
    };


	}
}
#endif
