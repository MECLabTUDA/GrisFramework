#include "Vector.h"

namespace gris
{
  template class GRIS_GSTD_API Vector<int, double, 3u>;
  template class GRIS_GSTD_API Vector<int, double, 2u>;
  template class GRIS_GSTD_API Vector<unsigned int, double, 3u>;
  template class GRIS_GSTD_API Vector<unsigned int, double, 2u>;
  
  template class GRIS_GSTD_API Vector<double, double, 3u>;
  template class GRIS_GSTD_API Vector<double, double, 2u>;
  template class GRIS_GSTD_API Vector<float, float, 3u>;
  template class GRIS_GSTD_API Vector<float, float, 2u>;
}