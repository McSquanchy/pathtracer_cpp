//
// Created by kevin on 21/09/2021.
//

#include "glm.hpp"
#include "SDL2/SDL.h"

#ifndef RAYTRACER_COLOR_COLORMODE_H
#define RAYTRACER_COLOR_COLORMODE_H

class ColorMode {
 public:
  virtual ~ColorMode() = default;
  virtual void process(glm::vec3 &) const = 0;
  [[nodiscard]] static glm::vec3 lerp(glm::vec3, glm::vec3, float);
};

class RGB : public ColorMode {
 public:
  RGB() = default;
  void process(glm::vec3 &) const override;
};

class sRGB : public ColorMode {
 public:
  sRGB() = default;
  void process(glm::vec3 &) const override;
 private:
  static constexpr float m_gamma = 2.2f;
};

template<typename T>
concept color_strategy = std::derived_from<T, ColorMode>;

#endif //RAYTRACER_COLOR_COLORMODE_H
