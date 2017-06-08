#include "private/test.pch"
#include "tests.h"
#include "TestClass.h"

#include <boost/test/auto_unit_test.hpp>

#include <string>
#include <sstream>

using namespace std;
using namespace gris::gstd;

namespace gris
{
  /**
  */
  void showProperties()
  {
    TestClass test;
    test.printProperties(std::cout);
  }

  /**
  */
  void testDouble()
  {
    TestClass test;
    test.setDouble(42.42);
    std::string value;
    test.getProperty("double", value);
    value.resize(5); // make sure rounding is ignored
    BOOST_CHECK(value == "42.42");    
  }

  void testInt()
  {
    TestClass test;
    const std::string ref = "42";    
    test.setProperty("int", ref);
    std::string value;
    test.getProperty("int", value);
    BOOST_CHECK(value == "42");
  }

  void testSize_t()
  {
    TestClass test;
    const std::string ref = "42";    
    test.setProperty("size_t", ref);
    std::string value;
    test.getProperty("size_t", value);
    BOOST_CHECK(value == "42");
  }

  void testBool()
  {
    try
    {
      TestClass test;
      std::string value;
      // check possibilities for false
      test.setProperty("bool", "0");
      test.getProperty("bool", value);
      BOOST_CHECK(value == "0");
      test.setProperty("bool", "false");
      test.getProperty("bool", value);
      BOOST_CHECK(value == "0");
      test.setProperty("bool", "False");
      test.getProperty("bool", value);
      BOOST_CHECK(value == "0");
      // check possibilities for true
      test.setProperty("bool", "1");
      test.getProperty("bool", value);
      BOOST_CHECK(value == "1");
      test.setProperty("bool", "true");
      test.getProperty("bool", value);
      BOOST_CHECK(value == "1");
      test.setProperty("bool", "True");
      test.getProperty("bool", value);
      BOOST_CHECK(value == "1");
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }
  
  void testVec3d()
  {
    TestClass test;
    const auto input = Vec3d(41,42,43);
    test.setVec3d(input);
    std::string value;
    test.getProperty("Vec3d", value);
    BOOST_CHECK(value == "41 42 43");
    test.setProperty("Vec3d", value);
    auto output = test.getVec3d();
    BOOST_CHECK_EQUAL(input, output);
  }

  void testVecDouble()
  {
    try
    {
      TestClass test;
      const std::string ref = "1.2";
      test.setProperty("VecDouble", ref);
      std::string value;
      test.getProperty("VecDouble", value);
      BOOST_CHECK(value == ref);
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }

  /**
  */
  void testSubClass()
  {
    try
    {  
      TestClass test;
      const std::string key = "SubClassDouble";
      const std::string ref = "42.42";
      test.setProperty(key, ref);
      std::string value;
      test.getProperty(key, value);
      value.resize(5); // ignore rounding errors
      BOOST_CHECK(value == ref);
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }

  /**
  */
  void testSubClassWithPrefix()
  {
    try
    {  
      TestClass test;
      const std::string key = "prefix.SubClassDouble";
      const std::string ref = "42.42";
      test.setProperty(key, ref);
      std::string value;
      test.getProperty(key, value);
      value.resize(5); // ignore rounding errors
      BOOST_CHECK(value == ref);
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }

  /**
  */
  void testSetBadKey()
  {
    try
    {
      TestClass test;
      const std::string key = "nonexistent";
      const std::string ref = "dummy";
      test.setProperty(key, ref);
    }
    catch (std::exception& e)
    {
      BOOST_CHECK( 0==strcmp(e.what(), "setProperty: Property with key 'nonexistent' was not found") );
      throw e;
    }
  }

  const char* badCall()
  {
    std::string tmp = "badKey";
    return tmp.c_str();
  }

  /**
  */
  void testGetBadKey()
  {
    try
    {
      TestClass test;
      const std::string key = "nonexistent";
      std::string value;
      test.getProperty(key, value);
    }
    catch (std::exception& e)
    {
      BOOST_CHECK( std::string("getProperty: Property with key 'nonexistent' was not found") == e.what() );
      throw e;
    }
  }

}