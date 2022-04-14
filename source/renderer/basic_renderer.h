//
// Created by kevin on 11/10/2021.
//

#ifndef PATHTRACER_RENDERER_BASIC_RENDERER_H_
#define PATHTRACER_RENDERER_BASIC_RENDERER_H_

#include <chrono>
#include "renderer.h"
#include "../math/vectors.h"

template<color_strategy T, is_resolution U>
class BasicRenderer : public Renderer<T, U> {
 public:
  BasicRenderer();
  [[maybe_unused]] explicit BasicRenderer(std::pair<int, int>) noexcept;
  glm::vec3 ComputeColor(glm::vec3, glm::vec3) override;
};

template<color_strategy T, is_resolution U>
[[maybe_unused]] BasicRenderer<T, U>::BasicRenderer() : Renderer<T, U>() {}

template<color_strategy T, is_resolution U>
[[maybe_unused]] BasicRenderer<T, U>::BasicRenderer(std::pair<int, int> res) noexcept : Renderer<T, U>(res) {}

template<color_strategy T, is_resolution U>
glm::vec3 BasicRenderer<T, U>::ComputeColor(glm::vec3 origin, glm::vec3 direction) {
  auto hp = Vectors::TraceRay(this->m_scene, origin, direction, 0, std::numeric_limits<float>::infinity());
  auto &collider = hp.second;
  return collider.diffuse();
}

#endif //PATHTRACER_RENDERER_BASIC_RENDERER_H_
