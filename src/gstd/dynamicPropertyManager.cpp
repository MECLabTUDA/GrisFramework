#include "private/gris.pch"

#include "DynamicPropertyManager.h"
#include "DynamicProperty.h"

#include "XmlDocument.h"
#include "XmlNode.h"

#include <loki/Singleton.h>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <sstream>

namespace fs = boost::filesystem;

namespace
{
  using namespace gris;

  void proceedNode(XmlNode& node, const std::vector<std::string>& tokens);
  void proceedProperty(XmlNode& node, gstd::DynamicProperty& prop);
}

namespace gris
{
namespace gstd
{
  /**
  */
  using TheDynamicPropertyManager = Loki::SingletonHolder<DynamicPropertyManager>;

  /**
  */
  DynamicPropertyManager& GetDynamicPropertyManager()
  {
    return TheDynamicPropertyManager::Instance();
  }

  /**
  */
  DynamicPropertyManager::DynamicPropertyManager()
  {    
  }

  /**
  */
  DynamicPropertyManager::~DynamicPropertyManager()
  {
    try
    {
      disconnect();
    }
    catch (std::exception& e)
    {
      std::cout << "==== WARNING =====\n   DTOR.DynamicPropertyManager: " << e.what() << "\n==== WARNING =====\n";
    }
  }

  /**
  */
  void DynamicPropertyManager::disconnect()
  {
    if (nullptr != mpXml.get())
    {
      XmlDocument::save(mFileName.c_str(), *mpXml);
      mpBaseNode = XmlNode();
      mpXml.reset();
    }
  }
  
  /**
  */
  void DynamicPropertyManager::initialize(const std::string& prefix, const std::string& filename)
  {
    fs::path fn(filename);
    std::unique_ptr<XmlDocument> pTmp;
    if (fs::is_regular_file(fn))
    {
      pTmp = XmlDocument::read(filename.c_str());            
    }
    else
    {
      pTmp = prefix.empty() ? XmlDocument::create("root")   :   XmlDocument::create(prefix.c_str());
    }
    mpBaseNode = pTmp->getRoot();
    mpXml.swap(pTmp);
    mFileName = filename;
  }

  /**
  */
  void DynamicPropertyManager::connect(const std::string& prefixes)
  { 
    if (!mpXml.get())
      throw std::exception("DynamicPropertyManager was not initialized");

    std::vector<std::string> tokens;
    boost::split(tokens, prefixes, boost::is_any_of("."));
    if(tokens.empty())
      throw std::exception("no prefixes available");  // enforce proper use of Manager
    
    mpBaseNode = mpXml->getRoot();
    proceedNode(mpBaseNode, tokens);
  }
  
  /**
  */
  void DynamicPropertyManager::login(DynamicProperty& object, const std::string& prefixes)
  {
    if (!mpXml.get())
      throw std::exception("DynamicPropertyManager was not initialized");

    XmlNode node = mpBaseNode;    
    if ( ! prefixes.empty())
    {
      std::vector<std::string> tokens;
      boost::split(tokens, prefixes, boost::is_any_of("."));
      proceedNode(node, tokens);
    }
    proceedProperty(node, object);
  }

}
}

namespace
{
  /**
  */
  void proceedNode(XmlNode& node, const std::vector<std::string>& tokens)
  {
    for (int i(0); i<(int)tokens.size(); ++i)
    {
      if (node.hasChild(tokens[i].c_str()))
      {
        node = node.getChild(tokens[i].c_str());
      }
      else
      {
        node = node.addChild(tokens[i].c_str());
      }
    }
  }

  /**
  */
  void proceedProperty(XmlNode& node, gstd::DynamicProperty& prop)
  {
    std::vector<std::string> names;
    prop.getPropertyNames(names);
    for (const auto& name : names)
    {
      if (node.hasChild(name.c_str()))
      {
        std::string val = node.getChild(name.c_str()).getValue();
        prop.setProperty(name, val);
      }
      else
      {
        std::string val;
        prop.getProperty(name, val);
        node.addChild(name.c_str()).setValue(val.c_str());
      }
    }
  }
}