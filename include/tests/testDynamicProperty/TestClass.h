#pragma once

#include "gstd/Vector.h"

#include <gstd/DynamicProperty.h>

namespace gris
{
  class TestClass : public gris::gstd::DynamicProperty
  {
  public:
    TestClass();

  public:
      double getDouble() const { return mDouble; }
      void   setDouble(double x) { mDouble = x; }

      bool getBool() const { return mBool; }
      void setBool(bool b) { mBool = b; }

      const gstd::Vec3d&  getVec3d() const { return mVec; }
      void                setVec3d(const gstd::Vec3d& vec) { mVec = vec; }

      const std::vector<double>&  getVecDouble() const { return mVecDouble; }
      void                setVecDouble(const std::vector<double>& vec) { mVecDouble = vec; }

    private:
      double mDouble;
      bool   mBool;
      gstd::Vec3d  mVec;
      std::vector<double> mVecDouble;
  };
}
