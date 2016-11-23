#include "private/test.pch"
#include "TestClass.h"

#include <functional>

namespace gris
{
  using namespace gstd;

  /**
  */
  TestClass::TestClass()
    : mDouble(0)
    , mBool(0)
    , mVec(0,0,0)
  {
    declareProperty<double>("double", 
      std::bind(&TestClass::setDouble, this, std::placeholders::_1), 
      std::bind(&TestClass::getDouble, this));
    
    declareProperty<bool>("bool",
      std::bind(&TestClass::setBool, this, std::placeholders::_1),
      std::bind(&TestClass::getBool, this));

    declareProperty<Vec3d>("Vec3d",
      std::bind(&TestClass::setVec3d, this, std::placeholders::_1),
      std::bind(&TestClass::getVec3d, this));
      
    //declareProperty("bool", &setBool, &getBool);
    //declareProperty("Vec3d", setVec3d, getVec3d);
  }
}