#include <private/test.pch>
#include "tests.h"

#include <boost/test/auto_unit_test.hpp>

#include <string>
#include <sstream>

using namespace std;
using namespace gris;

namespace gris
{
  namespace gstd
  {
    /*void operator>>(std::istream& is, Vec3d& vec)
    {
      std::string str;
      is >> str;
      std::stringstream ss(str);
      ss >> vec.x();
      ss >> vec.y();
      ss >> vec.z();
    }*/
  }
}

namespace gris
{
  void testDouble()
  {
    TestClass t;
    const char* name = "x";
    std::cout << "testing variable '" << name << "'" << endl;
    {      
      std::ostringstream os;
      t.get(name, os);
      std::cout << name << ": " << os.str() << std::endl;
      t.set(name, "42");
      os.str("");
      t.get(name, os);
      std::cout << name << ": " << os.str() << std::endl;
    }
    std::cout << endl;
  }

  void testBool()
  {
    TestClass t;
    const char* name = "valid";
    std::cout << "testing variable '" << name << "'" << endl;
    {      
      std::ostringstream os;
      t.get(name, os);
      std::cout << name << ": " << os.str() << std::endl;
      BOOST_CHECK_EQUAL(os.str(), "false");

      t.set(name, "true");
      os.str("");
      t.get(name, os);
      std::cout << name << ": " << os.str() << std::endl;
      BOOST_CHECK_EQUAL(os.str(), "true");
    }
    std::cout << endl;
  }
  
  void testVec3d()
  {
    TestClass t;
    const char* name = "double vector";
    std::cout << "testing variable '" << name << "'" << endl;
    {      
      std::ostringstream os;
      t.get(name, os);
      std::cout << name << ": " << os.str() << std::endl;
      BOOST_CHECK_EQUAL(os.str(), "0 0 0");

      const char* values = "1.5 2.005 -20.11";
      t.set(name, values);
      os.str("");
      t.get(name, os);
      std::cout << name << ": " << os.str() << std::endl;
      BOOST_CHECK_EQUAL(os.str(), values);
    }
    std::cout << endl;
  }

  void testKeys()
  {
    try
    {
      TestClass t;
      auto keysIst  = t.getProperties();
      std::vector<const char*> keysSoll = {"x", "valid", "double vector" };
      if (keysIst.size() != keysSoll.size())
        throw std::exception("Error: Sizes of key-vectors are not equal!");
      for (size_t i(0); i<keysIst.size(); ++i)
      {
        BOOST_CHECK_EQUAL(true, keysSoll.end() != std::find_if(keysSoll.begin(), keysSoll.end(), [&] (const auto& obj) { return std::strcmp(keysIst[i], obj); }));
      }      
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
      TestClass t;
      const char* name = "badKey";
      std::ostringstream os;
      t.get(name, os);      
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
      TestClass t;
      const char* name = badCall();
      std::ostringstream os;
      t.get(name, os);
    }
    catch (std::exception& e)
    {
      BOOST_CHECK( 0==strcmp(e.what(), "no such key") );
      throw e;
    }
  }

}