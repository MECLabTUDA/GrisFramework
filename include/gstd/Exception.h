#pragma once

#pragma warning( disable : 4251 ) // dll-interface warnings
#pragma warning( disable : 4275 ) // dll-interface warning non dll-interface class 'std::exception' used as base for dll-interface class 'gris::muk::MukException'

#include "gstd_api.h"

#include <exception>
#include <string>

#define EXCEPTION(e, ...) e (__LINE__, __FUNCTION__, __FILE__, __VA_ARGS__)
#define GSTD_EXCEPTION(...) EXCEPTION(::gris::gstd::Exception, __VA_ARGS__)

#define GSTD_EXCEPTION_STREAM(x) /* requires inclusion of <sstream> */ \
  [&] { ::std::ostringstream __oss; { __oss << x; } return GSTD_EXCEPTION(__oss.str() ); } ()
#define GSTD_EXCEPTION_FORMAT(fmt, x) /* requires inclusion of "boost/format.hpp" */ \
  GSTD_EXCEPTION( (boost::format( fmt ) % x ).str() )


// if(!check) instruction (#generated error);
// e.g. GSTD_ASSERT(1 == 2, throw) will throw an error
// or   GSTD_ASSERT(1 == 2, return [](void*) { return true; }) will effectively 'return true'
#define GSTD_ASSERT(check, instruction) \
if (! check ) { \
  instruction (GSTD_EXCEPTION_STREAM("The assertion '" << #check << "' failed."));\
}
// if(variable == nulltpr) instruction (#generated error);
// e.g. GSTD_ASSERT(ptr, throw) will throw an error
// or   GSTD_ASSERT(ptr, return [](void*) { return true; }) will effectively 'return true'
#define GSTD_ASSERT_INIT(variable, instruction) \
if ( variable == nullptr ) { \
    instruction (GSTD_EXCEPTION_STREAM("The variable or return value of '" << #variable << "' is not initialized."));\
}

namespace gris
{
namespace gstd
{
  class GRIS_GSTD_API Exception : public std::exception
  {
  public:
    Exception(const unsigned int line, const std::string& function, const std::string& file, const std::string& message );
    Exception(const Exception& other);

  public:
    virtual const char* what() const;

    const std::string&           message()  const   { return mMessage; }
    unsigned int                 line()     const   { return mLine; }
    const char*                  function() const   { return mFunction.c_str(); }
    const char*                  file()     const   { return mFilename.c_str(); }

    void                         reformatString();

  public:
    static std::string indent(const std::string& text);
  private:
    std::string mFilename;
    std::string mMessage;
    std::string mFunction;
    unsigned int mLine;

    std::string mFormattedMessage;
  };
}
}