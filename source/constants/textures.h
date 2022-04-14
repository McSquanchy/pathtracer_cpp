//
// Created by kevin on 22/10/2021.
//

#ifndef PATHTRACER_CONSTANTS_TEXTURES_H_
#define PATHTRACER_CONSTANTS_TEXTURES_H_

namespace Textures
{
  namespace
  {
    [[maybe_unused]] inline static std::string BRICK()
    {
      return {"resources/textures/tx_brick_wall.bmp"};
    }

    [[maybe_unused]] inline static std::string BRICK2()
    {
      return {"resources/textures/tx_brick_wall_2.bmp"};
    }
    [[maybe_unused]] inline static std::string CONCRETE()
    {
      return {"resources/textures/tx_concrete.bmp"};
    }
    [[maybe_unused]] inline static std::string LAVA()
    {
      return {"resources/textures/tx_lava_diffuse.bmp"};
    }
    [[maybe_unused]] inline static std::string WOOD()
    {
      return {"resources/textures/tx_wood.bmp"};
    }
    [[maybe_unused]] inline static std::string SUN()
    {
      return {"resources/textures/tx_sun.bmp"};
    }
    [[maybe_unused]] inline static std::string MARS()
    {
      return {"resources/textures/tx_mars.bmp"};
    }
    [[maybe_unused]] inline static std::string GLOW()
    {
      return {"resources/textures/tx_glow.bmp"};
    }

    [[maybe_unused]] inline static std::string BGLOW()
    {
      return {"resources/textures/tx_glow_blue.bmp"};
    }
    [[maybe_unused]] inline static std::string GGLOW()
    {
      return {"resources/textures/tx_glow_green.bmp"};
    }
  }
}

#endif //PATHTRACER_CONSTANTS_TEXTURES_H_
