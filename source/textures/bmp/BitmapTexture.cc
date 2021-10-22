//
// Created by kevin on 21/10/2021.
//

#include <fstream>
#include <iostream>
#include <exponential.hpp>
#include "BitmapTexture.h"

void BitmapTexture::Read(const char *fp) {
  m_colors.clear();
  std::ifstream f;
  f.open(fp, std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    std::cerr << "File could not be opened!\n";
    return;
  }

  const int file_header_size = 14;
  const int informationHeaderSize = 40;

  unsigned char fileHeader[file_header_size];
  f.read(reinterpret_cast<char *>(fileHeader), file_header_size);

  if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
    std::cerr << "wrong input format\n";
    f.close();
    return;
  }

  unsigned char informationHeader[informationHeaderSize];
  f.read(reinterpret_cast<char *>(informationHeader), informationHeaderSize);

  m_width =
      informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);

  m_height =
      informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16)
          + (informationHeader[11] << 24);

  m_colors.resize(m_width * m_height);

  const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

  for (auto y = 0; y < m_height; ++y) {
    for (auto x = 0; x < m_width; ++x) {
      unsigned char color[3];
      f.read(reinterpret_cast<char *>(color), 3);
      auto r = static_cast<float>(glm::pow(color[2] / 255.0f, m_gamma));
      auto g = static_cast<float>(glm::pow(color[1] / 255.0f, m_gamma));
      auto b = static_cast<float>(glm::pow(color[0] / 255.0f, m_gamma));
      m_colors[y * m_width + x] = glm::vec3{r, g, b};
    }
    f.ignore(paddingAmount);
  }
  f.close();
}

glm::vec3 &BitmapTexture::GetColor(int x, int y) {
  return m_colors[y * m_width + x];
}
