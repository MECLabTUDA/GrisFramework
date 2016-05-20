#pragma once

#pragma warning (push)
#pragma warning( disable : 4251 ) // dll-interface warnings

namespace gris
{
  namespace gstd
  {
    template<>
    class Property <bool> : public IProperty
    {
      friend class DynamicPropertyManager;

      public:        
        explicit Property(bool& d) : data(d) {}

        virtual std::ostream& operator<<(std::ostream& os) override
        {
          return data ?  os << "true" : os << "false";
        }

        virtual void operator>>(std::istream& is) override
        {          
          std::string tmp;
          is >> tmp;
          if (0==tmp.compare("false"))
            data = false;
          else if (0==tmp.compare("true"))
            data = true;
          else
            throw std::exception("not able to cast istream");
        }

        private:
          bool& data;
    };

  }
}

#pragma warning (pop)