#include "private/test.pch"
#include "TestClass.h"

#include <functional>
#include <iostream>

namespace std
{
  ostream& operator<<(ostream& os, const std::vector<double>& v)
  {
    std::for_each(v.begin(), v.end() -1 , [&] (double d) { os << v << " " ; });
    if( ! v.empty())
      os << v.back();
    return os;
  }

  istream& operator>>(istream& is, std::vector<double>& v)
  {
    double val (0);
    while(is >> val || !is.eof()) 
    {
      if(is.fail()) 
      {
        throw std::exception("failed to parse string to a vector of doubles");        
      }
      v.push_back(val);
    }    
    is.clear(); // boost::lexical_cast fails if is.eof() == true, which is always the case after the loop. therefore clear the flag
    return is;
  }
}

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
  }
}