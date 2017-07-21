#include "Exception.h"

#include <sstream>

namespace gris
{
namespace gstd
{
  Exception::Exception(const unsigned int line, const std::string & function, const std::string & file, const std::string & message)
    : mLine(line)
    , mFilename(file)
    , mFunction(function)
    , mMessage(message)
    , std::exception()
  {
    std::ostringstream oss (mFunction);
    oss << " (" << mLine << "): " << mMessage;
    mFormattedMessage = oss.str();
  }

  Exception::Exception(const Exception & other)
    : std::exception()
    , mLine(other.mLine)
    , mFunction(other.mFunction)
    , mFilename(other.mFilename)
    , mMessage(other.mMessage)
    , mFormattedMessage(other.mFormattedMessage)
  {
  }

  const char * Exception::what() const
  {
    return mFormattedMessage.c_str();
  }

}
}