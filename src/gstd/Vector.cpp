#include "Vector.h"

namespace gris
{
  template class GRIS_GSTD_API DiscreteVector<int, double, 3u>;
  template class GRIS_GSTD_API DiscreteVector<int, double, 2u>;
  template class GRIS_GSTD_API DiscreteVector<unsigned int, double, 3u>;
  template class GRIS_GSTD_API DiscreteVector<unsigned int, double, 2u>;
  
  template class GRIS_GSTD_API FieldVector<double, double, 3u>;
  template class GRIS_GSTD_API FieldVector<double, double, 2u>;
  template class GRIS_GSTD_API FieldVector<float, float, 3u>;
  template class GRIS_GSTD_API FieldVector<float, float, 2u>;
}