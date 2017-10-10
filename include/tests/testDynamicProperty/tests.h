#pragma once

#include <string>

namespace gris
{
  void showProperties();

  std::string testContainerOp(const std::string& s);

  void testDouble();
  void testInt();
  void testSize_t();
  void testBool();
  void testVec3d();
  void testVecDouble();

  void testSubClass();
  void testSubClassWithPrefix();

  void testSetBadKey();
  void testGetBadKey();
}