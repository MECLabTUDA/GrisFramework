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
      double getX() const   { return x; }
      void   setX(double x) { this->x = x; }

    private:
      double x;
      bool   valid;
      gstd::Vec3d  vd;
  };
}
