#include "Exception.h"

#include <sstream>

namespace gris
{
namespace gstd
{
  Exception::Exception(const std::string& message)
    : mMessage(message)
  {
    reformatString();
  }

  Exception::Exception(const unsigned int line, const std::string & function, const std::string & file, const std::string & message)
    : mLine(line)
    , mFilename(file)
    , mFunction(function)
    , mMessage(message)
  {
    reformatString();
  }

  Exception::Exception(const Exception & o)
    : mLine(o.mLine)
    , mFunction(o.mFunction)
    , mFilename(o.mFilename)
    , mMessage(o.mMessage)
    , mFormattedMessage(o.mFormattedMessage)
  {
  }

  Exception& Exception::operator=(const Exception& o)
  {
    if (this != &o)
    {
      mLine = o.mLine;
      mFunction = o.mFunction;
      mFilename = o.mFilename;
      mMessage = o.mMessage;
      mFormattedMessage = o.mFormattedMessage;
    }
    return *this;
  }

  const char * Exception::what() const
  {
    return mFormattedMessage.c_str();
  }

  void Exception::prepend(const std::string & s)
  {
    mMessage = s + mMessage;
    reformatString();
  }

  void Exception::append(const std::string & s)
  {
    mMessage += s;
    reformatString();
  }

  void Exception::reformatString()
  {
    std::ostringstream oss;
    if ( ! mFilename.empty())
      oss << mFilename << ":" << mLine << " ";
    oss << "[" << mFunction << "]:" << std::endl << indent(mMessage) << std::endl;
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