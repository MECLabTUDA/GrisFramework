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
  }
}