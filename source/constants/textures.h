//
// Created by kevin on 22/10/2021.
//

#ifndef RAYTRACER_CONSTANTS_TEXTURES_H_
#define RAYTRACER_CONSTANTS_TEXTURES_H_

namespace Textures {
namespace {
[[maybe_unused]] inline static std::string BRICK() {
  return
      {"resources/textures/brick_wall_final.bmp"};
}
[[maybe_unused]] inline static std::string CONCRETE() {
  return
      {"resources/textures/tx_concrete.bmp"};
}
[[maybe_unused]] inline static std::string LAVA_DIFFUSE() {
  return
      {"resources/textures/tx_lava_diffuse.bmp"};
}
[[maybe_unused]] inline static std::string LAVA_EMISSION() {
  return
      {"resources/textures/tx_lava_emission.bmp"};
}
[[maybe_unused]] inline static std::string BRATAN() {
  return
      {"resources/textures/tx_ehrenbratan.bmp"};
}
}
}

#endif //RAYTRACER_CONSTANTS_TEXTURES_H_
