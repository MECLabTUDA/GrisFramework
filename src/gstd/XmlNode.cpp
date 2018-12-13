#include "XmlNode.h"
#include "Exception.h"

#include <pugixml.hpp>

#include <iostream>
#include <sstream>

using namespace std;

namespace gris
{
  const char* XmlNode::ErrorStrings[N_Items] = 
  {
    "Child not found",
    "Attribute not found",
    "XPath not found",
    "XPath invalid",
  };

  struct XmlNode::Impl
  {
    Impl() {}
    ~Impl() {}

    pugi::xml_node node;
    std::string    name;
  };

  XmlNode::XmlNode()
    : mp(std::make_unique<Impl>())
  {
  }

  XmlNode::XmlNode(const XmlNode& o)
    : mp(std::make_unique<Impl>())
  {
    mp->node = o.mp->node;
    mp->name = o.mp->name;
  }

  XmlNode::~XmlNode()
  {
  }

  XmlNode::XmlNode(pugi::xml_node* node)
    : mp(std::make_unique<Impl>())
  {
    if (nullptr!=node)
    {
      mp->node = *node;
      mp->name = node->name();
    }
  }

  XmlNode& XmlNode::operator=(const XmlNode& o)
  {
    if (this!=&o)
    {
      mp->node = o.mp->node;
      mp->name = o.mp->name;
    }
    return *this;
  }

  /**
  */
  XmlNode::XmlNode(XmlNode&& o) = default;

  /**
  */
  XmlNode& XmlNode::operator=(XmlNode&& o) = default;

  /**
  */
  void XmlNode::setRoot(pugi::xml_node& root)
  {
    mp->node = root;
    mp->name = root.name();
  }

  /**
  */
  pugi::xml_node* XmlNode::getNode()
  {
    return &(mp->node);
  }

  /**
  */
  std::vector<XmlNode> XmlNode::getChildren() const
  {
    std::vector<XmlNode> result;
    auto children = mp->node.children();
    for (auto iter = children.begin(); iter!=children.end(); ++iter)
    {
      auto node = XmlNode(&(*iter));
      result.push_back(node);
    }
    return result;
  }

  /**
  */
  XmlNode XmlNode::getChild(const std::string& name) const
  {
    XmlNode node;
    node.mp->node = mp->node.child(name.c_str());
    if (node.mp->node.empty())
    {
      std::stringstream ss;
      ss << ErrorStrings[enChildNotFound] << ": " << name;
      throw GSTD_EXCEPTION(ss.str());
    }
    return node;
  }

  /**
  */  
  bool XmlNode::hasChild(const std::string& name) const
  {
    return !(mp->node.child(name.c_str()).empty());
  }

  /**
  */
  XmlAttribute XmlNode::getAttribute(const std::string& name) const
  {
    XmlAttribute res(&mp->node.attribute(name.c_str()));
    if (res.mAtt->empty())
    {
      std::stringstream ss;
      ss << ErrorStrings[enAttributeNotFound] << ": " << name;
      throw GSTD_EXCEPTION(ss.str());
    }
    return res;
  }

  /**
  */
  void XmlNode::addChild(const XmlNode& node)
  {
    mp->node.append_copy(node.mp->node);
  }

  /**
  */
  XmlNode XmlNode::addChild(const std::string& name)
  {
    XmlNode res;
    res.mp->node = mp->node.append_child(name.c_str());
    return res;
  }

  /**
  */
  void XmlNode::setValue(const std::string& val)
  {
    mp->node.text().set(val.c_str());
  }

  /**
  */
  void XmlNode::setName(const std::string& val)
  {
    if (mp->node.set_name(val.c_str()))
    {
      mp->name = val;
    }
  }
  
  /**
  */
  const char* XmlNode::getName() const
  {
    return mp->name.c_str();
  }

  /**
  */
  const char* XmlNode::getValue() const
  {    
    return mp->node.child_value();
  }
  
  /**
  */
  void XmlNode::addAttribute(const std::string& name, const std::string& value)
  {    
    pugi::xml_attribute att;
    att.set_name(name.c_str());
    att.set_value(value.c_str());
    mp->node.append_copy(att);
    //mp->node.append_attribute(att);
  }

  /**
  */
  XmlAttribute XmlNode::addAttribute(const std::string& name)
  {    
    pugi::xml_attribute att = mp->node.append_attribute(name.c_str());
    return XmlAttribute (&att);    
  }

  std::vector<XmlNode> XmlNode::XPathNodes(const std::string& query) const
  {
    std::vector<XmlNode> result;
    pugi::xpath_node_set nodes;
    try 
    {
       nodes = mp->node.select_nodes(query.c_str());
    }
    catch (const pugi::xpath_exception& e)
    {
      std::stringstream ss;
      ss << ErrorStrings[enXPathInvalid] << ": " << e.what();
      throw GSTD_EXCEPTION(ss.str());
    }
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
      XmlNode node;
      node.mp->node = it->node();
      result.push_back(node);
    }
    return std::vector<XmlNode>();
  }

  XmlNode XmlNode::XPathNode(const std::string& query) const
  {
    XmlNode node;
    try 
    {
      node.mp->node = mp->node.select_node(query.c_str()).node();
    } 
    catch (const pugi::xpath_exception& e)
    {
      std::stringstream ss;
      ss << ErrorStrings[enXPathInvalid] << ": " << e.what();
      throw GSTD_EXCEPTION(ss.str());
    }
    if (node.mp->node.empty())
    {
      std::stringstream ss;
      ss << ErrorStrings[enXPathNotFound] << ": " << query;
      throw GSTD_EXCEPTION(ss.str());
    }
    return node;
  }

  /**
  */
  void XmlNode::removeChild(const std::string& name)
  {
    if ( ! hasChild(name))
      return;
    mp->node.remove_child(name.c_str());
  }

  /**
  */
  void XmlNode::removeChildren()
  {
    const auto nodes = mp->node.children();
    std::vector<std::string> names;
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
      names.push_back(std::string(it->name()));
    }
    for (const auto& name : names) 
    {
      mp->node.remove_child(name.c_str());
    }
  }

  /**
  */
  ostream& XmlNode::print(ostream& os) const
  {
    os << "Node " << mp->name;
    return os;
  }

  /**
  */
  std::ostream& operator<<(std::ostream& os, const XmlNode& node)
  {
    return node.print(os);
  }
}
