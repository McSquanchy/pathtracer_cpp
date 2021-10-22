//
// Created by kevin on 11/10/2021.
//

#include "glm.hpp"
#include "../geometry/geometry.h"
#include "vectors.h"
#include "prng.h"

glm::vec3 Vectors::CreateEyeRay(glm::vec3 &eye_forward,
                                glm::vec3 &eye_right,
                                glm::vec3 &eye_up,
                                glm::vec2 &pixel,
                                float h_fov,
                                float v_fov) {
  return glm::normalize(eye_forward + glm::tan(glm::radians(h_fov / 2)) * pixel[0] * eye_right
                            + glm::tan(glm::radians(v_fov / 2)) * pixel[1] * eye_up);
}

std::pair<float, float> Vectors::IntersectRay(glm::vec3 &from, glm::vec3 &to, TexturedSphere& sphere) {
  float &r = sphere.radius();
  auto CO = from - sphere.origin(); // sphere center to eye

  float a = 1;
  float b = 2 * glm::dot(CO, to);
  float c = glm::dot(CO, CO) - (r * r);

  auto discriminant = (b * b) - (4 * a * c);
  if (discriminant < 0) { // only real valued solutions
    return {std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};
  }

  auto t1 = (-b + glm::sqrt(discriminant)) / (2 * a);
  auto t2 = (-b - glm::sqrt(discriminant)) / (2 * a);

  return {t1, t2};
}

std::pair<glm::vec3, TexturedSphere&> Vectors::TraceRay(Scene &scene,
                                                                        glm::vec3 &origin,
                                                                        glm::vec3 &direction,
                                                                        float t_min,
                                                                        float t_max) {
  float closest_t = std::numeric_limits<float>::infinity();
  auto closest_sphere = scene.elements()[0];

  for (auto& shape: scene.elements()) {
    if (shape.radius() == 0) {
      continue;
    }
    auto intersections = IntersectRay(origin, direction, shape);
    if (intersections.first > t_min && intersections.first < t_max && intersections.first < closest_t) {
      closest_t = intersections.first;
      closest_sphere = shape;
    }
    if (intersections.second > t_min && intersections.second < t_max && intersections.second < closest_t) {
      closest_t = intersections.second;
      closest_sphere = shape;
    }
  }
  return {{origin + closest_t * direction}, closest_sphere};
}

glm::vec3 Vectors::GenerateRandomVector(glm::vec3 &normal) {
  glm::vec3 res(PRNG::RandomNumber(-1.0f, 1.0f),
                PRNG::RandomNumber(-1.0f, 1.0f),
                PRNG::RandomNumber(-1.0f, 1.0f));
  while (glm::length(res) > 1) {
    res = {PRNG::RandomNumber(-1.0f, 1.0f), PRNG::RandomNumber(-1.0f, 1.0f),
           PRNG::RandomNumber(-1.0f, 1.0f)};
  }
  res = glm::normalize(res);
  return glm::dot(res, normal) > 0 ? res : -res;
}

