#include "private/gris.pch"

#include "dynamicPropertyManager.h"

#include <iostream>
#include <sstream>

using namespace gris::gstd;


void DynamicPropertyManager::connectClass(const std::string& classname, DynamicProperty& object)
{
  std::cout << "DynamicPropertyManager:  " << "classname" << std::endl;

  for (auto& prop : object.mProperties)
  {
    std::cout << "DynamicPropertyManager:  " << prop.first << ": " << prop.second.get() << std::endl;
  }
}


void DynamicPropertyManager::registerManager(const std::string& filename, bool writeOnDisconnect)
{
  std::cout << "DynamicPropertyManager:  " << "filename" << std::endl;
}