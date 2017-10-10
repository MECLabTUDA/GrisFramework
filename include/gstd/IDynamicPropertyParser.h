#pragma once

#include "gstd_api.h"

#include "dynamicProperty.h"

// boost
#include "boost/filesystem/path.hpp"

// STL
#include <string>

namespace gris
{
namespace gstd
{

  class GRIS_GSTD_API IDynamicPropertyParser
  {
  public:
    using path = boost::filesystem::path;

  public:
    IDynamicPropertyParser();
    virtual ~IDynamicPropertyParser() {};

    virtual void flush() = 0;
  
  public:
    const path&         filename()   const { return mFilename; }
    const std::string&  subSection() const { return mSubSection; }

  public:
    void read(DynamicProperty& prop, const path& filename, const std::string& subsection = "");
    void write(const DynamicProperty& prop, const path& filename, const std::string& subsection = "");

  public:
    const std::string& warnings() const { return mWarnings; }
    void clearWarnings() { mWarnings.clear(); }

  protected:
    void appendWarning(const std::string& warning);

  protected:
    // Internal implementation of the read procedure
    virtual void readImpl(DynamicProperty& prop) = 0;
    // Internal implementation of the write procedure
    virtual void writeImpl(const DynamicProperty& prop) = 0;

    virtual void beforeChangeFilename(const path& newFilename) { };
    virtual void beforeChangeSubSection(const path& newSubsection) { };

  private:
    path mFilename;
    std::string mSubSection;
    std::string mWarnings;
  };

}
}