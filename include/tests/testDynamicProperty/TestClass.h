#pragma once

#include "gstd/Vector.h"

#include <gstd/DynamicProperty.h>

namespace std
{
  ostream& operator<<(ostream& os, const std::vector<double>& v);
  istream& operator>>(istream& is, std::vector<double>& v);
};

namespace gris
{
  class TestClass : public gris::gstd::DynamicProperty
  {
  public:
    TestClass();

  public:
      double getDouble() const    { return mDouble; }
      void   setDouble(double x)  { mDouble = x; }
      int    getInt() const       { return mInt; }
      void   setInt(int x)        { mInt = x; }
      size_t getSize_t() const    { return mSize_t; }
      void   setSize_t(size_t x)  { mSize_t = x; }

      bool getBool() const { return mBool; }
      void setBool(bool b) { mBool = b; }

      const gstd::Vec3d&  getVec3d() const { return mVec; }
      void                setVec3d(const gstd::Vec3d& vec) { mVec = vec; }

      const std::vector<double>&  getVecDouble()                                const { return mVecDouble; }
      void                        setVecDouble(const std::vector<double>& vec)        { mVecDouble = vec; }

    private:
      double mDouble;
      bool   mBool;
      int    mInt;
      size_t mSize_t;
      gstd::Vec3d  mVec;
      std::vector<double> mVecDouble;
  };
}
