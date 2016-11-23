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
//
//namespace gris
//{
//  namespace gstd
//  {
//    /*void operator>>(std::istream& is, Vec3d& vec)
//    {
//      std::string str;
//      is >> str;
//      std::stringstream ss(str);
//      ss >> vec.x();
//      ss >> vec.y();
//      ss >> vec.z();
//    }*/
//    std::ostream& operator<<(std::ostream& os, bool b)
//    {
//      if (b)
//        os << "true";
//      else
//        os << "false";
//      return os;
//    }
//
//    std::istream& operator>>(std::istream& is, bool& b)
//    {
//      std::string s;
//      is >> s;
//      if (0 == strcmp(s.c_str(), "true")
//        || 0 == strcmp(s.c_str(), "True")
//        || 0 == strcmp(s.c_str(), "1"))
//        b = true;
//      else
//        b = false;
//      return is;
//    }
//  }
//}

namespace gris
{
  void testDouble()
  {
    TestClass test;
    test.setDouble(42.42);
    std::string value;
    test.getParam("double", value);
    value.resize(5); // make sure rounding is ignored
    std::cout << value << std::endl;
    BOOST_CHECK(equals("42.42", value));    
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
      std::cout << "value " << value << std::endl;
      BOOST_CHECK(equals("0", value));
      test.setParam("bool", "false");
      test.getParam("bool", value);
      std::cout << "value " << value << std::endl;
      BOOST_CHECK(equals("0", value));
      test.setParam("bool", "False");
      test.getParam("bool", value);
      std::cout << "value " << value << std::endl;
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
    //value.resize(5); // make sure rounding is ignored
    std::cout << value << std::endl;
    BOOST_CHECK(equals("41 42 43", value));
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