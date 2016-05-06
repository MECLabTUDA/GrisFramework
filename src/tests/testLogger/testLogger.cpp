#include <private/testLogger.pch>

#include <gstd/logger.h>
#include <gstd/GrisFileLogger.h>


using namespace gris;



int main(int argc, char ** argv)
{
  GLOG << "blubb1" << std::endl;  
  THE_GRIS_LOGGER.init();
  GLOG << "blubb2" << std::endl;
  auto r = std::make_shared<GrisFileLogger>("logtest.txt");  
  THE_GRIS_LOGGER.add_ostream(r);
  LOG_LINE << "blubb line";
  THE_GRIS_LOGGER.remove_ostream(r);
  LOG_LINE << "blubb line2";
}