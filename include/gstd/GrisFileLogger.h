#pragma once

#include "gstd_api.h"

#include <fstream>

namespace gris
{
  class GRIS_GSTD_API GrisFileLogger : public std::ostream, std::streambuf
  {
    public:
      GrisFileLogger(const char* filename);
      ~GrisFileLogger();

      int   overflow(int c);
      void  putc(char c);

    private:
      std::ofstream ofs;
  };

}
