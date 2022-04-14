//
// Created by kevin on 29/09/2021.
//

#ifndef PATHTRACER_GEOMETRY_GEOMETRY_H_
#define PATHTRACER_GEOMETRY_GEOMETRY_H_

#include <vec4.hpp>
#include <vec3.hpp>
#include <ostream>
#include <cmath>
#include <geometric.hpp>
#include <iostream>
#include "../constants/colors.h"

class Geometry
{
public:
  virtual ~Geometry() = default;
  [[nodiscard]] virtual glm::vec3 &origin() = 0;
  [[nodiscard]] virtual glm::vec3 &diffuse() = 0;
  [[nodiscard]] virtual glm::vec3 &emission() = 0;
  [[nodiscard]] virtual glm::vec3 &specular() = 0;
  [[nodiscard]] virtual glm::vec3 BDRF() = 0;
  [[nodiscard]] virtual glm::vec3 BDRF(glm::vec3 &, glm::vec3 &, glm::vec3 &) = 0;
  virtual void print(std::ostream &stream) const = 0;

protected:
  glm::vec3 m_diffuse;
  glm::vec3 m_emission;
  glm::vec3 m_specular;
  glm::vec3 m_origin;
  Geometry()
      : m_diffuse(Colors::BLACK()),
        m_emission(Colors::BLACK()),
        m_specular(Colors::BLACK()),
        m_origin({0, 0, 0}) {}
  Geometry(glm::vec3 origin, glm::vec3 color)
      : m_diffuse(color), m_emission(Colors::BACKGROUND()), m_specular(Colors::BACKGROUND()), m_origin(origin) {}
  Geometry(glm::vec3 origin, glm::vec3 color, glm::vec3 emission)
      : m_diffuse(color), m_emission(emission), m_specular(Colors::BACKGROUND()), m_origin(origin) {}
  Geometry(glm::vec3 origin, glm::vec3 color, glm::vec3 emission, glm::vec3 specular)
      : m_diffuse(color), m_emission(emission), m_specular(specular), m_origin(origin) {}
  friend std::ostream &operator<<(std::ostream &stream, const Geometry &geometry)
  {
    geometry.print(stream);
    return stream;
  }
};

#endif //PATHTRACER_GEOMETRY_GEOMETRY_H_
