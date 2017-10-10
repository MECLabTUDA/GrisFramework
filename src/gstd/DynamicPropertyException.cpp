#include "DynamicPropertyException.h"

#include <sstream>

namespace gris
{
namespace gstd
{
  DynamicPropertyException::DynamicPropertyException(const unsigned int line, const std::string & function, const std::string & file, const Type& type, const std::string & message)
    : Exception(line, file, function, message)
    , mType(type)
  {
    reformatString();
  }

  DynamicPropertyException::DynamicPropertyException(const DynamicPropertyException & other)
    : Exception(other)
  {
  }

  DynamicPropertyException::Type DynamicPropertyException::type() const
  {
    return mType;
  }

}
}