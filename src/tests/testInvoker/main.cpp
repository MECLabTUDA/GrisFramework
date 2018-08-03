#include "tests.h"
#include "testInvoker.h"

#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>

#include <iostream>

//____________________________________________________________________________//

struct MyConfig {
  MyConfig()   { std::cout << "start test suite \n"; }
  ~MyConfig()  { std::cout << "end test suite\n"; }
};

//____________________________________________________________________________//

using namespace gris::gstd;

BOOST_GLOBAL_FIXTURE( MyConfig );

BOOST_AUTO_TEST_CASE( test_case_InvokeLater )
{
  BOOST_CHECK_NO_THROW(gris::gstd::testInvokeLater());
  
  BOOST_CHECK_NO_THROW(gris::gstd::testInvokeLaterException());

  BOOST_CHECK(gris::gstd::testValueInvokeLater());
}

BOOST_AUTO_TEST_CASE(test_case_InvokeLaterStack)
{
  BOOST_CHECK_NO_THROW(gris::gstd::testInvokeLaterTemplate<InvokeLaterStack>());

  BOOST_CHECK(gris::gstd::testValueInvokeStack());
}

BOOST_AUTO_TEST_CASE(test_case_InvokeLaterQueue)
{
  BOOST_CHECK_NO_THROW(gris::gstd::testInvokeLaterTemplate<InvokeLaterQueue>());

  BOOST_CHECK(gris::gstd::testValueInvokeQueue());
}

BOOST_AUTO_TEST_CASE(test_case_InvokeLaterList)
{
  BOOST_CHECK_NO_THROW(gris::gstd::testInvokeLaterList());

  BOOST_CHECK_NO_THROW(gris::gstd::testValueInvokeList());
}