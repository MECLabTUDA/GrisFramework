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
    reformatString();
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

  void Exception::reformatString()
  {
    std::ostringstream oss;
    oss << mFilename << ":" << mLine << " [" << mFunction << "]:" << std::endl << indent(mMessage) << std::endl;
    mFormattedMessage = oss.str();
  }

  std::string Exception::indent(const std::string & text)
  {
    std::stringstream out;
    out << "  ";
    for (auto it = text.begin(); it != text.end(); ++it)
    {
      out << *it;
      if (*it == '\n')
        out << "  ";
    }
    return out.str();
  }

}
}