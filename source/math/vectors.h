//
// Created by kevin on 11/10/2021.
//

#ifndef PATHTRACER_MATH_VECTORS_H_
#define PATHTRACER_MATH_VECTORS_H_

#include "../scene/scene.h"

class Vectors {
 public:
  static std::pair<float, float> IntersectRay(glm::vec3 &, glm::vec3 &, TexturedSphere &);
  static glm::vec3 CreateEyeRay(glm::vec3 &, glm::vec3 &, glm::vec3 &, glm::vec2 &, float, float);
  static std::pair<glm::vec3, TexturedSphere &> TraceRay(Scene &,
                                                         glm::vec3 &,
                                                         glm::vec3 &,
                                                         float = 0,
                                                         float = std::numeric_limits<float>::infinity());
  static glm::vec3 GenerateRandomVector(glm::vec3 &);
};

#endif //PATHTRACER_MATH_VECTORS_H_
