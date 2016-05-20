#include "private/test.pch"
#include "TestClass.h"

namespace gris
{

  TestClass::TestClass()
    : x(0)
    , valid(0)
    , vd(0,0,0)
  {
    append("x", x);
    append("valid", valid);
    append("double vector", vd);
  }
}