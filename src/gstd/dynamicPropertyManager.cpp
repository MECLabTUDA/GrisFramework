#include "private/gris.pch"

#include "dynamicPropertyManager.h"

#include <iostream>

using namespace gris::gstd;


void DynamicPropertyManager::connectClass(const std::string& classname, DynamicProperty& object)
{
  std::cout << "DynamicPropertyManager:  " << "classname" << std::endl;

  for (auto prop : object.props)
  {
    prop.second->get( std::cout << "DynamicPropertyManager:  " << prop.first << " ") << std::endl;
  }
}


void DynamicPropertyManager::registerManager(const std::string& filename, bool writeOnDisconnect)
{
  std::cout << "DynamicPropertyManager:  " << "filename" << std::endl;
}