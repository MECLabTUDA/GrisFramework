#pragma once

#include "gstd_api.h"
#include "gstd/VectorImpl.h"

namespace gris
{
  using Vec3i = Vector<int, double, 3u>;
  using Vec2i = Vector<int, double, 2u>;
  using Vec3u = Vector<size_t, double, 3u>;
  using Vec2u = Vector<size_t, double, 2u>;
  
  using Vec3d = Vector<double, double, 3u>;
  using Vec2d = Vector<double, double, 2u>;
  using Vec3f = Vector<double, double, 3u>;
  using Vec2f = Vector<double, double, 2u>;
}
