#include "tests.h"

#include <boost/test/auto_unit_test.hpp>
#include "gstd/InvokeLater.h"

namespace gris
{
  namespace gstd
  {
    void testValueInvokeList()
    {
      std::string testString = "";
      {
        std::function<void(const std::string&)> f = [&testString](const std::string& s) { testString += s; };
        InvokeLaterList<int> invoker;
        BOOST_CHECK(invoker.insert(0, std::bind(f, "a")));
        invoker.clear();
        BOOST_CHECK(invoker.insert(10, std::bind(f, "b")));
        BOOST_CHECK(!invoker.insert(10, std::bind(f, "x")));
        invoker.insert(16, std::bind(f, "c"));
        invoker.insert(13, std::bind(f, "d"));
        BOOST_CHECK(1 == invoker.push(std::bind(f, "e")));
        invoker.remove(10);
      }
      BOOST_CHECK(testString == "ecd");
    }
  }
}