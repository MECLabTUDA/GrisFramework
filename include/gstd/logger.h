#ifndef GRIS_LOGGER_H
#define GRIS_LOGGER_H

#include "gstd_api.h"

#include <loki/Singleton.h>

#include <ostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>

namespace gris
{
  class GRIS_GSTD_API LoggerCallback
  {
    public:
    LoggerCallback() {};
    virtual ~LoggerCallback() {};

    virtual void stream(const char*) = 0;
  };

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
      Logger& operator<<(const std::string& str);
      
      Logger& operator<<(Function_Pointer_Manipulator manip)
      {
        mBuffer << manip;
        return *this;
      }

      template<typename T>
      Logger& operator<<(const T& str)
      {
        mBuffer << str;
        return *this;
      }

      Logger& operator=(Logger& l);

    public:
      Logger& printTimeStamp();

    private:
      void flush();
              
    public:
      //void add_ostream(std::shared_ptr<std::ostream> pOut);
      //void remove_ostream(std::shared_ptr<std::ostream> pOut);
      void addStream(std::ostream* pOut);
      void removeStream(std::ostream* pOut);

      void addCallback(LoggerCallback* cb);
      void removeCallback(LoggerCallback* cb);

    private:
      //std::vector<std::shared_ptr<std::ostream>> mpStreams;
      std::vector<std::ostream*> mpStreams;
      std::vector<LoggerCallback*> mpCallbacks;
      std::ofstream mOfs;
      std::stringstream mBuffer;
  };

  GRIS_GSTD_API Logger& GetGrisLogger();
  
  #define LOG       gris::GetGrisLogger()
  #define LOG_LINE  gris::GetGrisLogger() = LOG
  #define TLOG      gris::GetGrisLogger().printTimeStamp() << ": "
  #define TLOG_LINE gris::GetGrisLogger() = TLOG
  #define THE_GRIS_LOGGER gris::GetGrisLogger()
  
}



#endif