#include "tests.h"

#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>

#include <iostream>

//____________________________________________________________________________//

struct MyConfig 
{
  MyConfig()   { std::cout << "start test suite \n"; }
  ~MyConfig()  { std::cout << "end test suite\n"; }
};

//____________________________________________________________________________//

BOOST_GLOBAL_FIXTURE( MyConfig );

using namespace gris;

BOOST_AUTO_TEST_CASE( test_case_Vectors )
{
  BOOST_CHECK_NO_THROW(gris::testBaseVector());
  BOOST_CHECK_NO_THROW(gris::testDiscreteVector());
  BOOST_CHECK_NO_THROW(gris::testFieldVector());
}
