#include "private/test.pch"
#include "testDynamicPropertyManager.h"
#include "TestClass.h"

#include "gstd/DynamicPropertyManager.h"

#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem.hpp>

#include <cstdio> // delete file

namespace fs = boost::filesystem;

namespace gris
{
namespace gstd
{
  /**
  */
  void testInitializeManager()
  {    
    constexpr auto init_file = "app_init.xml";
    if (fs::is_regular_file(init_file))
    {
      std::remove(init_file);
    }
    auto& manager = GetDynamicPropertyManager();
    manager.initialize();
    manager.disconnect();
    BOOST_CHECK(fs::is_regular_file("app_init.xml"));
    
  }

  /**
  */
  void testLoginWithoutInitialization()
  {
    auto& manager = GetDynamicPropertyManager();
    TestClass prop;
    manager.login(prop);
  }

  /**
  */
  void testLogin()
  {
    try
    {
      auto& manager = GetDynamicPropertyManager();
      manager.initialize("testLogin.xml");
      TestClass prop;
      manager.login(prop);
      manager.disconnect();
    }
    catch (std::exception& e)
    {
       std::cout << "exception in " << __FUNCTION__ << ":   " << e.what() << std::endl;
    }
  }

  /**
  */
  void testLoginAfterConnect()
  {
    try
    {
      auto& manager = GetDynamicPropertyManager();
      manager.initialize("Mukno", "testLoginWithPrefix.xml");
      manager.connect("ModulePathPlanning");
      TestClass prop;
      manager.login(prop, "Test.TestClass");
      TestClass prop2;
      manager.login(prop2, "Test.TestClass2");
      manager.connect("ModuleNavigation");
      prop2;
      manager.login(prop2, "Test.TestClass2");
      manager.disconnect();
    }
    catch (std::exception& e)
    {
      std::cout << "exception in " << __FUNCTION__ << ":   " << e.what() << std::endl;
    }
  }

}
}