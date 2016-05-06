#include <private/testLogger.pch>

#include <gstd/logger.h>

using namespace gris;

typedef DefaultLogger TheLogger;

int main(int argc, char ** argv)
{  
  LOG_INFO << "test";
  LOG_INFO << "test" << " double";
}