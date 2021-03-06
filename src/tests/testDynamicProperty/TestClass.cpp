#include "private/test.pch"
#include "TestClass.h"
#include "custom_streams.h"

#include <functional>


namespace gris
{
  using namespace gstd;

  /**
  */
  TestClass::TestClass()
    : mDouble(0)
    , mBool(0)
    , mSize_t(0)
    , mInt(0)
    , mVec(0,0,0)
    //, mVecDouble {0.0} 
    , mpSubClass(std::make_unique<TestSubClass>())
    , mpSubClassPrefix(std::make_unique<TestSubClass>())    
  {
    declareProperty<double>("double", 
      std::bind(&TestClass::setDouble, this, std::placeholders::_1), 
      std::bind(&TestClass::getDouble, this));

    declareProperty<int>("int", 
      std::bind(&TestClass::setInt, this, std::placeholders::_1), 
      std::bind(&TestClass::getInt, this));
    
    declareProperty<size_t>("size_t",
      std::bind(&TestClass::setSize_t, this, std::placeholders::_1),
      std::bind(&TestClass::getSize_t, this));

    declareProperty<bool>("bool",
      std::bind(&TestClass::setBool, this, std::placeholders::_1),
      std::bind(&TestClass::getBool, this));

    declareProperty<Vec3d>("Vec3d",
      std::bind(&TestClass::setVec3d, this, std::placeholders::_1),
      std::bind(&TestClass::getVec3d, this));
    
    declareProperty<std::vector<double>>("VecDouble",
      std::bind(&TestClass::setVecDouble, this, std::placeholders::_1),
      std::bind(&TestClass::getVecDouble, this));

    includeProperty(*mpSubClass);
    includeProperty(*mpSubClassPrefix, "prefix");
  }

  /**
  */
  TestSubClass::TestSubClass()
    : mDouble(0.0)
  {
    declareProperty<double>("SubClassDouble", 
      std::bind(&TestSubClass::setDouble, this, std::placeholders::_1), 
      std::bind(&TestSubClass::getDouble, this));
  }

  /**
  */
  TestSubClass::~TestSubClass()
  {
  }
}