#include "IDynamicPropertyParser.h"

// STL
#include <sstream>

// GSTD
#include "Exception.h"

// BOOST
#include "boost/filesystem.hpp"

namespace gris
{
namespace gstd
{

  /** \brief make a new IDynamicPropertyParser object. 
  */
  IDynamicPropertyParser::IDynamicPropertyParser()
    : mFilename()
    , mWarnings()
    , mSubSection()
  {
  }

  /** \brief Read the file and set the according parameters in the DynamicProperty.
  */
  void IDynamicPropertyParser::read(DynamicProperty & prop, const path & filename, const std::string& subsection)
  {
    if (filename.empty())
      throw GSTD_EXCEPTION("Cannot read from empty filename.");

    namespace fs = boost::filesystem;
    if (!fs::exists(filename))
      throw GSTD_EXCEPTION_STREAM(filename << " does not exist.");

    bool bFilenameChanged = filename != mFilename;
    if (bFilenameChanged)
      beforeChangeFilename(filename);
    mFilename = filename;

    if (mSubSection != subsection || bFilenameChanged)
      beforeChangeSubSection(subsection);
    mSubSection = subsection;

    readImpl(prop);
  }

  /** \brief write the according parameters from DynamicProperty to the file (stored filename, if no filename is given.
  */
  void IDynamicPropertyParser::write(const DynamicProperty & prop, const path & filename, const std::string& subsection)
  {
    if (filename.empty())
      throw GSTD_EXCEPTION("Cannot read from empty filename.");

    namespace fs = boost::filesystem;
    if (!fs::exists(filename.parent_path()))
      throw GSTD_EXCEPTION_STREAM("Parent folder of file (" << filename.parent_path() << ") does not exist.");
    if (fs::exists(filename))
      appendWarning("Overwriting " + filename.string() + ", which already existed.");

    bool bFilenameChanged = filename != mFilename;
    if (bFilenameChanged)
      beforeChangeFilename(filename);
    mFilename = filename;

    if (mSubSection != subsection || bFilenameChanged)
      beforeChangeSubSection(subsection);
    mSubSection = subsection;

    writeImpl(prop);
  }

  void IDynamicPropertyParser::appendWarning(const std::string & warning)
  {
    mWarnings += (mWarnings.empty() ? "" : "\n") + warning;
  }

}
}