#pragma once

#include "gstd_api.h"

#include "IDynamicPropertyParser.h"

namespace rapidjson 
{
  template <typename, typename, typename> class GenericDocument;
  template <typename, typename> class GenericValue;
  template <typename> struct UTF8;
}


namespace gris
{
namespace gstd
{
  struct PrivateJsonData;

  class GRIS_GSTD_API JSONDynamicPropertyParser : public IDynamicPropertyParser
  {
  public:

    JSONDynamicPropertyParser();
    JSONDynamicPropertyParser(const bool pretty);
    ~JSONDynamicPropertyParser();

    virtual void flush();

  protected:
    // Internal implementation of the read procedure
    virtual void readImpl(DynamicProperty& prop);
    // Internal implementation of the write procedure
    virtual void writeImpl(const DynamicProperty& prop);

    
    virtual void beforeChangeFilename(const path& newfilename);
    virtual void beforeChangeSubSection(const std::string newSubSection);

  private:
    void saveFile();
    void readFile(const boost::filesystem::path& filename);

  public:
    bool writePretty() const { return mPretty; }
    void setWritePretty(const bool p) { mPretty = p; }

  private:
    std::unique_ptr<PrivateJsonData> mp;

    bool                         mFileChanged;
    bool                         mPretty;
  };
}
}