#include "JSONDynamicPropertyParser.h"

#include "Exception.h"

#include "Vector.h"

// rapidjson
#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
// english language parse errors
#include "rapidjson/error/en.h"

// boost strings
#include <boost/algorithm/string.hpp>

#include <fstream>

namespace gris
{
namespace gstd
{
struct PrivateJsonData
{
public:
  rapidjson::Document Document;
  rapidjson::Value*   pCurrentNode;
};

JSONDynamicPropertyParser::JSONDynamicPropertyParser()
  : IDynamicPropertyParser()
  , mFileChanged(false)
  , mPretty(false)
  , mp(std::make_unique<PrivateJsonData>())
{
}

JSONDynamicPropertyParser::JSONDynamicPropertyParser(const bool pretty)
  : JSONDynamicPropertyParser()
{
  mPretty = pretty;
}

JSONDynamicPropertyParser::~JSONDynamicPropertyParser()
{
  flush();
}

void JSONDynamicPropertyParser::flush()
{
  if (mFileChanged)
    saveFile();
}

void JSONDynamicPropertyParser::readImpl(DynamicProperty & prop)
{
  for (auto&& pair : prop)
  { 
    // to support prefixes, the propertynames could replace "." with "/" for x-path search
    auto&& trueName = pair.first;
    auto&& p        = pair.second;

    auto Iter = mp->pCurrentNode->FindMember(trueName.c_str());
    if (Iter != mp->pCurrentNode->MemberEnd())
    {
      if (p->hint() == PropertyDefinition::Float && Iter->value.IsDouble())
      {
        p->setValue(std::to_string(Iter->value.GetDouble()));
      }
      else if (p->hint() == PropertyDefinition::Integer && Iter->value.IsInt())
      {
        p->setValue(std::to_string(Iter->value.GetInt()));
      }
      else if (p->hint() == PropertyDefinition::Boolean && Iter->value.IsBool())
      {
        p->setValue(Iter->value.IsTrue() ? "true" : "false");
      }
      else if (p->hint() == PropertyDefinition::Vector3 && Iter->value.IsArray() && Iter->value.Size() == 3)
      {
        double value[3];
        size_t i(0);
        for (auto it = Iter->value.Begin(); it != Iter->value.End(); ++it, ++i)
          value[i] = it->GetDouble();
        std::ostringstream ss;
        ss << Vec3d(value);
        p->setValue(ss.str());
      }
/*      else if (p->hint() == PropertyDefinition::Matrix4x4 && Iter->value.IsArray() && Iter->value.Size() == 4)
      {
        double value[4][4] {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
        size_t i(0), j(0);
        for (auto it1 = Iter->value.Begin(); it != Iter->value.End(); ++it1, ++i, j = 0)
          for (auto it2 = it1->value.Begin(); it != it1->value.End(); ++it2, ++j)
            value[i][j] = it2->GetDouble();
        std::ostringstream ss;
        ss << Matrix4x4(value);
        p->setValue(ss.str());
      }*/
      else
      {
        if (!Iter->value.IsString())
        {
          rapidjson::StringBuffer json;
          mp->pCurrentNode->Accept(rapidjson::Writer<rapidjson::StringBuffer>(json));
          throw GSTD_EXCEPTION_FORMAT(
            "The JSON object member `%s` is not of type String, but %d (%s) in [%s:%s] `%s`.",
            trueName % Iter->value.GetType() % "0 null, 1 false, 2 true, 3 object, 4 array, 5 string, 6 number"
            % filename() % subSection() % json.GetString());
        }
        p->setValue(Iter->value.GetString());
      }
    }
    else // the child is missing in the XML, do not change the value
      appendWarning("The Value for the Property `" + trueName + "` " +
        "was missing in the XML-file `" + filename().string() + "`.");
  }
}

void JSONDynamicPropertyParser::writeImpl(const DynamicProperty & prop)
{
  const std::vector<std::string> names = prop.propertyNames();
  std::string value;

  if (mp->pCurrentNode->IsNull())
    mp->pCurrentNode->SetObject();
  else if (!mp->pCurrentNode->IsObject())
    throw GSTD_EXCEPTION_STREAM("While writing in " << filename() << ": " <<
    (subSection() == "" ? "Root" : "Existing Member `" + subSection() + "`") << " is not of type object");

  for (auto&& pair : prop)
  {
    // to support prefixes, the propertynames could replace "." with "/" for x-path search
    auto&& trueName = pair.first;
    auto&& p = pair.second;

    auto Iter = mp->pCurrentNode->FindMember(trueName.c_str());
    decltype(mp->pCurrentNode) jsonPtr;
    if (Iter == mp->pCurrentNode->MemberEnd())
      jsonPtr = &mp->pCurrentNode->AddMember(
        rapidjson::Value(trueName, mp->Document.GetAllocator()), 
        rapidjson::kNullType, 
        mp->Document.GetAllocator())[trueName];
    else jsonPtr = &Iter->value;
    if (p->hint() == PropertyDefinition::Float)
    {
      jsonPtr->SetDouble(std::stod(p->getValue()));
    }
    else if (p->hint() == PropertyDefinition::Integer)
    {
      jsonPtr->SetInt(std::stoi(p->getValue()));
    }
    else if (p->hint() == PropertyDefinition::Boolean)
    {
      jsonPtr->SetBool(std::dynamic_pointer_cast<Property<bool>>(p)->getTypedValue());
    }
    else if (p->hint() == PropertyDefinition::Vector3)
    {
      jsonPtr->SetArray();
      Vec3d vec = std::dynamic_pointer_cast<Property<Vec3d>>(p)->getTypedValue();
      for (rapidjson::SizeType i = 0; i < 3; ++i)
        if (i < jsonPtr->Size()) jsonPtr->operator[](i).SetDouble(vec[i]);
        else 
          jsonPtr->PushBack(std::remove_pointer_t<decltype(jsonPtr)>(vec[i]), 
            mp->Document.GetAllocator());
    }
    else 
      Iter->value.SetString(p->getValue(), mp->Document.GetAllocator());
    mFileChanged = true;
  }
}

void JSONDynamicPropertyParser::beforeChangeFilename(const path & newfilename)
{
  if(mFileChanged) saveFile();
  mFileChanged = false;
  readFile(newfilename);
}

void JSONDynamicPropertyParser::beforeChangeSubSection(const std::string newSubSection)
{
  mp->pCurrentNode = &mp->Document;
  std::vector<std::string> parts;
  boost::algorithm::split(parts, newSubSection, std::bind1st(std::equal_to<char>(), '.'));
  for (auto& p : parts)
  {
    auto Iter = mp->pCurrentNode->FindMember(newSubSection.c_str());
    if (Iter == mp->pCurrentNode->MemberEnd())
      throw GSTD_EXCEPTION_FORMAT(
        "The Member `%s` of the Member path `[root].%s` could not be found.",
        p % newSubSection);
    mp->pCurrentNode = &Iter->value;
  }
}

void JSONDynamicPropertyParser::saveFile()
{
  std::ofstream ofs(filename().string());
  rapidjson::OStreamWrapper osw(ofs);

  if (mPretty)
  {
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    mp->Document.Accept(writer);
  }
  else
  {
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    mp->Document.Accept(writer);
  }
  mFileChanged = false;
}

void JSONDynamicPropertyParser::readFile(const boost::filesystem::path& filename)
{
  mp->Document = rapidjson::Document();
  std::ifstream ifs(filename.string());
  rapidjson::IStreamWrapper isw(ifs);
  rapidjson::ParseResult parseResult = mp->Document.ParseStream(isw);

  if (!parseResult)
  {
    size_t startPos = std::max(size_t(0), parseResult.Offset() - 10);
    size_t endPos = parseResult.Offset() + 10;
    ifs.seekg(startPos);
    char buffer[20];
    ifs.read(buffer, endPos - startPos);
    std::string strBuffer(buffer);
    size_t errorPos = startPos - parseResult.Offset();

    // parsing was not successful, but RapidJSON did not throw an excepNoKey
    throw GSTD_EXCEPTION_FORMAT("JSON parse error: %s (Position: %u, around `%s*%s` at *)",
      rapidjson::GetParseError_En(parseResult.Code()) % parseResult.Offset() %
      strBuffer.substr(0, errorPos) % strBuffer.substr(errorPos) );
  }

  // down-cast the root node
  mp->pCurrentNode = &mp->Document;
}

}
}