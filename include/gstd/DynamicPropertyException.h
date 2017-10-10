#pragma once

#pragma warning( disable : 4251 ) // dll-interface warnings
#pragma warning( disable : 4275 ) // dll-interface warning non dll-interface class 'std::exception' used as base for dll-interface class 'gris::muk::MukException'

#include "gstd_api.h"

#include "Exception.h"

namespace gris
{
namespace gstd
{
  class GRIS_GSTD_API DynamicPropertyException : public Exception
  {
  public:
    enum Type
    {
      INVALID,
      KEY_DOES_NOT_EXIST,
      KEY_ALREADY_EXISTS,
      GETTER_SETTER_UNDEFINED,
      CONVERSION_FAILED
    };

  public:
    DynamicPropertyException(const unsigned int line, const std::string& function, const std::string& file, const Type& type, const std::string& message );
    DynamicPropertyException(const DynamicPropertyException& other);

  public:
    Type type() const;
  public:
  private:
    Type mType;
  };
}
}