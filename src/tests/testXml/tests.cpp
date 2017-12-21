#include "private/gris.pch"
#include "tests.h"

#include "gstd/XmlDocument.h"
#include "gstd/XmlNode.h"

#include <boost/test/unit_test.hpp>

#include <iostream>

using namespace gris;

namespace
{
  const std::string fn  = "XmlDocumentTest.xml";
  const std::string fn2 = "XmlDocumentTestResult.xml";
  const std::string root = "root";
  const std::string child1 = "child1";
  const std::string child2 = "child2";
  const std::string text1 = "text1";
  const std::string text2 = "text2";
}

namespace gris
{
namespace test
{

/** \brief Writes and reads a simple xml document and compares two child values
*/
void basicReadWrite()
{
  using namespace std;

  {
    auto doc = XmlDocument::create(root.c_str());
    auto root = doc->getRoot();
    root.addChild(child1.c_str()).setValue(text1.c_str());
    root.addChild(child2.c_str()).setValue(text2.c_str());
    XmlDocument::save(fn.c_str(), *doc);
  }
  {
    auto doc = XmlDocument::read(fn.c_str());
    auto root = doc->getRoot();
    auto v1 = root.getChild(child1.c_str()).getValue();
    auto v2 = root.getChild(child2.c_str()).getValue();
    BOOST_CHECK_EQUAL(text1, v1);
    BOOST_CHECK_EQUAL(text2, v2);
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

/** \brief Todo test setting/cahnging/reading attributes
*/
void testAttributes()
{
}

/** \brief Test if changing of values works

  adresses B#8047
*/
void testSetValue()
{
  try
  {
    // build basic document
    {
      auto doc = XmlDocument::create(root.c_str());
      auto root = doc->getRoot();
      root.addChild(child1.c_str()).setValue(text1.c_str());
      root.addChild(child2.c_str()).setValue(text2.c_str());
      XmlDocument::save(fn.c_str(), *doc);
    }
    // change basic document
    {
      auto doc = XmlDocument::read(fn.c_str());
      auto root = doc->getRoot();
      root.getChild(child1.c_str()).setValue(text2.c_str());
      root.getChild(child2.c_str()).setValue("");
      XmlDocument::save(fn2.c_str(), *doc);
    }
    // read basic document
    {
      auto doc = XmlDocument::read(fn2.c_str());
      auto root = doc->getRoot();
      auto str1 = root.getChild(child1.c_str()).getValue();
      auto str2 = root.getChild(child2.c_str()).getValue();
      BOOST_CHECK_EQUAL(str1, text2);
      BOOST_CHECK_EQUAL(str2, "");
    }
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
    throw e;
  }
}

}
}
