//
// Created by kevin on 21/10/2021.
//

#ifndef PATHTRACER_GEOMETRY_TEXTURED_SPHERE_H_
#define PATHTRACER_GEOMETRY_TEXTURED_SPHERE_H_

#include <memory>
#include "sphere.h"
#include "../textures/bmp/BitmapTexture.h"

class TexturedSphere : public Sphere
{
public:
  TexturedSphere(glm::vec3 origin, float radius, glm::vec3 color) : Sphere(origin, radius, color) {}
  TexturedSphere(glm::vec3 origin, float radius, glm::vec3 color, glm::vec3 emission) : Sphere(origin,
                                                                                               radius,
                                                                                               color,
                                                                                               emission) {}
  TexturedSphere(glm::vec3 origin, float radius, glm::vec3 color, glm::vec3 emission, glm::vec3 specular) : Sphere(
                                                                                                                origin,
                                                                                                                radius,
                                                                                                                color,
                                                                                                                emission,
                                                                                                                specular) {}

  TexturedSphere(glm::vec3 origin,
                 float radius,
                 glm::vec3 color,
                 glm::vec3 emission,
                 glm::vec3 specular,
                 const std::string &filepath,
                 TextureType type) : Sphere(origin, radius, color, emission, specular)
  {
    switch (type)
    {
    case TextureType::DIFFUSE:
      m_tx_diffuse = std::make_shared<BitmapTexture>(type);
      m_tx_diffuse->Read(filepath.c_str());
      break;
    case TextureType::EMISSIVE:
      m_tx_emission = std::make_shared<BitmapTexture>(type);
      m_tx_emission->Read(filepath.c_str());
      break;
    case TextureType::SPECULAR:
      m_tx_specular = std::make_shared<BitmapTexture>(type);
      m_tx_specular->Read(filepath.c_str());
      break;
    case TextureType::NONE:
      break;
    }
  }

  TexturedSphere() : Sphere::Sphere() {}
  [[nodiscard]] glm::vec3 BDRF() override
  {
    return m_diffuse / static_cast<float>(M_PI);
  }

  [[nodiscard]] glm::vec3 &GetDiffuseColor(glm::vec3 &normal)
  {
    if (m_tx_diffuse)
    {
      int x_coordinate =
          static_cast<int>(((atan2f(normal.x, normal.z) / (2 * M_PI)) + 0.5f) * (m_tx_diffuse->Width() - 1));
      int y_coordinate = static_cast<int>(acosf(-normal.y) / M_PI * (m_tx_diffuse->Height() - 1));
      return m_tx_diffuse->GetColor(x_coordinate, y_coordinate);
    }
    else
      return m_diffuse;
  }

  [[nodiscard]] glm::vec3 GetEmissiveColor(glm::vec3 &normal)
  {
    if (m_tx_emission)
    {
      int x_coordinate =
          static_cast<int>(((atan2f(normal.x, normal.z) / (2 * M_PI)) + 0.5f) * (m_tx_emission->Width() - 1));
      int y_coordinate = static_cast<int>(acosf(-normal.y) / M_PI * (m_tx_emission->Height() - 1));
      return m_brightness * m_tx_emission->GetColor(x_coordinate, y_coordinate);
    }
    else
    {
      return m_emission;
    }
  }

  [[nodiscard]] glm::vec3 &GetSpecularColor(glm::vec3 &normal)
  {
    if (m_tx_specular)
    {
      int x_coordinate =
          static_cast<int>(((atan2f(normal.x, normal.z) / (2 * M_PI)) + 0.5f) * (m_tx_specular->Width() - 1));
      int y_coordinate = static_cast<int>(acosf(-normal.y) / M_PI * (m_tx_specular->Height() - 1));
      return m_tx_specular->GetColor(x_coordinate, y_coordinate);
    }
    else
      return m_specular;
  }

  bool addTexture(const std::string &path, TextureType type)
  {
    switch (type)
    {
    case TextureType::NONE:
      return false;
    case TextureType::DIFFUSE:
      if (m_tx_diffuse == nullptr)
      {
        m_tx_diffuse = m_tx_diffuse = std::make_shared<BitmapTexture>(type);
      }
      m_tx_diffuse->Read(path.c_str());
      return true;
    case TextureType::EMISSIVE:
      if (m_tx_emission == nullptr)
      {
        m_tx_emission = std::make_shared<BitmapTexture>(type);
      }
      m_tx_emission->Read(path.c_str());
      return true;
    case TextureType::SPECULAR:
      if (m_tx_specular == nullptr)
      {
        m_tx_specular = std::make_shared<BitmapTexture>(type);
      }
      m_tx_specular->Read(path.c_str());
      return true;
    default:
      return false;
    }
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
      resultingColor = GetDiffuseColor(normal) + mu * GetSpecularColor(normal);
    }
    else
    {
      resultingColor = GetDiffuseColor(normal);
    }

    return resultingColor / static_cast<float>(M_PI);
  }

private:
  float m_brightness = 1.2f;
  std::shared_ptr<BitmapTexture> m_tx_diffuse;
  std::shared_ptr<BitmapTexture> m_tx_emission;
  std::shared_ptr<BitmapTexture> m_tx_specular;
};

#endif //PATHTRACER_GEOMETRY_TEXTURED_SPHERE_H_
