#pragma once

#include "DiscreteVector.h"

#include <limits>

namespace gris
{
  template<typename T, typename S, size_t DIM>
  class Vector : public DiscreteVector<T, S, DIM>
  {
    public:
      Vector<T, S, DIM>() {}
      explicit Vector<T, S, DIM>(const T& val) : DiscreteVector<T, S, DIM>(val) {}
      explicit Vector<T, S, DIM>(const T* d)   : DiscreteVector<T, S, DIM>(d) {}
      Vector<T, S, DIM>(DiscreteVector<T, S, DIM>&& base) : DiscreteVector<T, S, DIM>(base) {} // is this a good solution?
      Vector<T, S, DIM>& operator=(DiscreteVector<T, S, DIM>&& base) { coeff = std::move(base.coeff); return *this; }    // is this a good solution?

      Vector<T, S, DIM>(T x, T y)       : DiscreteVector<T, S, DIM>(x,y) {}
      Vector<T, S, DIM>(T x, T y, T z)  : DiscreteVector<T, S, DIM>(x,y,z) {}

    private:
      static constexpr T Precision = std::numeric_limits<T>::epsilon();

    public:
      friend bool operator==(const Vector<T, S, DIM>& lhs, const Vector<T, S, DIM>& rhs) 
      {
        for (size_t i(0); i<DIM; ++i)
        {
          if (std::abs(lhs[i] - rhs[i]) > Precision)
            return false;
        }
        return true;
      }

      Vector<T, S, DIM>& operator/=(const T& val)
      {
        for(size_t i(0); i<DIM; ++i)
          coeff[i] /= val;
        return *this;
      }

      template<typename Scalar>
      friend Vector<T, S, DIM> operator/(Vector<T, S, DIM> q, Scalar val)
      {
        for(size_t i(0); i<DIM; ++i)
          q.coeff[i] /= val;
        return q;
      }

    public:
      void normalize()
      {
        const S norm = this->norm();
        for(size_t i(0); i<DIM; ++i)
          coeff[i] /= norm;
      }

      Vector<T, S, DIM> normalized() const
      {
        Vector<T, S, DIM> ret(*this);
        ret.normalize();
        return ret;
      }

      Vector<T, S, DIM> cross(const Vector<T, S, DIM>& o) const
      {
        assert(DIM==3);
        return Vector<T, S, DIM>(
          coeff[1]*o.coeff[2] - o.coeff[1]*coeff[2],
          coeff[2]*o.coeff[0] - coeff[0]*o.coeff[2], 
          coeff[0]*o.coeff[1] - coeff[1]*o.coeff[0]);
      }
  };
}
