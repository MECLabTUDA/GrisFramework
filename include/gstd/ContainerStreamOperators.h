#pragma once

#include <ios>
#include <type_traits>
#include <sstream>
#include <vector>
#include <exception>

namespace std {
  template<typename CharT, typename T, typename = std::enable_if_t<!std::is_reference<T>::value && !std::is_pointer<T>::value, const std::vector<T>&>>
  std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const std::vector<T>& container)
  {
    bool comma = false;
    os << "[";
    for (auto it = container.cbegin(); it != container.cend(); ++it)
    {
      if (comma) os << ","; else comma = true;
      // read the current container value into a stringstream
      std::basic_stringstream<CharT> ss;
      ss << *it;
      // iterate over the stringstream
      for (std::istreambuf_iterator<CharT> ssit(ss); ssit != std::istreambuf_iterator<CharT>(); ++ssit)
      {
        if (*ssit == '\\' || *ssit == ']' || *ssit == '[' || *ssit == ',')
          os << '\\';
        os << *ssit;
      }
    }
    os << "]";
    return os;
  }
  //will not clear, but override
  template<typename CharT, typename T, typename = std::enable_if_t<
    !is_reference<T>::value && !is_pointer<T>::value && !is_const<T>::value &&
    (is_default_constructible<T>::value || is_constructible<T, const basic_string<CharT>&>::value)>>
  std::basic_istream<CharT>& operator >> (std::basic_istream<CharT>& is, std::vector<T>& container)
  {
    std::istreambuf_iterator<CharT> ssit(is);
    if (ssit == std::istreambuf_iterator<CharT>() || *ssit != '[')
      throw std::exception("invalid stream or stream format");
    ++ssit;
    std::basic_stringstream<CharT> ssbuf;
    bool endflag = false;
    auto current = container.begin();
    for (; ssit != std::istreambuf_iterator<CharT>(); ++ssit)
    {
      switch (*ssit)
      {
      case ']':
        endflag = true;
      case ',':
        if (current != container.end())
          ssbuf >> *(current++);
        else
        {
          T t;
          ssbuf >> t;
          container.push_back(t);
          current = container.end();
        }
        // reset the stringstream
        ssbuf = std::basic_stringstream<CharT>();
        break;
      case '\\':
        if (++ssit == std::istreambuf_iterator<CharT>())
          throw std::exception("stream ended after escape character \\");
        if (*ssit != '\\' && *ssit != ']' && *ssit != '[' && *ssit != ',')
          throw std::exception((std::string("invalid escape sequence `") + *ssit + "` in input stream!").c_str());
      default:
        ssbuf << *ssit;
        break;
      }
      if (endflag)
      {
        ++ssit;
        break;
      }
    }
    return is;
  }
}