#include "Math.hpp"


float MathUtils::clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}