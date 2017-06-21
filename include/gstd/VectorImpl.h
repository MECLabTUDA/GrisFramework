#pragma once

#include "BaseVector.h"

#include <limits>

namespace gris
{
  template<typename T, typename S, size_t DIM>
  class Vector : public BaseVector<T,DIM>
  {
    public:
      Vector<T, S, DIM>() {}
      explicit Vector<T, S, DIM>(const T& val) : BaseVector<T,DIM>(val) {}
      explicit Vector<T, S, DIM>(const T* d)   : BaseVector<T,DIM>(d) {}

      Vector<T, S, DIM>(T x, T y)       : BaseVector<T,DIM>(x,y) {}
      Vector<T, S, DIM>(T x, T y, T z)  : BaseVector<T,DIM>(x,y,z) {}

    // ------ functions for natural/integer scalars ------ //    
    public:
      Vector<T, S, DIM>& operator+=(const Vector<T, S, DIM>& rhs)
      {
        for(size_t i(0); i<DIM; ++i)
          coeff[i] += rhs[i];
        return *this;
      }

      Vector<T, S, DIM>& operator-=(const Vector<T, S, DIM>& rhs)
      {
        for(size_t i(0); i<DIM; ++i)
          coeff[i] -= rhs[i];
        return *this;
      }

      friend Vector<T, S, DIM> operator+(Vector<T, S, DIM> lhs, const Vector<T, S, DIM>& rhs)
      {
        lhs += rhs;
        return lhs;
      }

      friend Vector<T, S, DIM> operator-(Vector<T, S, DIM> lhs, const Vector<T, S, DIM>& rhs)
      {
        lhs -= rhs;
        return lhs;
      }

      template<typename Scalar>
      Vector<T, S, DIM>& operator*=(const Scalar& val)
      {
        for(size_t i(0); i<DIM; ++i)
          coeff[i] = static_cast<T>(coeff[i]*val);
        return *this;
      }

      template<typename Scalar>
      friend Vector<T, S, DIM> operator*(Vector<T, S, DIM> q, Scalar val)
      {  
        for(size_t i(0); i<DIM; ++i)
          q.coeff[i] = static_cast<T>(q.coeff[i]*val);
        return q;
      }

      template<typename Scalar>
      friend Vector<T, S, DIM> operator*(Scalar val, Vector<T, S, DIM> q)
      {
        for(size_t i(0); i<DIM; ++i)
          q.coeff[i] = static_cast<T>(q.coeff[i]*val);
        return q;
      }

      template<typename = std::enable_if_t<std::is_signed<T>::value>>
      Vector<T, S, DIM> abs() const
      {
        Vector<T, S, DIM> ret;
        for(size_t i(0); i<DIM; ++i)
          ret[i] = static_cast<T>(std::abs(coeff[i]));
        return ret;
      }
      
    public:
      bool isZero() const
      {
        return (*this == Vector<T, S, DIM>(static_cast<T>(0)));
      }

      S squaredNorm() const
      {
        S d(0);
        for(size_t i(0); i<DIM; ++i)
          d += coeff[i]*coeff[i];
        return d;
      }

      S norm() const
      {
        return std::sqrt(squaredNorm());
      }
                  
      S dot(const Vector<T, S, DIM>& o) const
      {
        S d(0);
        for(size_t i(0); i<DIM; ++i)
          d += coeff[i]*o.coeff[i];
        return d;
      }

      // ------ functions for field scalars ------ //    
    private:
      static constexpr T Precision = std::numeric_limits<T>::epsilon();

    public:
      template<typename = std::enable_if_t<std::is_floating_point<T>::value>>
      friend bool operator==(const Vector<T, S, DIM>& lhs, const Vector<T, S, DIM>& rhs) 
      {
        for (size_t i(0); i<DIM; ++i)
        {
          if (std::abs(lhs[i] - rhs[i]) > Precision)
            return false;
        }
        return true;
      }

      template<typename = std::enable_if_t<std::is_floating_point<T>::value>>
      Vector<T, S, DIM>& operator/=(const T& val)
      {
        for(size_t i(0); i<DIM; ++i)
          coeff[i] /= val;
        return *this;
      }

      //template<typename Scalar, typename = std::enable_if_t<std::is_floating_point<T>::value>>
      template<typename Scalar>
      friend Vector<T, S, DIM> operator/(Vector<T, S, DIM> q, Scalar val)
      {
        for(size_t i(0); i<DIM; ++i)
          q.coeff[i] /= val;
        return q;
      }

    public:
      template<typename = std::enable_if_t<std::is_floating_point<T>::value>>
      void normalize()
      {
        const S norm = this->norm();
        for(size_t i(0); i<DIM; ++i)
          coeff[i] /= norm;
      }

      template<typename = std::enable_if_t<std::is_floating_point<T>::value>>
      Vector<T, S, DIM> normalized() const
      {
        Vector<T, S, DIM> ret(*this);
        ret.normalize();
        return ret;
      }

      template<typename = std::enable_if_t<std::is_floating_point<T>::value>>
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
