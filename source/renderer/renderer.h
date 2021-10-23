//
// Created by kevin on 27/09/2021.
//

#include <iostream>
#include <memory>
#include <utility>
#include "SDL2/SDL.h"
#include "../color/colormode.h"
#include "../constants/colors.h"
#include "../constants/resolutions.h"
#include "../scene/scene.h"
#include "../buffer/buffer.h"
#include <cstdlib>
#include <random>
#include <iomanip>

#ifndef RAYTRACER_RENDERER_RENDERER_H_
#define RAYTRACER_RENDERER_RENDERER_H_

template<color_strategy T, is_resolution U>
class Renderer {
 public:
  Renderer();
  [[maybe_unused]] explicit Renderer(std::pair<int, int>) noexcept;
  ~Renderer() = default;
  [[maybe_unused]] void run() noexcept;
  [[maybe_unused]] void setScene(const Scene &);
  [[maybe_unused]] void setCamera(const glm::vec3 &, const glm::vec3 &) noexcept;
  [[maybe_unused]] void setFov(float) noexcept;
  void setSampling(int samples) {
    m_sampling = samples;
  }

  void setRenderQuality(int rays_per_pixel) {
    m_rays_per_pixel = rays_per_pixel;
  }
 protected:
  virtual void draw() noexcept = 0;
  virtual glm::vec3 ComputeColor(glm::vec3, glm::vec3) = 0;
  void drawPixel(glm::vec2 &, glm::vec3 &);
  std::unique_ptr<T> m_color_mode;
  std::shared_ptr<U> m_resolution;
  std::unique_ptr<Buffer<U>> m_buffer;
  std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> m_window;
  std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> m_window_renderer;
  SDL_Event m_window_event;
  Scene m_scene;
  glm::vec3 m_eyeOrigin;
  glm::vec3 m_lookAt;
  glm::vec3 m_cameraForward;
  glm::vec3 m_cameraRight;
  glm::vec3 m_cameraUp;
  float m_vertical_fov;
  float m_horizontal_fov;
  float m_probability = 0.2f;
  int m_rays_per_pixel = 1;
  int m_sampling = 1;
};

template<color_strategy T, is_resolution U>
Renderer<T, U>::Renderer()
    : m_color_mode(std::make_unique<T>()),
      m_resolution(std::make_shared<U>()),
      m_buffer(std::make_unique<Buffer<U>>(m_resolution)),
      m_window(std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>(SDL_CreateWindow(
          "Computer Graphics",
          SDL_WINDOWPOS_CENTERED,
          SDL_WINDOWPOS_CENTERED,
          m_resolution->width(),
          m_resolution->height(),
          0), SDL_DestroyWindow)),
      m_window_renderer(std::unique_ptr<SDL_Renderer,
                                        void (*)(SDL_Renderer *)>(SDL_CreateRenderer(
                                                                      m_window.get(),
                                                                      -1,
                                                                      SDL_RENDERER_ACCELERATED
                                                                          | SDL_RENDERER_PRESENTVSYNC),
                                                                  SDL_DestroyRenderer)),
      m_window_event({}),
      m_eyeOrigin({0, 0, 0}),
      m_lookAt({0, 0, 1}),
      m_vertical_fov(36),
      m_horizontal_fov(glm::degrees(
          2 * std::atan(std::tan(glm::radians(m_vertical_fov) / 2) * (m_resolution->aspectRatio())))) {
  if (!m_window) {
    std::cout << "Failed to create window\n";
    std::cout << "Error - " << SDL_GetError() << "\n";
    return;
  }

  if (!m_window_renderer) {
    std::cout << "Failed to get window's surface\n";
    std::cout << "Error - " << SDL_GetError() << "\n";
    return;
  }
}

template<color_strategy T, is_resolution U>
[[maybe_unused]] Renderer<T, U>::Renderer(std::pair<int, int> size) noexcept :
    m_color_mode(std::make_unique<T>()), m_resolution(std::make_shared<U>(size)),
    m_buffer(std::make_unique<Buffer<U>>(m_resolution)),
    m_window(std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>(SDL_CreateWindow(
        "Computer Graphics",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_resolution->width(),
        m_resolution->height(),
        0), SDL_DestroyWindow)),
    m_window_renderer(std::unique_ptr<SDL_Renderer,
                                      void (*)(SDL_Renderer *)>(SDL_CreateRenderer(
                                                                    m_window.get(),
                                                                    -1,
                                                                    SDL_RENDERER_ACCELERATED
                                                                        | SDL_RENDERER_PRESENTVSYNC),
                                                                SDL_DestroyRenderer)), m_window_event({
                                                                                                      }),
    m_eyeOrigin({0, 0, 0}),
    m_lookAt({0, 0, 1}),
    m_vertical_fov(36),
    m_horizontal_fov(glm::degrees(
        2 * std::atan(std::tan(glm::radians(m_vertical_fov) / 2) * (m_resolution->aspectRatio())))) {
  if (!m_window) {
    std::cout << "Failed to create window\n";
    std::cout << "Error - " << SDL_GetError() << "\n";
    return;
  }

  if (!m_window_renderer) {
    std::cout << "Failed to get window's surface\n";
    std::cout << "Error - " << SDL_GetError() << "\n";
    return;
  }
}

