//
// Created by kevin on 23/10/2021.
//

#ifndef PATH_TRACER_BUFFER_BUFFER_H_
#define PATH_TRACER_BUFFER_BUFFER_H_

#include <vec3.hpp>
#include "../constants/resolutions.h"

template<is_resolution U>
class Buffer {
 public:
  explicit Buffer(std::shared_ptr<U>);
  [[nodiscard]] glm::vec3 &GetPixelColor(int, int);
  void SetPixelColor(int, int, glm::vec3 &);
 private:
  std::shared_ptr<U> m_resolution;
  std::vector<glm::vec3> pixel_buffer;
};

template<is_resolution U>
Buffer<U>::Buffer(std::shared_ptr<U> resolution) : m_resolution(resolution) {
  pixel_buffer.resize(static_cast<int>(m_resolution->height() * m_resolution->width()));
}

template<is_resolution U>
glm::vec3 &Buffer<U>::GetPixelColor(int x, int y) {
  return pixel_buffer[y * m_resolution->width() + x];
}

template<is_resolution U>
void Buffer<U>::SetPixelColor(int x, int y, glm::vec3 &value) {
  pixel_buffer[y * m_resolution->width() + x] = value;
}

#endif //PATH_TRACER_BUFFER_BUFFER_H_
