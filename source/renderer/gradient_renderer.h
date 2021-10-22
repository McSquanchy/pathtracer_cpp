//
// Created by kevin on 11/10/2021.
//

#include "renderer.h"
#include "../constants/colors.h"
#include <typeinfo>

#ifndef RAYTRACER_RENDERER_GRADIENT_RENDERER_H_
#define RAYTRACER_RENDERER_GRADIENT_RENDERER_H_

template<color_strategy T, is_resolution U>
class GradientRenderer : public Renderer<T, U> {
 public:
  GradientRenderer();
  [[maybe_unused]] explicit GradientRenderer(std::pair<int, int>) noexcept;
  glm::vec3 ComputeColor(glm::vec3, glm::vec3) override;
  void draw() noexcept override;
};

template<color_strategy T, is_resolution U>
[[maybe_unused]] GradientRenderer<T, U>::GradientRenderer() : Renderer<T, U>() {}

template<color_strategy T, is_resolution U>
[[maybe_unused]] GradientRenderer<T, U>::GradientRenderer(std::pair<int, int> res) noexcept : Renderer<T, U>(res) {}

template<color_strategy T, is_resolution U>
void GradientRenderer<T, U>::draw() noexcept {
  SDL_RenderClear(this->m_window_renderer.get());
  glm::vec3 start(1.0f, 0, 0);
  glm::vec3 end(0, 1.0f, 0);
  auto width = 1.0f / this->m_resolution->width();

  std::unique_ptr<ColorMode> other_variant;
  if (typeid(T) == typeid(RGB)) { // I know, using RTTI is discouraged. Didn't find any other solution
    other_variant = std::make_unique<sRGB>();
  } else {
    other_variant = std::make_unique<RGB>();
  }

  for (auto x = this->m_sampling / 2; x < this->m_resolution->width(); x += this->m_sampling) {
    auto newColor = ColorMode::lerp(start, end, static_cast<float>(x) * width);

    this->m_color_mode->process(newColor);
    if (this->m_sampling == 1) {
      SDL_SetRenderDrawColor(this->m_window_renderer.get(),
                             static_cast<int>(newColor[0]),
                             static_cast<int>(newColor[1]),
                             static_cast<int>(newColor[2]),
                             static_cast<int>(newColor[3]));
      SDL_RenderDrawLine(this->m_window_renderer.get(),
                         x,
                         0,
                         x,
                         this->m_resolution->height() / 2 - 10);

      newColor = ColorMode::lerp(start, end, static_cast<float>(x) * width);
      other_variant->process(newColor);

      SDL_SetRenderDrawColor(this->m_window_renderer.get(),
                             static_cast<int>(newColor[0]),
                             static_cast<int>(newColor[1]),
                             static_cast<int>(newColor[2]),
                             255);
      SDL_RenderDrawLine(this->m_window_renderer.get(),
                         x,
                         this->m_resolution->height() / 2 + 10,
                         x,
                         this->m_resolution->height());
    } else {
      newColor = ColorMode::lerp(start, end, static_cast<float>(x) * width);
      this->m_color_mode->process(newColor);
      SDL_SetRenderDrawColor(this->m_window_renderer.get(),
                             static_cast<int>(newColor[0]),
                             static_cast<int>(newColor[1]),
                             static_cast<int>(newColor[2]),
                             255);
      SDL_Rect grid_rect;
      grid_rect.x = x - this->m_sampling / 2;
      grid_rect.y = 0;
      grid_rect.w = this->m_sampling;
      grid_rect.h = this->m_resolution->height() / 2 - 10;
      SDL_RenderFillRect(this->m_window_renderer.get(), &grid_rect);

      newColor = ColorMode::lerp(start, end, static_cast<float>(x) * width);
      other_variant->process(newColor);

      SDL_SetRenderDrawColor(this->m_window_renderer.get(),
                             static_cast<int>(newColor[0]),
                             static_cast<int>(newColor[1]),
                             static_cast<int>(newColor[2]),
                             255);

      grid_rect.x = x - this->m_sampling / 2;
      grid_rect.y = this->m_resolution->height() / 2 + 10;
      grid_rect.w = this->m_sampling;
      grid_rect.h = this->m_resolution->height() / 2 - 10;
      SDL_RenderFillRect(this->m_window_renderer.get(), &grid_rect);
    }
  }
  std::cout << "100% done\n";
  SDL_RenderPresent(this->m_window_renderer.get());
}

template<color_strategy T, is_resolution U>
glm::vec3 GradientRenderer<T, U>::ComputeColor([[maybe_unused]] glm::vec3 origin, [[maybe_unused]] glm::vec3 d) {
  return Colors::BACKGROUND();
}

#endif //RAYTRACER_RENDERER_GRADIENT_RENDERER_H_
