#pragma once

#pragma warning( disable : 4251 ) // dll-interface warnings
#pragma warning( disable : 4275 ) // dll-interface warning non dll-interface class 'std::exception' used as base for dll-interface class 'gris::muk::MukException'

#include "gstd_api.h"

#include <exception>
#include <string>

#define EXCEPTION(e, ...) e (__LINE__, __FUNCTION__, __FILE__, __VA_ARGS__)
#define GSTD_EXCEPTION(...) EXCEPTION(::gris::gstd::Exception, __VA_ARGS__)

/* requires inclusion of <sstream> */
#define GSTD_EXCEPTION_STREAM(x)\
  GSTD_EXCEPTION([&] { ::std::ostringstream __oss; { __oss << x; } return __oss.str(); } ())
/* requires inclusion of "boost/format.hpp" */
#define GSTD_EXCEPTION_FORMAT(fmt, x)\
  GSTD_EXCEPTION( (boost::format( fmt ) % x ).str() )


// if(!check) instruction (#generated error);
// e.g. GSTD_ASSERT(1 == 2, throw) will throw an error
// or   GSTD_ASSERT(1 == 2, return [](void*) { return true; }) will effectively 'return true'
// or   GSTD_ASSERT(1 == 2, return true; ) does the same, but produces 'unclean' code
#define GSTD_ASSERT(check, instruction) \
if (! check ) { \
  instruction (GSTD_EXCEPTION_STREAM("The assertion '" << #check << "' failed."));\
}
// if(variable == nulltpr) instruction (#generated error);
// e.g. GSTD_ASSERT_INIT(ptr, throw) will throw an error
// or   GSTD_ASSERT_INIT(ptr, return [](void*) { return true; }) will effectively 'return true'
// or   GSTD_ASSERT_INIT(ptr, return true; ) does the same, but produces 'unclean' code
#define GSTD_ASSERT_INIT(variable, instruction) \
if ( variable == nullptr ) { \
    instruction (GSTD_EXCEPTION_STREAM("The variable or return value of '" << #variable << "' is not initialized."));\
}

namespace gris
{
namespace gstd
{
  /**
  */
  class GRIS_GSTD_API Exception : public std::exception
  {
    public:
      explicit Exception(const std::string& message);
      Exception(const unsigned int line, const std::string& function, const std::string& file, const std::string& message );
      Exception(const Exception& other);
      Exception& operator=(const Exception& other);

    public:
      static std::string indent(const std::string& text);

    public:
      virtual const char* what() const;

    public:
      const std::string&           message()  const   { return mMessage; }
      unsigned int                 line()     const   { return mLine; }
      const char*                  function() const   { return mFunction.c_str(); }
      const char*                  file()     const   { return mFilename.c_str(); }

      void                         prepend(const std::string& s);
      void                         append(const std::string& s);

    protected:
      void                         reformatString();

    private:
      std::string mFilename;
      std::string mMessage;
      std::string mFunction;
      unsigned int mLine;

      std::string mFormattedMessage;
  };
}
}