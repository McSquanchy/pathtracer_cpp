//
// Created by kevin on 23/10/2021.
//

#ifndef PATHTRACER_BUFFER_BUFFER_H_
#define PATHTRACER_BUFFER_BUFFER_H_

#include <vec3.hpp>
#include <fstream>
#include "../constants/resolutions.h"

template <is_resolution U>
class Buffer
{
public:
  explicit Buffer(std::shared_ptr<U>);
  [[nodiscard]] glm::vec3 &GetPixelColor(int, int);
  void SetPixelColor(int, int, glm::vec3 &);
  void WriteToFile(const char *fp) const;

private:
  std::shared_ptr<U> m_resolution;
  std::vector<glm::vec3> pixel_buffer;
};

template <is_resolution U>
Buffer<U>::Buffer(std::shared_ptr<U> resolution) : m_resolution(resolution)
{
  pixel_buffer.resize(static_cast<int>(m_resolution->height() * m_resolution->width()));
}

template <is_resolution U>
glm::vec3 &Buffer<U>::GetPixelColor(int x, int y)
{
  return pixel_buffer[y * m_resolution->width() + x];
}

template <is_resolution U>
void Buffer<U>::SetPixelColor(int x, int y, glm::vec3 &value)
{
  pixel_buffer[y * m_resolution->width() + x] = value;
}

template <is_resolution U>
void Buffer<U>::WriteToFile(const char *fp) const
{
  std::ofstream file;
  file.open(fp, std::ios::out | std::ios::binary);

  if (!file.is_open())
  {
    std::cerr << "Couldn't open target file " << fp << "\n";
    return;
  }

  unsigned char bmp_padding[3]{0, 0, 0};
  const int padding_amt = ((4 - (m_resolution->width() * 3) % 4) % 4);

  const int fileHeaderSize = 14;
  const int informationHeaderSize = 40;
  const int fileSize = fileHeaderSize + informationHeaderSize + m_resolution->width() * m_resolution->height() * 3 + padding_amt * m_resolution->height();

  unsigned char fileHeader[fileHeaderSize];

  // File type
  fileHeader[0] = 'B';
  fileHeader[1] = 'M';

  // File size
  fileHeader[2] = fileSize;
  fileHeader[3] = fileSize >> 8;
  fileHeader[4] = fileSize >> 16;
  fileHeader[5] = fileSize >> 24;

  //Reserved 1 (not used)
  fileHeader[6] = 0;
  fileHeader[7] = 0;
  // Reserved 2 (not used)
  fileHeader[8] = 0;
  fileHeader[9] = 0;
  // Pixel data offset
  fileHeader[10] = fileHeaderSize + informationHeaderSize;
  fileHeader[11] = 0;
  fileHeader[12] = 0;
  fileHeader[13] = 0;

  unsigned char informationHeader[informationHeaderSize];

  informationHeader[0] = informationHeaderSize;
  informationHeader[1] = 0;
  informationHeader[2] = 0;
  informationHeader[3] = 0;

  informationHeader[4] = m_resolution->width();
  informationHeader[5] = m_resolution->width() >> 8;
  informationHeader[6] = m_resolution->width() >> 16;
  informationHeader[7] = m_resolution->width() >> 24;

  informationHeader[8] = -m_resolution->height();
  informationHeader[9] = -m_resolution->height() >> 8;
  informationHeader[10] = -m_resolution->height() >> 16;
  informationHeader[11] = -m_resolution->height() >> 24;

  informationHeader[12] = 1;
  informationHeader[13] = 0;

  informationHeader[14] = 24;
  informationHeader[15] = 0;

  informationHeader[16] = 0;
  informationHeader[17] = 0;
  informationHeader[18] = 0;
  informationHeader[19] = 0;

  informationHeader[20] = 0;
  informationHeader[21] = 0;
  informationHeader[22] = 0;
  informationHeader[23] = 0;

  informationHeader[24] = 0;
  informationHeader[25] = 0;
  informationHeader[26] = 0;
  informationHeader[27] = 0;

  informationHeader[28] = 0;
  informationHeader[29] = 0;
  informationHeader[30] = 0;
  informationHeader[31] = 0;

  informationHeader[32] = 0;
  informationHeader[33] = 0;
  informationHeader[34] = 0;
  informationHeader[35] = 0;

  informationHeader[36] = 0;
  informationHeader[37] = 0;
  informationHeader[38] = 0;
  informationHeader[39] = 0;

  file.write(reinterpret_cast<char *>(fileHeader), fileHeaderSize);
  file.write(reinterpret_cast<char *>(informationHeader), informationHeaderSize);

  for (auto y = 0; y < m_resolution->height(); ++y)
  {
    for (auto x = 0; x < m_resolution->width(); ++x)
    {
      auto pixelColor = pixel_buffer[y * m_resolution->width() + x];
      auto r = static_cast<unsigned char>(pixelColor.x);
      auto g = static_cast<unsigned char>(pixelColor.y);
      auto b = static_cast<unsigned char>(pixelColor.z);
      unsigned char color[]{b, g, r};
      file.write(reinterpret_cast<char *>(color), 3);
    }
    file.write(reinterpret_cast<char *>(bmp_padding), padding_amt);
  }

  file.close();
  std::cout << "Written image to file " << fp << "\n";
}

#endif //PATHTRACER_BUFFER_BUFFER_H_
