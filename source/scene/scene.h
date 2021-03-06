//
// Created by kevin on 29/09/2021.
//

#ifndef PATHTRACER_SCENE_SCENE_H_
#define PATHTRACER_SCENE_SCENE_H_

#include <initializer_list>
#include <vector>
#include "../geometry/sphere.h"
#include "../geometry/textured_sphere.h"

class Scene
{
public:
  Scene() = default;
  Scene(std::initializer_list<TexturedSphere> elements);
  std::vector<TexturedSphere> elements();
  TexturedSphere &GetObject(int index);

private:
  std::vector<TexturedSphere> m_elements;
};

#endif //PATHTRACER_SCENE_SCENE_H_
