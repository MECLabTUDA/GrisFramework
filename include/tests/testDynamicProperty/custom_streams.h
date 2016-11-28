#pragma once

#include <iostream>
#include <vector>

namespace std
{
  ostream& operator<<(ostream& os, const std::vector<double>& v)
  {
    for(int i(0); i<(int)v.size()-1; ++i)
      os << v[i] << " " ;
    if( ! v.empty())
      os << v.back();
    return os;
  }

  istream& operator>>(istream& is, std::vector<double>& v)
  {
    double val (0);    
    while(is >> val || !is.eof()) 
    {
      if(is.fail()) 
      {
        throw std::exception("failed to parse string to a vector of doubles");        
      }
      v.push_back(val);
    }    
    is.clear(); // boost::lexical_cast fails if is.eof() == true, which is always the case after the loop. therefore clear the flag
    return is;
  }
}
