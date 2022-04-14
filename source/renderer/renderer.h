//
// Created by kevin on 27/09/2021.
//

#ifndef PATHTRACER_RENDERER_RENDERER_H_
#define PATHTRACER_RENDERER_RENDERER_H_

#include <iostream>
#include <memory>
#include <utility>
#include <chrono>
#include <cstdlib>
#include <random>
#include <iomanip>
#include <functional>
#include "SDL2/SDL.h"
#include "../color/colormode.h"
#include "../constants/colors.h"
#include "../constants/resolutions.h"
#include "../scene/scene.h"
#include "../buffer/buffer.h"
#include "../math/vectors.h"

template <color_strategy T, is_resolution U>
class Renderer
{
public:
  Renderer();
  [[maybe_unused]] explicit Renderer(std::pair<int, int>) noexcept;
  ~Renderer() = default;
  [[maybe_unused]] virtual void Run() noexcept;
  [[maybe_unused]] void SetScene(const Scene &);
  [[maybe_unused]] void SetCamera(const glm::vec3 &, const glm::vec3 &) noexcept;
  [[maybe_unused]] void SetFOV(float) noexcept;
  [[maybe_unused]] void SetSampling(int = 1);
  [[maybe_unused]] void SetRenderQuality(int);
  [[maybe_unused]] void SetDisplayOnScreen(bool);
  [[maybe_unused]] void SetWriteToFile(bool);
  [[maybe_unused]] void SetAA(bool);
  [[maybe_unused]] void SetAASigma(float);

protected:
  virtual void Draw() noexcept;
  virtual glm::vec3 ComputeColor(glm::vec3, glm::vec3) = 0;
  virtual void RenderToScreen();
  void DrawPixel(int x, int y, glm::vec3 &);
  std::unique_ptr<T> m_color_mode;
  std::shared_ptr<U> m_resolution;
  std::unique_ptr<Buffer<U>> m_buffer;
  SDL_Event m_window_event;
  Scene m_scene;
  glm::vec3 m_eyeOrigin;
  glm::vec3 m_cameraForward{};
  glm::vec3 m_cameraRight{};
  glm::vec3 m_cameraUp{};
  float m_vertical_fov;
  float m_horizontal_fov;
  float m_probability = 0.2f;
  int m_rays_per_pixel = 1;
  int m_sampling = 1;
  bool m_display_on_screen = true;
  bool m_write_to_file = false;
  bool m_anti_aliasing = false;
  std::default_random_engine engine{42};
  float m_sigma = 0.5f;
  std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>> m_window = nullptr;
  std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>> m_window_renderer = nullptr;
  bool initializeSDL();
};

template <color_strategy T, is_resolution U>
Renderer<T, U>::Renderer()
    : m_color_mode(std::make_unique<T>()),
      m_resolution(std::make_shared<U>()),
      m_buffer(std::make_unique<Buffer<U>>(m_resolution)),
      m_window_event({}),
      m_eyeOrigin({0, 0, 0}),
      m_vertical_fov(36),
      m_horizontal_fov(glm::degrees(
          2 * std::atan(std::tan(glm::radians(m_vertical_fov) / 2) * (m_resolution->aspectRatio()))))
{
  glm::vec3 m_lookAt{0, 0, 1};
  SetCamera(m_eyeOrigin, m_lookAt);
}

template <color_strategy T, is_resolution U>
[[maybe_unused]] Renderer<T, U>::Renderer(std::pair<int, int> size) noexcept
    : m_color_mode(std::make_unique<T>()), m_resolution(std::make_shared<U>(size)),
      m_buffer(std::make_unique<Buffer<U>>(m_resolution)),
      m_window_event({}),
      m_eyeOrigin({0, 0, 0}),
      m_vertical_fov(36),
      m_horizontal_fov(glm::degrees(
          2 * std::atan(std::tan(glm::radians(m_vertical_fov) / 2) * (m_resolution->aspectRatio()))))
{
  glm::vec3 m_lookAt{0, 0, 1};
  SetCamera(m_eyeOrigin, m_lookAt);
}

