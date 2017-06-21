#include "XmlNode.h"

#include <pugixml.hpp>

#include <iostream>


using namespace std;

namespace gris
{

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
  XmlNode XmlNode::getChild(const char* name) const
  {
    XmlNode node;
    node.mp->node = mp->node.child(name);
    if (node.mp->node.empty())
    {
      throw XmlException(XmlException::ChildNotFound, name);
    }
    return node;
  }

  /**
  */  
  bool XmlNode::hasChild(const char* name) const
  {
    return !(mp->node.child(name).empty());
  }

  /**
  */
  XmlAttribute XmlNode::getAttribute(const char* name) const
  {
    XmlAttribute res(&mp->node.attribute(name));
    if (res.mAtt->empty())
    {
      throw XmlException(XmlException::AttributeNotFound, name);
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
  XmlNode XmlNode::addChild(const char* name)
  {
    XmlNode res;
    res.mp->node = mp->node.append_child(name);
    return res;
  }

  /**
  */
  void XmlNode::setValue(const char* val)
  {
    mp->node.set_value(val);
    mp->node.append_child(pugi::node_pcdata).set_value(val);
  }

  /**
  */
  void XmlNode::setName(const char* val)
  {
    if (mp->node.set_name(val))
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
  void XmlNode::addAttribute(const char* name, const char* value)
  {    
    pugi::xml_attribute att;
    att.set_name(name);
    att.set_value(value);
    mp->node.append_copy(att);
    //mp->node.append_attribute(att);
  }

  /**
  */
  XmlAttribute XmlNode::addAttribute(const char* name)
  {    
    pugi::xml_attribute att = mp->node.append_attribute(name);
    return XmlAttribute (&att);    
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

  /**
  */
  char const* XmlException::what() const 
  {
    return std::string(mStr).append(": ").append(mInfo).c_str();
  }

}
