#include "private/gris.pch"
#include "Vector.h"

namespace gris
{
  namespace gstd
  {
    Vec3d::Vec3d()
    {
      std::fill(data, data+3, 0);
    }

    Vec3d::Vec3d(const Vec3d& o)
    {
      std::copy(o.data, o.data+3, data);
    }

    Vec3d::Vec3d(double x, double y, double z)
    {
      this->x() = x;
      this->y() = y;
      this->z() = z;
    }

    Vec3d::Vec3d(double val)
    {
      std::fill(data, data+3, val);
    }


    Vec3d Vec3d::operator=(const Vec3d& o)
    {
      if (&o==this)
        return *this;
      std::copy(o.data, o.data+3, data);
    }


    std::ostream& operator<<(std::ostream& os, const Vec3d& obj)
    {
      return obj.operator<<(os);      
    }

    std::istream& operator>>(std::istream& is, Vec3d& obj)
    {
      return is >> obj.x() >> obj.y() >> obj.z();
    }

  }
}
