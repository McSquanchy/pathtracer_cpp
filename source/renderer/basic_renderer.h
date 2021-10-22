//
// Created by kevin on 11/10/2021.
//

#include "renderer.h"
#include "../math/vectors.h"

#ifndef RAYTRACER_RENDERER_BASIC_RENDERER_H_
#define RAYTRACER_RENDERER_BASIC_RENDERER_H_

template<color_strategy T, is_resolution U>
class BasicRenderer : public Renderer<T, U> {
 public:
  BasicRenderer();
  [[maybe_unused]] explicit BasicRenderer(std::pair<int, int>) noexcept;
  glm::vec3 ComputeColor(glm::vec3, glm::vec3) override;
  void draw() noexcept override;
};

template<color_strategy T, is_resolution U>
[[maybe_unused]] BasicRenderer<T, U>::BasicRenderer() : Renderer<T, U>() {}

template<color_strategy T, is_resolution U>
[[maybe_unused]] BasicRenderer<T, U>::BasicRenderer(std::pair<int, int> res) noexcept : Renderer<T, U>(res) {}

template<color_strategy T, is_resolution U>
void BasicRenderer<T, U>::draw() noexcept {
  SDL_RenderClear(this->m_window_renderer.get());
  int percentage = 10;
  for (auto x = this->m_sampling / 2; x < this->m_resolution->width(); x += this->m_sampling) {
    for (auto y = this->m_sampling / 2; y < this->m_resolution->height(); y += this->m_sampling) {
      glm::vec2 pixel{x, y};
      glm::vec2 pixelRelative = glm::vec2((static_cast<float>(x) / this->m_resolution->width() * 2) - 1,
                                          ((static_cast<float>(y) / this->m_resolution->height() * 2) - 1));
      auto ray = Vectors::CreateEyeRay(this->m_cameraForward,
                                       this->m_cameraRight,
                                       this->m_cameraUp,
                                       pixelRelative,
                                       this->m_horizontal_fov,
                                       this->m_vertical_fov);
      auto finalColor = ComputeColor(this->m_eyeOrigin, ray);
      if (this->m_sampling == 1) {
        this->drawPixel(pixel, finalColor);
      } else {
        this->m_color_mode->process(finalColor);
        SDL_SetRenderDrawColor(this->m_window_renderer.get(),
                               static_cast<Uint8>(finalColor[0]),
                               static_cast<Uint8>(finalColor[1]),
                               static_cast<Uint8>(finalColor[2]),
                               static_cast<Uint8>(finalColor[3]));
        SDL_Rect grid_rect;
        grid_rect.x = x - this->m_sampling / 2;
        grid_rect.y = y - this->m_sampling / 2;
        grid_rect.w = this->m_sampling;
        grid_rect.h = this->m_sampling;
        SDL_RenderFillRect(this->m_window_renderer.get(), &grid_rect);
      }
    }
    if ((static_cast<float>(x) / this->m_resolution->width()) * 100 >= percentage) {
      SDL_RenderPresent(this->m_window_renderer.get());
      std::cout << percentage << "% done.\n";
      percentage += 10;
    }
  }
  std::cout << "100% done.\n";
  SDL_RenderPresent(this->m_window_renderer.get());
}

template<color_strategy T, is_resolution U>
glm::vec3 BasicRenderer<T, U>::ComputeColor(glm::vec3 origin, glm::vec3 direction) {
  auto hp = Vectors::TraceRay(this->m_scene, origin, direction, 0, std::numeric_limits<float>::infinity());
  auto &collider = hp.second;
  return collider.diffuse();
}

#endif //RAYTRACER_RENDERER_BASIC_RENDERER_H_
