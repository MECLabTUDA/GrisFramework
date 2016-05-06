#include "private/gris.pch"
#include "GrisFileLogger.h"

namespace gris
{
  GrisFileLogger::GrisFileLogger(const char* filename)
    : std::ostream(this)
    , ofs(filename)
  {      
  }

  GrisFileLogger::~GrisFileLogger() 
  {
    ofs.close(); 
  }

  int GrisFileLogger::overflow(int c)
  {
    putc(c);
    return 0;
  }

  void GrisFileLogger::putc(char c)
  {
    ofs << c;
  }

}
