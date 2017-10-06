#pragma once

#include <iostream>
#include <array>
#include <cassert>

namespace gris
{
  template<typename T, size_t DIM>
  class BaseVector
  {
    public:
      BaseVector<T, DIM>()             { coeff.fill(0);  }
      explicit BaseVector<T, DIM>(const T& val) { coeff.fill(val);  }
      explicit BaseVector<T, DIM>(const T* d)   { for(size_t i(0); i<DIM; ++i) coeff[i] = d[i]; }

      template<typename = std::enable_if_t<DIM == 2>>
      BaseVector<T, DIM>(T x, T y)        { coeff[0] = x; coeff[1] = y; }
      template<typename = std::enable_if_t<DIM == 3>>
      BaseVector<T, DIM>(T x, T y, T z)   { coeff[0] = x; coeff[1] = y; coeff[2] = z; }
      
    public:
      T&        operator[](std::size_t idx)       { return coeff[idx]; }
      const T&  operator[](std::size_t idx) const { return coeff[idx]; }

      template<typename = std::enable_if_t<(DIM > 0)>>
      T&        x()       { return coeff[0]; }
      template<typename = std::enable_if_t<(DIM > 0)>>
      const T&  x() const { return coeff[0]; }
      template<typename = std::enable_if_t<(DIM > 1)>>
      T&        y()       { return coeff[1]; }
      template<typename = std::enable_if_t<(DIM > 1)>>
      const T&  y() const { return coeff[1]; }
      template<typename = std::enable_if_t<(DIM > 2)>>
      T&        z()       { return coeff[2]; }
      template<typename = std::enable_if_t<(DIM > 2)>>
      const T&  z() const { return coeff[2]; }
      T*        data()        { return coeff.data(); }
      const T*  data() const  { return coeff.data(); } 
      
    public:
      friend bool operator==(const BaseVector<T, DIM>& lhs, const BaseVector<T, DIM>& rhs) 
      {
        for (size_t i(0); i<DIM; ++i)
        {
          if ( ! (lhs[i] == rhs[i]))
            return false;
        }
        return true;
      }
      friend bool operator!=(const BaseVector<T, DIM>& lhs, const BaseVector<T, DIM>& rhs)
      {
        return ! (lhs == rhs); 
      }

    public:
      template<typename = std::enable_if<std::is_floating_point<T>::value>>
      bool hasNaN() const
      {
        for (size_t i(0); i<DIM; ++i)
        {
          if ( std::isnan(coeff[i]) )
            return true;
        }
        return false;
      }

      void fill(const T& val)
      {
        coeff.fill(val);
      }

      std::ostream& operator<<(std::ostream& os) const
      {
        for(size_t i(1); i<DIM; ++i)
          os << coeff[i-1] << " ";
        os << coeff.back();
        return os;
      }

      std::istream& operator>>(std::istream& is)
      {
        is >> coeff[0];
        for (size_t i(1); i<DIM; ++i)
        {
          // this is for boost::lexical_cast which does not ignore white spaces
          // see https://stackoverflow.com/questions/10382884/c-using-classes-with-boostlexical-cast
          if ((is.flags() & std::ios_base::skipws) == 0) 
          {
            char whitespace;
            is >> whitespace;
          }
          is >> coeff[i];
        }
        return is;
      }

      friend std::ostream& operator<<(std::ostream& os, const BaseVector<T, DIM>& v) { return v.operator<<(os); } 
      friend std::istream& operator>>(std::istream& is,       BaseVector<T, DIM>& v) { return v.operator>>(is); }

    protected:
      std::array<T, DIM> coeff;
  };

}
