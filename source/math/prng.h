//
// Created by kevin on 11/10/2021.
//

#ifndef RAYTRACER_MATH_PRNG_H_
#define RAYTRACER_MATH_PRNG_H_

namespace PRNG {
constexpr int seed = 42;

[[maybe_unused]] static float RandomNumber(const float low, const float high) {
  static std::default_random_engine re{seed};
  using Dist = std::uniform_real_distribution<float>;
  static Dist uid{};
  return uid(re, Dist::param_type{low, high});
}

}

#endif //RAYTRACER_MATH_PRNG_H_
