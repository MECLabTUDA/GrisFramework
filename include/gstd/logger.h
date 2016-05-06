#ifndef GRIS_LOGGER_H
#define GRIS_LOGGER_H

#include "gstd_api.h"

#include <loki/Singleton.h>

#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>


namespace gris
{
  
  
  class GRIS_GSTD_API FileLogger
  {
    public:
      FileLogger() : m_logger(std::cout) { init("log.txt"); }
      explicit FileLogger(const std::string& logFilename) : m_logger(std::cout)
      {
        init(logFilename);
      }
      ~FileLogger() 
      {
        print( "==================================================" );
        flush();
        print( "================== Log Finished ==================" );
        flush();
        print( "==================================================" );
        flush();
        print( "" );
        flush();
        print( "" );
        flush();
        print( "" );
        flush();
        m_fs.close(); 
      }

      FileLogger& operator<< (const std::string& s)
      {         
        print(s);
        return *this; 
      }

      FileLogger& operator=(FileLogger& fl)
      {
        fl.flush();
        return *this;
      }
      
      void flush()
      {        
        time_t      now;
        struct tm * info;
        time(&now);
        info = localtime(&now);
        char buffer[23];
        strftime(buffer,23,"%F %X : ",info);
        m_fs     << buffer << m_ss.str() << "\n";
        m_logger << buffer << m_ss.str() << "\n";
        m_ss.str(std::string());
      }


    private:
      void init(const std::string& logFilename)
      {
        m_fs.open(logFilename.c_str(), std::fstream::app);
        print( "" );
        flush();
        print( "" );
        flush();
        print( "" );
        flush();
        print( "==================================================" );
        flush();
        print( "================== New Log =======================" );
        flush();
        print( "==================================================" );
        flush();
        print( "" );      
        flush();
      }

      void print(const std::string& text)
      {
        m_ss << text.c_str();
      }

    private:      
      std::stringstream m_ss;
      std::ostream&  m_logger;
      std::ofstream  m_fs;
  };
    
  typedef Loki::SingletonHolder<FileLogger> DefaultLogger;

  #define LOG_INFO TheLogger::Instance() = TheLogger::Instance()
}



#endif