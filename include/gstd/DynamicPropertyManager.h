#ifndef GRIS_DYNAMIC_PROPERTY_MANAGER_H
#define GRIS_DYNAMIC_PROPERTY_MANAGER_H

#include "gstd_api.h"

#include "XmlNode.h"

#include <string>
#include <memory>

namespace gris 
{
  class XmlDocument;

  namespace gstd
  {   
    class DynamicProperty;

    /**
    */
    class GRIS_GSTD_API DynamicPropertyManager
    {
      public:
        DynamicPropertyManager();
        ~DynamicPropertyManager();

        DynamicPropertyManager(const DynamicPropertyManager&) = delete;
        DynamicPropertyManager& operator =(const DynamicPropertyManager&) = delete;

      public:
        void initialize(const std::string& prefix = "root", const std::string& filename = "app_init.xml");
        void connect   (const std::string& prefixes = "");
        void disconnect();

        void login(DynamicProperty& object, const std::string& prefixes = "");

      private:
        std::string mFileName;
        std::unique_ptr<XmlDocument> mpXml;
        XmlNode mpBaseNode;
    };

    GRIS_GSTD_API DynamicPropertyManager& GetDynamicPropertyManager();
  }
}

#endif
