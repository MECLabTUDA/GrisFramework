#include "private/test.pch"
#include "tests.h"

#include <boost/test/auto_unit_test.hpp>

#include <string>
#include <sstream>

using namespace std;
using namespace gris;

namespace
{
  bool equals(const std::string& lhs, const std::string& rhs)
  {
    return 0 == lhs.compare(rhs);
  }
}


namespace gris
{
  void testDouble()
  {
    TestClass test;
    test.setDouble(42.42);
    std::string value;
    test.getParam("double", value);
    value.resize(5); // make sure rounding is ignored
    BOOST_CHECK(equals("42.42", value));    
  }

  void testInt()
  {
    TestClass test;
    const std::string ref = "42";    
    test.setParam("int", ref);
    std::string value;
    test.getParam("int", value);
    BOOST_CHECK(equals("42", value));
  }

  void testSize_t()
  {
    TestClass test;
    const std::string ref = "42";    
    test.setParam("size_t", ref);
    std::string value;
    test.getParam("size_t", value);
    BOOST_CHECK(equals("42", value));
  }

  void testBool()
  {
    try
    {
      TestClass test;
      std::string value;
      // check possibilities for false
      test.setParam("bool", "0");
      test.getParam("bool", value);
      BOOST_CHECK(equals("0", value));
      test.setParam("bool", "false");
      test.getParam("bool", value);
      BOOST_CHECK(equals("0", value));
      test.setParam("bool", "False");
      test.getParam("bool", value);
      BOOST_CHECK(equals("0", value));
      // check possibilities for true
      test.setParam("bool", "1");
      test.getParam("bool", value);
      BOOST_CHECK(equals("1", value));
      test.setParam("bool", "true");
      test.getParam("bool", value);
      BOOST_CHECK(equals("1", value));
      test.setParam("bool", "True");
      test.getParam("bool", value);
      BOOST_CHECK(equals("1", value));
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }
  
  void testVec3d()
  {
    using namespace gstd;
    TestClass test;
    test.setVec3d(Vec3d(41,42,43));
    std::string value;
    test.getParam("Vec3d", value);
    BOOST_CHECK(equals("41 42 43", value));
  }

  void testVecDouble()
  {
    try
    {
      using namespace std;
      std::vector<double> v(1,1);
      std::cout << "v " << v << endl;
      v.clear();
      std::string str("1.0");
      std::istringstream iss(str);
      iss >> v;
      std::cout << "v " << v << endl;

      using namespace gstd;
      TestClass test;
      const std::string ref = "1.2";
      std::cout << "ref " << ref << std::endl;
      test.setParam("VecDouble", ref);
      std::string value;
      test.getParam("VecDouble", value);
      std::cout << "value " << value << std::endl;
      BOOST_CHECK(equals(ref, value));
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }

  void testKeys()
  {
    try
    {
      /*TestClass t;
      std::vector<std::string> keysIst;
      t.getProperties(keysIst);
      std::vector<std::string> keysSoll = {"x", "valid", "double vector" };
      if (keysIst.size() != keysSoll.size())
        throw std::exception("Error: Sizes of key-vectors are not equal!");
      for (size_t i(0); i<keysIst.size(); ++i)
      {
        BOOST_CHECK_EQUAL(true, keysSoll.end() != std::find_if(keysSoll.begin(), keysSoll.end(), [&] (const auto& obj) { return 0 == keysIst[i].compare(obj); }));
      }  */    
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << endl;
      throw e;
    }    
  }

  /**
  */
  void testBadKey()
  {
    try
    {
    
    }
    catch (std::exception& e)
    {      
      BOOST_CHECK( 0==strcmp(e.what(), "no such key") );
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
  void testInvalidChar()
  {
    try
    {
      
    }
    catch (std::exception& e)
    {
      BOOST_CHECK( 0==strcmp(e.what(), "no such key") );
      throw e;
    }
  }

}