//
// Created by kevin on 29/09/2021.
//

#include "SDL2/SDL.h"
#include "scene/scene.h"
#include "renderer/basic_renderer.h"
#include "renderer/monte_carlo_renderer.h"
#include "renderer/gradient_renderer.h"
#include "color/colormode.h"
#include "constants/resolutions.h"
#include "renderer/bitmap_renderer.h"
#include "textures/bmp/BitmapTexture.h"
#include "geometry/sphere.h"
#include "constants/textures.h"

int WinMain([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  glm::vec3 eye{0, 0, -4};
  glm::vec3 lookAt{0, 0, 6};
  float v_fov = 36;

  BitmapRenderer<sRGB, CustomResolution> renderer({480, 480});

  renderer.setSampling(1);
  renderer.setRenderQuality(32);
  renderer.setCamera(eye, lookAt);
  renderer.setFov(v_fov);

  Scene default_scene{
      TexturedSphere{{0, 0, 0}, 0, Colors::BLACK() /* null_sphere */ },
      TexturedSphere{{-1001, 0, 0}, 1000, Colors::RED()},
      TexturedSphere{{1001, 0, 0}, 1000, Colors::BLUE()},
      TexturedSphere{{0, 0, 1001}, 1000, Colors::GRAY()},
      TexturedSphere{{0, 1001, 0}, 1000, Colors::GRAY()},
      TexturedSphere{{0, -1001, 0}, 1000, Colors::WHITE(), Colors::WHITE()},
      TexturedSphere{{-0.6, 0.7, -0.6}, 0.3, Colors::YELLOW(), Colors::BLACK(), Colors::WHITE()},
      TexturedSphere{{0.3, 0.4, 0.3}, 0.6, Colors::CYAN(), Colors::BLACK(), Colors::WHITE()},
  };

//  Scene default_scene{
//      TexturedSphere{{0, 0, 0}, 0, Colors::BLACK() /* null_sphere */ },
//      TexturedSphere{{-1001, 0, 0}, 1000, Colors::RED()},
//      TexturedSphere{{1001, 0, 0}, 1000, Colors::BLUE()},
//      TexturedSphere{{0, 0, 1001}, 1000, Colors::GRAY()},
//      TexturedSphere{{0, 1001, 0}, 1000, Colors::GRAY()},
//      TexturedSphere{{0, -1001, 0}, 1000, Colors::WHITE(), Colors::WHITE()},
//      TexturedSphere{{-0.6, 0.7, -0.6}, 0.3, Colors::BLACK(), Colors::BLACK(), Colors::BLACK(),
//                     Textures::BRICK(), TextureType::DIFFUSE},
//      TexturedSphere{{0.3, 0.4, 0.3}, 0.6, Colors::BLACK(), Colors::BLACK(), Colors::BLACK(), Textures::LAVA_DIFFUSE(),
//                     TextureType::DIFFUSE},
//  };

//  default_scene.GetObject(7).addTexture(Textures::BRATAN(), TextureType::EMISSIVE);
//  Scene custom_scene_one{
//      TexturedSphere{{-1001, 0, 0}, 1000, Colors::MOUNTAIN_MEADOW()},
//      TexturedSphere{{1001, 0, 0}, 1000, Colors::MOUNTAIN_MEADOW()},
//      TexturedSphere{{0, 0, 1001}, 1000, Colors::BONE()},
//      TexturedSphere{{0, 1001, 0}, 1000, Colors::BONE()},
//      TexturedSphere{{0, -1001, 0}, 1000, Colors::BONE(), Colors::BONE()},
//      TexturedSphere{{-0.35, 0.7, -0.6}, 0.4, Colors::FIERY_ROSE(), Colors::BLACK(), Colors::BLACK()},
//      TexturedSphere{{0.3, 0.4, 0.8}, 0.3, Colors::YELLOW(), Colors::BLACK(), Colors::BONE()},
//      TexturedSphere{{0, -0.6, 0.7}, 0.25, Colors::GRAY(), Colors::BLACK(), Colors::BLACK()},
//  };

  renderer.setScene(default_scene);

  renderer.run();

  return 0;
}