//
// Created by kevin on 11/10/2021.
//

#ifndef PATHTRACER_MATH_PRNG_H_
#define PATHTRACER_MATH_PRNG_H_

#include <random>

namespace PRNG {
constexpr int seed = 42;

[[maybe_unused]] static float RandomNumber(const float low, const float high) {
  static std::default_random_engine re{seed};
  using Dist = std::uniform_real_distribution<float>;
  static Dist uid{};
  return uid(re, Dist::param_type{low, high});
}

}

#endif //PATHTRACER_MATH_PRNG_H_
