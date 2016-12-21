#include "private/gris.pch"
#include "dynamicPropertyTemplates.h"

namespace gris
{
  namespace gstd
  {
    template<T>
    class Property <bool> : public IProperty
    {
      friend class DynamicPropertyManager;

      public:        
      explicit Property(T& d) : data(d) {}               

      virtual std::ostream& operator<<(std::ostream& os) override
      {
        return os << data;
      }

      virtual void operator>>(std::istream& is) override
      {
        is >> data;
      }

      private:
      T& data;
    };
  }
}