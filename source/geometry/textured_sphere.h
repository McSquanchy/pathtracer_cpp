//
// Created by kevin on 21/10/2021.
//

#ifndef RAYTRACER_GEOMETRY_TEXTURED_SPHERE_H_
#define RAYTRACER_GEOMETRY_TEXTURED_SPHERE_H_

#include <memory>
#include "sphere.h"
#include "../textures/bmp/BitmapTexture.h"

class TexturedSphere : public Sphere {
 public:
  TexturedSphere(glm::vec3 origin, float radius, glm::vec3 color) : Sphere(origin, radius, color),
                                                                    m_tx(std::make_shared<BitmapTexture>(TextureType::NONE)) {}
  TexturedSphere(glm::vec3 origin, float radius, glm::vec3 color, glm::vec3 emission) :
      Sphere(origin,
             radius,
             color,
             emission
      ), m_tx(std::make_shared<BitmapTexture>(TextureType::NONE)) {}
  TexturedSphere(glm::vec3 origin, float radius, glm::vec3 color, glm::vec3 emission, glm::vec3 specular) : Sphere(
      origin,
      radius,
      color,
      emission,
      specular),
                                                                                                            m_tx(
                                                                                                                std::make_shared<
                                                                                                                    BitmapTexture
                                                                                                                >(
                                                                                                                    TextureType::NONE)) {}

  TexturedSphere(glm::vec3 origin,
                 float radius,
                 glm::vec3 color,
                 glm::vec3 emission,
                 glm::vec3 specular,
                 const std::string &texture_path, TextureType type) : Sphere(
      origin,
      radius,
      color,
      emission,
      specular), m_tx(std::make_shared<BitmapTexture>(type)) {
    if (type != TextureType::NONE)
      m_tx->Read(texture_path.c_str());
  }

  TexturedSphere() : Sphere::Sphere(), m_tx(std::make_shared<BitmapTexture>(TextureType::NONE)) {}
  [[nodiscard]] glm::vec3 BDRF() override {
    return m_diffuse / static_cast<float>(M_PI);
  }
  [[nodiscard]] glm::vec3 GetTextureColor(glm::vec3 &normal) {
    int x_coordinate = static_cast<int>(((atan2f(normal.x, normal.z) / (2 * M_PI)) + 0.5f) * (m_tx->Width() - 1));
    int y_coordinate = static_cast<int>(acosf(-normal.y) / M_PI * (m_tx->Height() - 1));
    return m_tx->GetColor(x_coordinate, y_coordinate) * m_brightness;
  }

//  glm::vec3 BDRF(glm::vec3 &normal, glm::vec3 &direction, glm::vec3 &random_vec) override {
//    glm::vec3 resultingColor;
//
//    direction = -1.0f * glm::normalize(direction);
//    glm::vec3 projection = normal * (glm::dot(direction, normal)); // blue
//    glm::vec3 reflection = glm::normalize(projection + (projection - direction));
//    float mu = 20;
//    float e = 0.01f;
//
//    if (glm::dot(random_vec, reflection) > (1 - e)) {
//      resultingColor = diffuse() + mu * specular();
//    } else {
//      resultingColor = diffuse();
//    }
//
//    return resultingColor / static_cast<float>(M_PI);
//  }

  glm::vec3 BDRF(glm::vec3 &normal, glm::vec3 &direction, glm::vec3 &random_vec) override {
    glm::vec3 resultingColor;
    direction = -1.0f * glm::normalize(direction);
    glm::vec3 projection = normal * (glm::dot(direction, normal));
    glm::vec3 reflection = glm::normalize(projection + (projection - direction));
    float mu = 20;
    float e = 0.01f;
    if (glm::dot(random_vec, reflection) > (1 - e)) {
      if (m_tx->isDiffuse()) {
        resultingColor = GetTextureColor(normal) /* diffuse */ + mu * specular();
      } else if (m_tx->isSpecular()) {
        resultingColor = diffuse() + mu * GetTextureColor(normal) /* specular */;
      } else {
        resultingColor = diffuse() + mu * specular();
      }
    } else {
      if (m_tx->isDiffuse()) {
        resultingColor = GetTextureColor(normal) /* diffuse */;
      } else {
        resultingColor = diffuse();
      }
    }

    return resultingColor / static_cast<float>(M_PI);
  }

//  void print(std::ostream &stream) const override {
//    stream << "Origin: [" << m_origin[0] << "," << m_origin[1] << "," << m_origin[2]
//           << "] - Radius: " << m_radius << " - Color: [" << m_diffuse[0] << "," << m_diffuse[1]
//           << "," << m_diffuse[2] << "," << m_diffuse[3] << "]";
//  }

 private:
  float m_brightness = 1;
  std::shared_ptr<BitmapTexture> m_tx;
};

#endif //RAYTRACER_GEOMETRY_TEXTURED_SPHERE_H_
