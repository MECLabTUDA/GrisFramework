#include <private/testXml.pch>

#include <gstd/XmlDocument.h>
#include <gstd/XmlNode.h>

#include <iostream>

using namespace gris;


int main(int argc, char ** argv)
{
  using namespace std;
  try
  {
    const std::string fn = "XmlDocumentText.xml";
    const std::string root = "root";
    const std::string child1 = "child1";
    const std::string text1 = "text1";
    const std::string text2 = "text2";

    {
      auto doc = XmlDocument::create(root.c_str());
      auto root = doc->getRoot();
      root.addChild(child1.c_str()).setValue(text1.c_str());
      XmlDocument::save(fn.c_str(), *doc);
    }
    {
      auto doc = XmlDocument::read(fn.c_str());
      auto root = doc->getRoot();
      auto child = root.getChild(child1.c_str());
      std::cout << child.getValue() << std::endl;
      child.setValue(text2.c_str());
      XmlDocument::save(fn.c_str(), *doc);
    }
    {
      auto doc = XmlDocument::read(fn.c_str());
      auto root = doc->getRoot();
      auto child = root.getChild(child1.c_str());
      std::cout << child.getValue() << std::endl;
    }
    {
      /*auto doc = XmlDocument::read("testReadXml.xml");
      auto root = XmlNode(doc->getRoot());
      auto att = root.getChild("ApplicationSettings").getAttribute("option_a");
      std::cout << att << endl;
      root.getChild("ApplicationSettings").getAttribute("option_a").setValue("blubb");
      std::cout << att << endl;
      XmlDocument::save("testSaveXml.xml", *doc);*/
    }
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }  
  
  return 0;
}
