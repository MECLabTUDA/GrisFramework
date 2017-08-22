#include "XmlDynamicPropertyParser.h"

#include "Exception.h"

#include <boost/algorithm/string/split.hpp>

#include <regex>

namespace gris
{
namespace gstd
{
XmlDynamicPropertyParser::~XmlDynamicPropertyParser()
{
  flush();
}

void XmlDynamicPropertyParser::flush()
{
  if (mFileChanged) XmlDocument::save(filename().string().c_str(), *mpDocumentNode);
}

void XmlDynamicPropertyParser::readImpl(DynamicProperty & prop)
{
  const std::vector<std::string> names = prop.propertyNames();
  for (auto& trueName : names)
  {
    // to support prefixes, the propertynames could replace "." with "/" for x-path search
    std::string xmlCompatibleName = XmlCompatible(trueName);

    if (mCurrentNode.hasChild(xmlCompatibleName.c_str()))
      prop.setProperty(trueName, mCurrentNode.getChild(xmlCompatibleName.c_str()).getValue());
    else // the child is missing in the XML, do not change the value
      appendWarning("The Value for the Property '" + trueName + "' (xml-tag: " +
      xmlCompatibleName + ") was missing in the XML-file.");
  }
}

void XmlDynamicPropertyParser::writeImpl(const DynamicProperty & prop)
{
  const std::vector<std::string> names = prop.propertyNames();
  std::vector<std::string> xmlNames;
  xmlNames.reserve(names.size());
  std::string value;
  for (auto& trueName : names)
  {
    // to support prefixes, the propertynames could replace "." with "/" for x-path search
    std::string xmlCompatibleName = XmlCompatible(trueName);
    auto it = std::find(xmlNames.begin(), xmlNames.end(), xmlCompatibleName);
    if (it != xmlNames.end())
      throw GSTD_EXCEPTION_FORMAT(
        "Using multiple instances of `%s` in XML, where the conversion to xml-compatibility is identical. The relevant properties are: `%s` and `%s`",
        xmlCompatibleName % trueName % names[it - xmlNames.begin()]
      );
    xmlNames.push_back(xmlCompatibleName);
    prop.getProperty(trueName, value);
    if (mCurrentNode.hasChild(xmlCompatibleName.c_str()))
      mCurrentNode.getChild(xmlCompatibleName.c_str()).setValue(value.c_str());
    else
      mCurrentNode.addChild(xmlCompatibleName.c_str()).setValue(value.c_str());
    mFileChanged = true;
  }
}

void XmlDynamicPropertyParser::beforeChangeFilename(const path & newfilename)
{
  if(mFileChanged) XmlDocument::save(filename().string().c_str(), *mpDocumentNode);
  mFileChanged = false;
  mpDocumentNode = XmlDocument::read(newfilename.string().c_str());
}

void XmlDynamicPropertyParser::beforeChangeSubSection(const std::string newSubSection)
{
  std::vector<std::string> parts;
  boost::algorithm::split(parts, newSubSection, std::bind1st(std::equal_to<char>(), '.'));
  std::string xpath;
  for (auto& p : parts)
  {
    if (!xpath.empty()) xpath += "/";
    xpath += XmlCompatible(p);
  }

  mCurrentNode = mpDocumentNode->getRoot().XPathNode(xpath.c_str());
}

std::string XmlDynamicPropertyParser::XmlCompatible(const std::string & name)
{
  std::regex whitespace_replacer("\\s");
  std::regex replacer("[^a-zA-Z_]");
  auto result =
    std::regex_replace(
      std::regex_replace(name, whitespace_replacer, "")
      , replacer, "_");
  if (!std::regex_search(result, std::regex("[^a-zA-Z_]")))
    throw GSTD_EXCEPTION("Conversion to XML-campatible failed. "
      "The name '" + name + "' cannot be converted into a XML-compatible tagname. It is therefore not valid.");
  return result;
}
}
}