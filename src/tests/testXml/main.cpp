#pragma once

#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "tests.h"

//____________________________________________________________________________//

struct MyConfig {
  MyConfig()   { std::cout << "start test suite \n"; }
  ~MyConfig()  { std::cout << "end test suite\n"; }
};

//____________________________________________________________________________//

BOOST_GLOBAL_FIXTURE( MyConfig );

BOOST_AUTO_TEST_CASE( test_case )
{
  BOOST_CHECK_NO_THROW(gris::test::basicReadWrite());
  BOOST_CHECK_NO_THROW(gris::test::testAttributes());
  BOOST_CHECK_NO_THROW(gris::test::testSetValue());
}