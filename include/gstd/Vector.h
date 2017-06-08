#pragma once

#include "gstd_api.h"
#include "gstd/FieldVector.h"

namespace gris
{
  using Vec3i = DiscreteVector<int, double, 3u>;
  using Vec2i = DiscreteVector<int, double, 2u>;
  using Vec3u = DiscreteVector<size_t, double, 3u>;
  using Vec2u = DiscreteVector<size_t, double, 2u>;
  
  using Vec3d = FieldVector<double, double, 3u>;
  using Vec2d = FieldVector<double, double, 2u>;
  using Vec3f = FieldVector<double, double, 3u>;
  using Vec2f = FieldVector<double, double, 2u>;
}