template <color_strategy T, is_resolution U>
[[maybe_unused]] [[maybe_unused]] void Renderer<T, U>::Run() noexcept
{
  bool window_active = true;
  if (!m_display_on_screen && !m_write_to_file)
  {
    std::cerr << "Neither displaying on screen nor writing to file. Aborting...\n";
    return;
  }
  Draw();
  if (m_display_on_screen)
  {
    RenderToScreen();
    while (window_active)
    {
      while (SDL_PollEvent(&m_window_event) > 0)
      {
        switch (m_window_event.type)
        {
        case SDL_QUIT:
          window_active = false;
          break;
        }
      }
    }
  }

  if (m_write_to_file)
  {
    std::time_t result = std::time(nullptr);
    std::asctime(std::localtime(&result));
    std::string filename("documentation/renders/mc_" + std::to_string(m_rays_per_pixel) + "rpp_" + std::to_string(m_sampling) + "sampling_" + std::to_string(result) + ".bmp");
    m_buffer->WriteToFile(filename.c_str());
  }
}

template <color_strategy T, is_resolution U>
void Renderer<T, U>::Draw() noexcept
{
  int processedPixels = 0;
  int width = m_resolution->width();
  int height = m_resolution->height();
  auto start_time = std::chrono::high_resolution_clock::now();

#pragma omp parallel for collapse(2) shared(std::cout, width, height, engine, processedPixels, start_time) default(none)
  for (auto x = m_sampling / 2; x < width; x += m_sampling)
  {
    for (auto y = m_sampling / 2; y < height; y += m_sampling)
    {
      glm::vec2 pixel{x, y};
      glm::vec2 pixelRelative = glm::vec2((static_cast<float>(x) / static_cast<float>(width) * 2) - 1,
                                          ((static_cast<float>(y) / static_cast<float>(height) * 2) - 1));
      auto ray = Vectors::CreateEyeRay(m_cameraForward,
                                       m_cameraRight,
                                       m_cameraUp,
                                       pixelRelative,
                                       m_horizontal_fov,
                                       m_vertical_fov);
      glm::vec3 finalColor{0, 0, 0};

      if (m_anti_aliasing)
      { // use anti aliasing
        // normal dist
        std::normal_distribution<float> x_distribution(static_cast<float>(x), m_sigma);
        std::normal_distribution<float> y_distribution(static_cast<float>(y), m_sigma);
        for (auto i = 0; i < m_rays_per_pixel; ++i)
        {
          finalColor += ComputeColor(m_eyeOrigin, ray);
          float yGauss = y_distribution(engine);
          float xGauss = x_distribution(engine);
          pixelRelative = glm::vec2((static_cast<float>(xGauss) / static_cast<float>(width) * 2) - 1,
                                    ((static_cast<float>(yGauss) / static_cast<float>(height) * 2) - 1));
          ray = Vectors::CreateEyeRay(m_cameraForward,
                                      m_cameraRight,
                                      m_cameraUp,
                                      pixelRelative,
                                      m_horizontal_fov,
                                      m_vertical_fov);
        }
      }
      else
      { // regular computation - no anti aliasing
        for (auto i = 0; i < m_rays_per_pixel; ++i)
        {
          finalColor += ComputeColor(m_eyeOrigin, ray);
        }
      }
      finalColor /= m_rays_per_pixel;
      m_buffer->SetPixelColor(x, y, finalColor);

#pragma omp atomic
      ++processedPixels;

      if (processedPixels > 0 && processedPixels % ((width * height) / 10) == 0)
      {
        auto elapsed = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                                               std::chrono::high_resolution_clock::now() - start_time)
                                               .count()) /
                       1e+6;
        printf("%.0f%% rendered. Total rendering time: %.2fms\n",
               ((static_cast<float>(processedPixels) / static_cast<float>(width * height)) * 100),
               elapsed);
      }
    }
  }
  if (m_sampling != 1) // beacuse of multi-threading this ensures that 100% progress is printed
    std::cout << "100% done.\n";
}

template <color_strategy T, is_resolution U>
void Renderer<T, U>::RenderToScreen()
{
  if (!initializeSDL())
    return;
  SDL_RenderClear(m_window_renderer.get());
  for (auto x = m_sampling / 2; x < m_resolution->width(); x += m_sampling)
  {
    for (auto y = m_sampling / 2; y < m_resolution->height(); y += m_sampling)
    {
      m_color_mode->Process(m_buffer->GetPixelColor(x, y));
      if (m_sampling == 1)
      {
        DrawPixel(x, y, m_buffer->GetPixelColor(x, y));
      }
      else
      {
        SDL_SetRenderDrawColor(m_window_renderer.get(),
                               static_cast<Uint8>(m_buffer->GetPixelColor(x, y)[0]),
                               static_cast<Uint8>(m_buffer->GetPixelColor(x, y)[1]),
                               static_cast<Uint8>(m_buffer->GetPixelColor(x, y)[2]),
                               255);
        SDL_Rect grid_rect;
        grid_rect.x = x - m_sampling / 2;
        grid_rect.y = y - m_sampling / 2;
        grid_rect.w = m_sampling;
        grid_rect.h = m_sampling;
        SDL_RenderFillRect(m_window_renderer.get(), &grid_rect);
      }
    }
  }
  SDL_RenderPresent(m_window_renderer.get());
}

