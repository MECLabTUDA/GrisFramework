#include <private/testLogger.pch>

#include <gstd/logger.h>
#include <gstd/GrisFileLogger.h>
#include <iostream>


using namespace gris;

using namespace std;

int main(int argc, char ** argv)
{
  LOG << "blubb1" << std::endl;
  TLOG << "blubb2" << std::endl;
  //auto r = std::make_shared<GrisFileLogger>("logtest.txt");    
  //THE_GRIS_LOGGER.add_ostream(&ss);
  std::stringstream ss;
  THE_GRIS_LOGGER.addStream(&ss);
  LOG_LINE << "blubb line";
  //THE_GRIS_LOGGER.remove_ostream(r);  
  THE_GRIS_LOGGER.removeStream(&ss);
  TLOG_LINE << "blubb line2";

  cout << endl;
  cout << endl;
  cout << ss.str();

}