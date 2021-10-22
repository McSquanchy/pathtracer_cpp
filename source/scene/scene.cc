//
// Created by kevin on 29/09/2021.
//

#include "scene.h"
Scene::Scene(std::initializer_list<TexturedSphere> elements) : m_elements(elements) {

}
std::vector<TexturedSphere> Scene::elements() {
  return m_elements;
}

TexturedSphere &Scene::GetObject(int index) {
  return m_elements.at(index);
}
