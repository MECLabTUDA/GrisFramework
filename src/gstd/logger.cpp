#include "private/gris.pch"
#include "logger.h"

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
  /**
  */
  Logger::Logger()
  {   
  }

  /**
  */
  Logger::~Logger()
  {
    printTimeStamp(); *this << ": -----------------------------------------------------\n";
    printTimeStamp(); *this << ": ------------------- Log Finished --------------------\n";
    printTimeStamp(); *this << ": -----------------------------------------------------\n";
  }

  /**
  */
  void Logger::init()
  {
    mpStreams.push_back(std::make_shared<DummyCout>());
    printTimeStamp(); *this << ": -----------------------------------------------------\n";
    printTimeStamp(); *this << ": ------------------- New Log Started -----------------\n";
    printTimeStamp(); *this << ": -----------------------------------------------------\n";
  }

  /**
  */
  Logger& Logger::operator<<(const char* str)
  {
    for (auto& pOut : mpStreams)
    {
      *pOut << str;
    }
    return *this;
  }


  Logger& Logger::operator=(Logger& l)
  {    
    l << std::endl;
    return *this;
  }
    
  void Logger::add_ostream(std::shared_ptr<std::ostream> pOut)
  {
    mpStreams.push_back(pOut);
  }

  void Logger::remove_ostream(std::shared_ptr<std::ostream> pOut)
  {
    auto iter = std::find(mpStreams.begin(), mpStreams.end(), pOut);
    mpStreams.erase(iter);
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