#include <private/testLogger.pch>

#include <gstd/logger.h>
#include <gstd/GrisFileLogger.h>
#include <iostream>


using namespace gris;

using namespace std;

int main(int argc, char ** argv)
{
  const std::string teststring = "test";
  // cout
  {
    LOG_LINE << teststring;
  }
  // file
  {
    const std::string fn = "test.log";
    GetGrisLogger().setCoutLogging(false);
    GetGrisLogger().setFilename(fn);
    GetGrisLogger().setFileLogging(true);
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs,line);
    if (line != teststring)
      std::cout << "ERROR during logging to file '" << fn << "': " << line << " != " << teststring << std::endl;
    else
      std::cout << "File logging successfull" << std::endl;
  }
  // stream logging
  {
    GetGrisLogger().setCoutLogging(false);
    GetGrisLogger().setFileLogging(false);
    std::stringstream ss;
    GetGrisLogger().addStream(&ss);
    LOG_LINE << teststring;
    if (ss.str() != (teststring + "\n"))
      std::cout << "ERROR during logging to stream: " << ss.str() << " != " << teststring + "\n" << std::endl;
    else
      std::cout << "stream logging successfull" << std::endl;
    GetGrisLogger().removeStream(&ss);
  }
}