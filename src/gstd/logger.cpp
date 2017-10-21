#include "logger.h"
#include "GrisFileLogger.h"

#include <loki/Singleton.h>

#pragma warning(push)
#pragma warning(disable: 4996)
#include <time.h>
#include <iostream>

namespace
{
  class DummyCout : public std::ostream, std::streambuf
  {
    public:
      DummyCout()
        : std::ostream(this)
      {}

      int   overflow(int c)
      {
        putc(c);
        return 0;
      }

      void  putc(char c)
      {
        std::cout << c;
      }
  };
}

namespace gris
{

  GRIS_GSTD_API Logger& GetGrisLogger()
  {
    return Loki::SingletonHolder<Logger>::Instance();
  }

  /**
  */
  Logger::Logger()
  {
    init();    
  }

  /**
  */
  Logger::~Logger()
  {
    printTimeStamp(); *this << ": -----------------------------------------------------\n";
    printTimeStamp(); *this << ": ------------------- Log Finished --------------------\n";
    printTimeStamp(); *this << ": -----------------------------------------------------\n";
    flush();
  }

  /**
  */
  void Logger::init()
  {
    mOfs.open("log/log.txt", std::ios_base::trunc);
    mOfs.close();
    printTimeStamp(); *this << ": -----------------------------------------------------\n";
    printTimeStamp(); *this << ": ------------------- New Log Started -----------------\n";
    printTimeStamp(); *this << ": -----------------------------------------------------\n";
    flush();
  }

  /**
  */
  Logger& Logger::operator<<(const char* str)
  {
    mBuffer << str;
    return *this;
  }

  Logger& Logger::operator<<(const std::string& str)
  {
    mBuffer << str;
    return *this;
  }


  Logger& Logger::operator=(Logger& l)
  {    
    l << std::endl;
    flush();
    return *this;
  }
  
  void Logger::addStream(std::ostream* pOut)
  {
    mpStreams.push_back(pOut);
  }

  void Logger::removeStream(std::ostream* pOut)
  {
    auto iter = std::find(mpStreams.begin(), mpStreams.end(), pOut);
    if (iter!=mpStreams.end())
      *this << "ostream removed\n";
    mpStreams.erase(iter);
  }


  void Logger::addCallback(LoggerCallback* cb)
  {
    mpCallbacks.push_back(cb);
  }

  void Logger::removeCallback(LoggerCallback* cb)
  {
    auto iter = std::find(mpCallbacks.begin(), mpCallbacks.end(), cb);
    if (iter!=mpCallbacks.end())
      *this << "callback removed\n";
    mpCallbacks.erase(iter);
  }

  /**
  */
  void Logger::flush()
  {
    for (auto& pOut : mpStreams)
    {
      if (pOut)
        *pOut << mBuffer.str();
    }
    for (auto& pOut : mpCallbacks)
    {
      if (pOut)
        pOut->stream(mBuffer.str().c_str());
    }
    if (mUseCout)
    {
      std::cout << mBuffer.str();
    }
    if (mUseLogFile)
    {
      mOfs.open("log.txt", std::ios_base::app);
      mOfs << mBuffer.str();
      mOfs.close();
    }
    mBuffer.str("");
  }

  Logger& Logger::printTimeStamp()
  {
    time_t now;
    time(&now);
    struct tm * info;    
    info = localtime(&now);
    const size_t StrLength = 21;
    char buffer[StrLength];
    strftime(buffer, StrLength, "%F %X", info);
    *this << buffer;
    return *this;
  }
}


#pragma warning(pop)