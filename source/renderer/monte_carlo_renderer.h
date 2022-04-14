//
// Created by kevin on 21/10/2021.
//

#ifndef PATHTRACER_RENDERER_BITMAP_RENDERER_H_
#define PATHTRACER_RENDERER_BITMAP_RENDERER_H_

#include <chrono>
#include "renderer.h"
#include "../math/prng.h"

template <color_strategy T, is_resolution U>
class MonteCarloRenderer : public Renderer<T, U>
{
public:
  MonteCarloRenderer();
  [[maybe_unused]] explicit MonteCarloRenderer(std::pair<int, int>) noexcept;
  glm::vec3 ComputeColor(glm::vec3, glm::vec3) override;
};

template <color_strategy T, is_resolution U>
[[maybe_unused]] MonteCarloRenderer<T, U>::MonteCarloRenderer() : Renderer<T, U>() {}

template <color_strategy T, is_resolution U>
[[maybe_unused]] MonteCarloRenderer<T, U>::MonteCarloRenderer(std::pair<int, int> res) noexcept : Renderer<T, U>(res) {}

template <color_strategy T, is_resolution U>
glm::vec3 MonteCarloRenderer<T, U>::ComputeColor(glm::vec3 origin, glm::vec3 direction)
{
  glm::vec3 resultingColor;
  auto hp = Vectors::TraceRay(this->m_scene, origin, direction, 0, std::numeric_limits<float>::infinity());
  auto &hit_point = hp.first;
  auto collider = hp.second;
  auto contact_normal = glm::normalize(hit_point - collider.origin());
  float correction = 0.001f;
  hit_point += correction * contact_normal;
  hit_point += correction * -1 * direction;

  if (PRNG::RandomNumber(0.0f, 1.0f) < this->m_probability || collider.radius() == 0.0f)
  {
    resultingColor = collider.GetEmissiveColor(contact_normal);
  }
  else
  {
    auto random_vector = Vectors::GenerateRandomVector(contact_normal);
    resultingColor = collider.GetEmissiveColor(contact_normal) + ((2.0f * static_cast<float>(M_PI)) / (1.0f - this->m_probability)) * glm::dot(random_vector, contact_normal) * (collider.BDRF(contact_normal, direction, random_vector) * ComputeColor(hit_point, random_vector));
  }
  return resultingColor;
}

#endif //PATHTRACER_RENDERER_BITMAP_RENDERER_H_
