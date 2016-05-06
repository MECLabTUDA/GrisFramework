#ifndef GRIS_LOGGER_H
#define GRIS_LOGGER_H

#include "gstd_api.h"

#include <loki/Singleton.h>

#include <ostream>
#include <vector>
#include <memory>

namespace gris
{
  class GRIS_GSTD_API Logger
  {
    public:
      Logger();
      ~Logger();

    public:
      void init();
            
    public:
      typedef std::ostream& (*Function_Pointer_Manipulator)(std::ostream&);

      Logger& operator<<(Logger&);
      Logger& operator<<(const char* str);
      Logger& operator<<(Function_Pointer_Manipulator manip)
      {
        for (auto& pOut : mpStreams)
        {
          *pOut << *manip;
        }
        return *this;
      }
      Logger& operator=(Logger& l);

    public:
      Logger& printTimeStamp();
              
    public:
      void add_ostream(std::shared_ptr<std::ostream> pOut);
      void remove_ostream(std::shared_ptr<std::ostream> pOut);

    private:
      std::vector<std::shared_ptr<std::ostream>> mpStreams;
  };
  
  typedef Loki::SingletonHolder<Logger> GrisLogger;

  #define GLOG     gris::GrisLogger::Instance().printTimeStamp() << ": "
  #define LOG_LINE gris::GrisLogger::Instance() = GLOG
  #define THE_GRIS_LOGGER gris::GrisLogger::Instance()
  
}



#endif