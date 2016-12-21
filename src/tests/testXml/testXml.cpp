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
    {
      auto doc = XmlDocument::read("testReadXml.xml");
      auto root = XmlNode(doc->getRoot());
      auto att = root.getChild("ApplicationSettings").getAttribute("option_a");
      std::cout << att << endl;
      root.getChild("ApplicationSettings").getAttribute("option_a").setValue("blubb");
      std::cout << att << endl;
      XmlDocument::save("testSaveXml.xml", *doc);
    }

    {
      auto doc = gris::XmlDocument::create("scene");
      gris::XmlNode root = doc->getRoot();
      gris::XmlNode node = root.addChild("Calibration");
      node.setValue("test");
      std::cout << "Get: " << node.getValue() << endl;
      gris::XmlNode obs  = root.addChild("Obstacles");
      gris::XmlDocument::save("scene.xml", *doc);
    }
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }  
  
  return 0;
}
