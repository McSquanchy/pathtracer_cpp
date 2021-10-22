//
// Created by kevin on 27/09/2021.
//
#include <ostream>

#ifndef RAYTRACER_RENDERER_RESOLUTIONS_H_
#define RAYTRACER_RENDERER_RESOLUTIONS_H_

class Resolution {
 protected:
  const std::pair<int, int> m_resolution;
  explicit Resolution(std::pair<int, int> t) : m_resolution(std::move(t)) {}

 public:
  virtual ~Resolution() = default;
  virtual constexpr float width() = 0;
  virtual constexpr float height() = 0;
  constexpr float aspectRatio() {
    return static_cast<float>(width()) / static_cast<float>(height());
  }
  friend std::ostream &operator<<(std::ostream &stream, const Resolution &res) {
    stream << "Width: " << res.m_resolution.first << " / Height: " << res.m_resolution.second;
    return stream;
  }
};

class [[maybe_unused]] SD : public Resolution {
 public:
  SD() : Resolution({640, 480}) {}
  constexpr float width() override { return static_cast<float>(m_resolution.first); }
  constexpr float height() override { return static_cast<float>(m_resolution.second); }
};

class [[maybe_unused]] HD : public Resolution {
 public:
  HD() : Resolution({1280, 720}) {}
  constexpr float width() override { return static_cast<float>(m_resolution.first); }
  constexpr float height() override { return static_cast<float>(m_resolution.second); }
};

class [[maybe_unused]] FullHD : public Resolution {
 public:
  FullHD() : Resolution({1920, 1080}) {}
  constexpr float width() override { return static_cast<float>(m_resolution.first); }
  constexpr float height() override { return static_cast<float>(m_resolution.second); }
};

class [[maybe_unused]] QHD : public Resolution {
 public:
  QHD() : Resolution({2560, 1440}) {}
  constexpr float width() override { return static_cast<float>(m_resolution.first); }
  constexpr float height() override { return static_cast<float>(m_resolution.second); }
};

class [[maybe_unused]] CustomResolution : public Resolution {
 public:
  explicit CustomResolution(std::pair<int, int> resolution) : Resolution(resolution) {}
  constexpr float width() override { return static_cast<float>(m_resolution.first); }
  constexpr float height() override { return static_cast<float>(m_resolution.second); }
};

template<typename T>
concept is_resolution = std::derived_from<T, Resolution>;

#endif //RAYTRACER_RENDERER_RESOLUTIONS_H_