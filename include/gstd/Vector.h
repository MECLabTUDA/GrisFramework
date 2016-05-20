#pragma once

#include "gstd_api.h"

#include <iostream>

namespace gris
{
  namespace gstd
  {
    class GRIS_GSTD_API Vec3d
    {
      public:
        Vec3d();      
        Vec3d(const Vec3d& o);
        Vec3d(double x, double y, double z);
        explicit Vec3d(double val);

        Vec3d operator=(const Vec3d& o);

        ~Vec3d() {}

        public:
        double x() const  { return data[0]; }
        double y() const  { return data[1]; }
        double z() const  { return data[2]; }

        double& x()       { return data[0]; }
        double& y()       { return data[1]; }
        double& z()       { return data[2]; }

        double  operator[](int i) const { return data[i]; }
        double& operator[](int i)       { return data[i]; }

        std::ostream& operator<<(std::ostream& os) const
        {
          return os << x() << " " << y() << " " << z();
        }
      
      private:
        double data[3];
    };


    GRIS_GSTD_API std::ostream&  operator<<(std::ostream& os, const Vec3d& obj);

    GRIS_GSTD_API std::istream&  operator>>(std::istream& is, Vec3d& obj);
  }
}
