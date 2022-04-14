//
// Created by kevin on 29/09/2021.
//

#include "scene/scene.h"
#include "renderer/monte_carlo_renderer.h"
#include "color/colormode.h"
#include "constants/resolutions.h"

int WinMain([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
// default camera (also for scene 3)
  glm::vec3 eye{0, 0, -4};
  glm::vec3 lookAt{0, 0, 6};
//
//  custom_one_camera
//  glm::vec3 eye{3, 0, -4};
//  glm::vec3 lookAt{-1, 0, 6};
//
// custom_two_camera
//  glm::vec3 eye{-3, 0.5, -6};
//  glm::vec3 lookAt{2.4, -0.5, 6};

  MonteCarloRenderer<sRGB, CustomResolution> renderer({480, 480});

  renderer.SetSampling(4);
  renderer.SetRenderQuality(8);
  renderer.SetCamera(eye, lookAt);
  renderer.SetFOV(36);
  renderer.SetWriteToFile(false);
  renderer.SetAASigma(0.5f);
  renderer.SetAA(true);

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

//  Scene custom_scene_one{
//      TexturedSphere{{0, 0, 0}, 0, Colors::BLACK() /* null_sphere */ },
//      TexturedSphere{{0, 1001, 0}, 1000, Colors::GRAY(), Colors::BLACK(), Colors::BLACK(), Textures::MARS(),
//                     TextureType::DIFFUSE},
//      TexturedSphere{{0, 0.2, 0}, 0.35, Colors::BLACK(), Colors::BLACK(), Colors::BLACK(), Textures::BRICK(),
//                     TextureType::DIFFUSE},
//      TexturedSphere{{0, -10, 14}, 8, Colors::BLACK(), Colors::BLACK(), Colors::BLACK(), Textures::SUN(),
//                     TextureType::EMISSIVE},
//  };
//
//  Scene custom_scene_two{
//      TexturedSphere{{0, 0, 0}, 0, Colors::BLACK() /* null_sphere */ },
//      TexturedSphere{{0, 1001, 0}, 1000, Colors::WHITE(), Colors::BLACK(), Colors::BLACK()},
//      TexturedSphere{{1, 0.2, 0}, 0.35, Colors::BLACK(), Colors::BLACK(), Colors::BLACK(), Textures::LAVA(),
//                     TextureType::EMISSIVE},
//      TexturedSphere{{0, -10, 14}, 8, Colors::BLACK(), Colors::BLACK(), Colors::BLACK(), Textures::BGLOW(),
//                     TextureType::EMISSIVE},
//      TexturedSphere{{-1.5, -0.7, 8}, 0.3, Colors::BLACK(), Colors::BLACK(), Colors::BLACK(), Textures::GGLOW(),
//                     TextureType::DIFFUSE},
//      TexturedSphere{{1, -0.2, 12}, 0.25, Colors::BLACK(), Colors::BLACK(), Colors::BLACK(), Textures::GLOW(),
//                     TextureType::EMISSIVE},
//  };

//  Scene custom_scene_three{
//      TexturedSphere{{0, 0, 0}, 0, Colors::BLACK() /* null_sphere */ },
//      TexturedSphere{{0, 0, 1001}, 1000, Colors::GRAY()},
//      TexturedSphere{{0, 1001, 0}, 1000, Colors::WHITE()},
//      TexturedSphere{{0.0, 0.4, -0.4}, 0.1, Colors::FIERY_ROSE(), Colors::FIERY_ROSE(), Colors::WHITE()},
//      TexturedSphere{{-0.5, 0.75, -0.3}, 0.1, Colors::BONE(), Colors::BONE(), Colors::FIERY_ROSE()},
//      TexturedSphere{{0.5, 0.75, -0.3}, 0.1, Colors::MOUNTAIN_MEADOW(), Colors::MOUNTAIN_MEADOW(), Colors::FIERY_ROSE()},
//  };

  renderer.SetScene(default_scene);
//  renderer.SetScene(custom_scene_one);
//  renderer.SetScene(custom_scene_two);
//  renderer.SetScene(custom_scene_three);

  renderer.Run();
}