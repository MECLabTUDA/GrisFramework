#pragma once

#include "gstd_api.h"

#include "IDynamicPropertyParser.h"
#include "XmlDocument.h"

namespace gris
{
namespace gstd
{

  class GRIS_GSTD_API XmlDynamicPropertyParser : public IDynamicPropertyParser
  {
  public:
    XmlDynamicPropertyParser() : IDynamicPropertyParser() {}
    ~XmlDynamicPropertyParser();

    virtual void flush();

  protected:
    // Internal implementation of the read procedure
    virtual void readImpl(DynamicProperty& prop);
    // Internal implementation of the write procedure
    virtual void writeImpl(const DynamicProperty& prop);
    
    virtual void beforeChangeFilename(const path& newfilename);
    virtual void beforeChangeSubSection(const std::string newSubSection);

  public:
    static std::string XmlCompatible(const std::string & name);

  public:
    XmlDocument* doc() { return mpDocumentNode.get(); }

  private:
    std::unique_ptr<XmlDocument> mpDocumentNode;
    XmlNode                      mCurrentNode;

    bool                         mFileChanged;
  };
}
}