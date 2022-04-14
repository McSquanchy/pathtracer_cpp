//
// Created by kevin on 21/10/2021.
//

#ifndef PATHTRACER_GEOMETRY_SPHERE_H_
#define PATHTRACER_GEOMETRY_SPHERE_H_

#include "geometry.h"

class Sphere : public Geometry
{
public:
  Sphere(glm::vec3 origin, float radius, glm::vec3 color) : Geometry(origin, color), m_radius(radius) {}
  Sphere(glm::vec3 origin, float radius, glm::vec3 color, glm::vec3 emission) : Geometry(origin, color, emission),
                                                                                m_radius(radius) {}
  Sphere(glm::vec3 origin, float radius, glm::vec3 color, glm::vec3 emission, glm::vec3 specular) : Geometry(origin,
                                                                                                             color,
                                                                                                             emission,
                                                                                                             specular),
                                                                                                    m_radius(radius) {}
  Sphere() : Geometry(), m_radius(0.0f) {}
  float &radius() { return m_radius; }
  [[nodiscard]] glm::vec3 &origin() override { return m_origin; }
  [[nodiscard]] glm::vec3 &emission() override { return m_emission; }
  [[nodiscard]] glm::vec3 &diffuse() override { return m_diffuse; }
  [[nodiscard]] glm::vec3 &specular() override { return m_specular; }
  [[nodiscard]] glm::vec3 BDRF() override
  {
    return m_diffuse / static_cast<float>(M_PI);
  }

  glm::vec3 BDRF(glm::vec3 &normal, glm::vec3 &direction, glm::vec3 &random_vec) override
  {

    glm::vec3 resultingColor;

    direction = -1.0f * glm::normalize(direction);
    glm::vec3 projection = normal * (glm::dot(direction, normal));
    glm::vec3 reflection = glm::normalize(projection + (projection - direction));
    float mu = 20;
    float e = 0.01f;

    if (glm::dot(random_vec, reflection) > (1 - e))
    {
      resultingColor = diffuse() + mu * specular();
    }
    else
    {
      resultingColor = diffuse();
    }

    return resultingColor / static_cast<float>(M_PI);
  }

  void print(std::ostream &stream) const override
  {
    stream << "Origin: [" << m_origin[0] << "," << m_origin[1] << "," << m_origin[2]
           << "] - Radius: " << m_radius << " - Color: [" << m_diffuse[0] << "," << m_diffuse[1]
           << "," << m_diffuse[2] << "," << m_diffuse[3] << "]";
  }

protected:
  float m_radius;
};

#endif //PATHTRACER_GEOMETRY_SPHERE_H_
