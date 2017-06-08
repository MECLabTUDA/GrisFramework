#include "tests.h."

#include <gstd/Vector.h>

#include <boost/test/auto_unit_test.hpp>

#include <iostream>

namespace
{
  using namespace gris;
  using std::cout;
  using std::endl;
}

namespace gris
{ 
  void testBaseVector()
  {
    {
      constexpr double x = 1.0;
      constexpr double y = 2.0;
      constexpr double z = 3.0;
      auto u = Vec3d(x, y, z);
      auto u2 = Vec3d(x, y, z);
      BOOST_CHECK_EQUAL(u.x(), x);
      BOOST_CHECK_EQUAL(u.y(), y);
      BOOST_CHECK_EQUAL(u.z(), z);
      BOOST_CHECK_EQUAL(u, u2);

      const bool notHasNan = u.hasNaN();
      BOOST_CHECK_EQUAL(notHasNan, false);
      const bool hasNan = Vec3d(std::numeric_limits<double>::quiet_NaN()).hasNaN();
      BOOST_CHECK_EQUAL(hasNan, true);

      const double val = -5.0;
      u.fill(val);
      BOOST_CHECK_EQUAL(u.x(), val);
      BOOST_CHECK_EQUAL(u.y(), val);
      BOOST_CHECK_EQUAL(u.z(), val);

      std::ostringstream oss;
      oss << u;
      std::istringstream iss(oss.str());
      Vec3d v;
      iss >> v;
      BOOST_CHECK_EQUAL(u, v);
    }
    {
      constexpr int x = 1;
      constexpr int y = 2;
      constexpr int z = 3;
      auto u = Vec3i(x,y,z);
      auto u2 = Vec3i(x,y,z);
      BOOST_CHECK_EQUAL(u.x(),x);
      BOOST_CHECK_EQUAL(u.y(),y);
      BOOST_CHECK_EQUAL(u.z(),z);
      BOOST_CHECK_EQUAL(u,u2);
      
      const int val = -5;
      u.fill(val);
      BOOST_CHECK_EQUAL(u.x(),val);
      BOOST_CHECK_EQUAL(u.y(),val);
      BOOST_CHECK_EQUAL(u.z(),val);

      std::ostringstream oss;
      oss << u;
      std::istringstream iss(oss.str());
      Vec3i v;
      iss >> v;
      BOOST_CHECK_EQUAL(u,v);
    }
  }

  void testDiscreteVector()
  {
    auto u = Vec3i(1,2,3);
    auto v = Vec3i(1,2,3);
    auto w = Vec3i(2,4,6);
    auto zero = Vec3i(0,0,0);
    BOOST_CHECK_EQUAL(u+v,w);
    BOOST_CHECK_EQUAL(w-v,u);
    
    BOOST_CHECK_EQUAL(2*v,w);
    BOOST_CHECK_EQUAL(v*2,w);
    BOOST_CHECK_EQUAL(2.0*v,w);
    BOOST_CHECK_EQUAL(v*2.0,w);
    BOOST_CHECK_EQUAL(2u*v,w);
    BOOST_CHECK_EQUAL(v*2u,w);

    const Vec3i absi (-1, -1, 2);
    const Vec3i absi_ (1, 1, 2);
    BOOST_CHECK_EQUAL(absi_, absi.abs());
    
    BOOST_CHECK_EQUAL(true, zero.isZero());
    BOOST_CHECK_EQUAL(false, u.isZero());

    const double normSquared = 14;
    const double norm        = std::sqrt(14);
    BOOST_CHECK_EQUAL(u.squaredNorm(), normSquared);
    BOOST_CHECK_CLOSE(u.norm(), norm, std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(u.dot(v), normSquared);
  }

  void testFieldVector()
  {
    auto u = Vec3d(1.0, 2.0, 3.0);
    auto v = Vec3d(0.5, 1.0, 1.5);
    auto zero = Vec3d(0, 0, 0);
    BOOST_CHECK_EQUAL(true, zero.isZero());
    BOOST_CHECK_EQUAL(false, u.isZero());
    BOOST_CHECK_EQUAL(0.5*u, v);
    BOOST_CHECK_EQUAL(u/2.0, v);
    BOOST_CHECK_EQUAL(u/2, v);

    auto N = Vec3d(10.0, 0, 0);
    auto n = Vec3d(1, 0, 0);
    BOOST_CHECK_EQUAL(n, N.normalized());

    auto x = Vec3d(1,0,0);
    auto y = Vec3d(0,1,0);
    auto z = Vec3d(0,0,1);
    BOOST_CHECK_EQUAL(x.cross(y), z);

    Vec3d arithmeticTest = x + 0.5*(y+z);
  }
}
