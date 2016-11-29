#include "private/test.pch"
#define BOOST_TEST_MODULE test
#include <boost/test/included/unit_test.hpp>
#include <iostream>

#include "tests.h"
#include "testDynamicPropertyManager.h"

//____________________________________________________________________________//

struct MyConfig {
  MyConfig()   { std::cout << "start test suite \n"; }
  ~MyConfig()  { std::cout << "end test suite\n"; }
};

//____________________________________________________________________________//

using namespace gris::gstd;

BOOST_GLOBAL_FIXTURE( MyConfig );

BOOST_AUTO_TEST_CASE( test_case_DynamicProperty )
{
  BOOST_CHECK_NO_THROW(gris::showProperties());

  BOOST_CHECK_NO_THROW(gris::testDouble());
  BOOST_CHECK_NO_THROW(gris::testInt());
  BOOST_CHECK_NO_THROW(gris::testSize_t());
  BOOST_CHECK_NO_THROW(gris::testBool());
  BOOST_CHECK_NO_THROW(gris::testVec3d());
  BOOST_CHECK_NO_THROW(gris::testVecDouble());

  BOOST_CHECK_NO_THROW(gris::testSubClass());
  BOOST_CHECK_NO_THROW(gris::testSubClassWithPrefix());
  
  BOOST_CHECK_THROW(gris::testSetBadKey(), std::exception);
  BOOST_CHECK_THROW(gris::testGetBadKey(), std::exception);
}

BOOST_AUTO_TEST_CASE(test_case_DynamicPropertyManager)
{
  BOOST_CHECK_NO_THROW(testInitializeManager());
  BOOST_CHECK_THROW(testLoginWithoutInitialization(), std::exception);
  BOOST_CHECK_NO_THROW(testLogin());
  BOOST_CHECK_NO_THROW(testLoginAfterConnect());
}