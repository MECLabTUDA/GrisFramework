#ifndef GRIS_DYNAMIC_PROPERTY_H
#define GRIS_DYNAMIC_PROPERTY_H

#include "private/std_api.h"

#include <string>
//#include <>


namespace gris {
	namespace gstd  // gris std
	{

    GRIS_GSTD_API class DynamicProperty
    {
      public:
        void connectClass    (const std::string& filename, bool writeOnDisconnect);
        void registerClass   (const std::string& tag);
        void append          ( const std::string& tag, const std::string& value);

      private:
        //std::vector<std::pair<std::string,std::string>> variables;        
    };
    
	}
}


#endif