template <color_strategy T, is_resolution U>
void Renderer<T, U>::SetScene(const Scene &s)
{
  m_scene = s;
}

template <color_strategy T, is_resolution U>
void Renderer<T, U>::DrawPixel(int x, int y, glm::vec3 &color)
{
  SDL_SetRenderDrawColor(m_window_renderer.get(),
                         static_cast<glm::uint8>(color[0]),
                         static_cast<glm::uint8>(color[1]),
                         static_cast<glm::uint8>(color[2]),
                         255);
  SDL_RenderDrawPoint(m_window_renderer.get(),
                      static_cast<int>(x),
                      static_cast<int>(y));
}

template <color_strategy T, is_resolution U>
void Renderer<T, U>::SetCamera(const glm::vec3 &eye, const glm::vec3 &lookAt) noexcept
{
  m_eyeOrigin = eye;
  m_cameraForward = glm::normalize(lookAt - eye); // direction from eye to lookAt point
  glm::vec3 world_up{0, 1, 0};                    // up vector
  auto eye_right = glm::cross(world_up, m_cameraForward);
  if (eye_right[0] < 0)
  { // make it point right
    eye_right *= -1;
  }

  m_cameraRight = glm::normalize(eye_right); // normalize

  auto eye_up = glm::cross(m_cameraForward, eye_right);
  if (eye_up[2] < 0)
  {
    eye_up *= -1;
  }
  m_cameraUp = glm::normalize(eye_up);
}

template <color_strategy T, is_resolution U>
[[maybe_unused]] void Renderer<T, U>::SetFOV(const float fov) noexcept
{
  m_vertical_fov = fov;
  m_horizontal_fov = glm::degrees(
      2 * std::atan(std::tan(glm::radians(m_vertical_fov) / 2) * (m_resolution->aspectRatio())));
}

template <color_strategy T, is_resolution U>
void Renderer<T, U>::SetSampling(int samples)
{
  m_sampling = samples;
}

template <color_strategy T, is_resolution U>
void Renderer<T, U>::SetRenderQuality(int rays_per_pixel)
{
  m_rays_per_pixel = rays_per_pixel;
}

template <color_strategy T, is_resolution U>
[[maybe_unused]] void Renderer<T, U>::SetDisplayOnScreen(bool value)
{
  m_display_on_screen = value;
}

template <color_strategy T, is_resolution U>
[[maybe_unused]] void Renderer<T, U>::SetWriteToFile(bool value)
{
  m_write_to_file = value;
}

template <color_strategy T, is_resolution U>
void Renderer<T, U>::SetAA(bool value)
{
  m_anti_aliasing = value;
}

template <color_strategy T, is_resolution U>
void Renderer<T, U>::SetAASigma(const float value)
{
  m_sigma = value;
}

template <color_strategy T, is_resolution U>
bool Renderer<T, U>::initializeSDL()
{
  std::cout << "\nInitializing SDL\n";
  if (m_window != nullptr || m_window_renderer != nullptr)
  {
    return false;
  }
  m_window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>>(SDL_CreateWindow(
                                                                                "Computer Graphics",
                                                                                SDL_WINDOWPOS_CENTERED,
                                                                                SDL_WINDOWPOS_CENTERED,
                                                                                m_resolution->width(),
                                                                                m_resolution->height(),
                                                                                0),
                                                                            SDL_DestroyWindow);
  m_window_renderer =
      std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>(SDL_CreateRenderer(m_window.get(),
                                                                                            -1,
                                                                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
                                                                         SDL_DestroyRenderer);

  if (!m_window)
  {
    std::cerr << "Failed to create window\n";
    std::cerr << "Error - " << SDL_GetError() << "\n";
    return false;
  }

  if (!m_window_renderer)
  {
    std::cerr << "Failed to get window's surface\n";
    std::cerr << "Error - " << SDL_GetError() << "\n";
    return false;
  }
  return true;
}

#endif //PATHTRACER_RENDERER_RENDERER_H_