template<color_strategy T, is_resolution U>
[[maybe_unused]] void Renderer<T, U>::run() noexcept {
  bool window_active = true;
  draw();
  while (window_active) {
    while (SDL_PollEvent(&m_window_event) > 0) {
      switch (m_window_event.type) {
        case SDL_QUIT:window_active = false;
          break;
      }
    }
  }
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::setScene(const Scene &s) {
  m_scene = s;
}

//template<color_strategy T, is_resolution U>
//std::pair<glm::vec3, Sphere> Renderer<T, U>::TraceRay(glm::vec3 &o, glm::vec3 &D, float t_min, float t_max) {
//  float closest_t = std::numeric_limits<float>::infinity();
//  Sphere closest_sphere;
//
//  for (auto &shape: m_scene.elements()) {
//    auto intersections = IntersectRay(o, D, shape);
//    if (intersections[0] > t_min && intersections[1] < t_max && intersections[0] < closest_t) {
//      closest_t = intersections[0];
//      closest_sphere = shape;
//    }
//    if (intersections[1] > t_min && intersections[1] < t_max && intersections[1] < closest_t) {
//      closest_t = intersections[1];
//      closest_sphere = shape;
//    }
//  }
//  return {{o + closest_t * D}, closest_sphere};
//}
//
//template<color_strategy T, is_resolution U>
//glm::vec2 Renderer<T, U>::IntersectRay(glm::vec3 &origin, glm::vec3 &D, Sphere s) {
//  float &r = s.radius();
//  auto CO = origin - s.origin();
//
//  float a = glm::dot(D, D);
//  float b = 2 * glm::dot(CO, D);
//  float c = glm::dot(CO, CO) - (r * r);
//
//  auto discriminant = (b * b) - (4 * a * c);
//  if (discriminant < 0) {
//    return {std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};
//  }
//
//  auto t1 = (-b + glm::sqrt(discriminant)) / (2 * a);
//  auto t2 = (-b - glm::sqrt(discriminant)) / (2 * a);
//
//  return {t1, t2};
//}
//
//template<color_strategy T, is_resolution U>
//glm::vec3 Renderer<T, U>::CreateEyeRay(glm::vec2 &pixel) {
//  auto f_bar = glm::normalize(m_lookAt - m_eyeOrigin);
//  glm::vec3 up{0, 1, 0};
//  auto r_bar = glm::cross(f_bar, up);
//  if (r_bar[0] < 0) {
//    r_bar *= -1;
//  }
//
//  r_bar = glm::normalize(r_bar);
//
//  auto u_bar = glm::cross(f_bar, r_bar);
//  if (u_bar[0] < 0) {
//    u_bar *= -1;
//  }
//  u_bar = glm::normalize(u_bar);
//
//  auto direction = f_bar + glm::tan(glm::radians(m_horizontal_fov / 2)) * pixel[0] * r_bar
//      + glm::tan(glm::radians(m_vertical_fov / 2)) * pixel[1] * u_bar;
//  return glm::normalize(direction);
//}

//template<color_strategy T, is_resolution U>
//float Renderer<T, U>::get_random_number() {
//  return uniform_dist(random_engine);
//}

//template<color_strategy T, is_resolution U>
//glm::vec3 Renderer<T, U>::createRandomVec(glm::vec3 &normal) {
//  glm::vec3 res(get_random_number(),
//                get_random_number(),
//                get_random_number());
//  while (glm::length(res) > 1) {
//    res = {get_random_number(), get_random_number(), get_random_number()};
//  }
//  return glm::dot(normal, res) > 0 ? res : -res;
//}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::drawPixel(glm::vec2 &pixel, glm::vec3 &color) {
  m_color_mode->process(color);
  SDL_SetRenderDrawColor(m_window_renderer.get(),
                         static_cast<glm::uint8>(color[0]),
                         static_cast<glm::uint8>(color[1]),
                         static_cast<glm::uint8>(color[2]),
                         255);
  SDL_RenderDrawPoint(m_window_renderer.get(),
                      static_cast<int>(pixel[0]),
                      static_cast<int>(pixel[1]));
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::setCamera(const glm::vec3 &eye, const glm::vec3 &lookAt) noexcept {
  m_eyeOrigin = eye;
  m_lookAt = lookAt;
  m_cameraForward = glm::normalize(lookAt - eye); // direction from eye to lookAt point
  glm::vec3 world_up{0, 1, 0}; // up vector
  auto eye_right = glm::cross(world_up, m_cameraForward);
  if (eye_right[0] < 0) { // make it point right
    eye_right *= -1;
  }

  m_cameraRight = glm::normalize(eye_right); // normalize

  auto eye_up = glm::cross(m_cameraForward, eye_right);
  if (eye_up[2] < 0) {
    eye_up *= -1;
  }
  m_cameraUp = glm::normalize(eye_up);
}

template<color_strategy T, is_resolution U>
[[maybe_unused]] void Renderer<T, U>::setFov(const float fov) noexcept {
  m_vertical_fov = fov;
  m_horizontal_fov = glm::degrees(
      2 * std::atan(std::tan(glm::radians(m_vertical_fov) / 2) * (m_resolution->aspectRatio())));
}

//template<color_strategy T, is_resolution U>
//glm::vec4 Renderer<T, U>::BRDF(Sphere &obj, glm::vec3 &normal, glm::vec3 &direction, glm::vec3 &reflection) {
//  glm::vec4 resultingColor;
//
//  normal = glm::normalize(normal);
//  direction = -1.0f * glm::normalize(direction);
//
//  glm::vec3 projection = glm::normalize(normal * (glm::dot(direction, normal)));
//  float mu = 0.01f;
//  float e = 20.0f;
//  if (glm::dot(direction, projection) > (1 - e)) {
//    resultingColor = obj.diffuse() + mu;
//  } else {
//    resultingColor = obj.diffuse();
//  }
//  return resultingColor / static_cast<float>(M_PI);
//}

#endif //RAYTRACER_RENDERER_RENDERER_H_
