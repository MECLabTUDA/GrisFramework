#include "GrisFileLogger.h"

namespace gris
{
  GrisFileLogger::GrisFileLogger(const char* filename)
    : std::ostream(this)    
  {      
    ofs.open( filename, std::ios_base::app);
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
