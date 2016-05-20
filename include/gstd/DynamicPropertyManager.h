#ifndef GRIS_DYNAMIC_PROPERTY_MANAGER_H
#define GRIS_DYNAMIC_PROPERTY_MANAGER_H

#include "gstd_api.h"
#include "dynamicProperty.h"

#include <loki/Singleton.h>

#pragma warning ( push )
#pragma warning ( disable: 4251 ) // no dll export for std::pair


namespace gris {
  namespace gstd  // gris std
  {   
    /**
    */
    class GRIS_GSTD_API DynamicPropertyManager
    {
      public:
        DynamicPropertyManager() {}
        ~DynamicPropertyManager() {}

        void connectClass     (const std::string& classname, DynamicProperty& object);
        void registerManager  (const std::string& filename, bool writeOnDisconnect=true);

      private:
        std::pair<std::string, bool> file;      
    };

    typedef Loki::SingletonHolder<DynamicPropertyManager> TheDynamicPropertyManager;
  }
}



#pragma warning ( pop )
#endif
