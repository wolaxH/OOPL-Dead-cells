#ifndef MAPOBJ_HPP
#define MAPOBJ_HPP

#include "Util/GameObject.hpp"

class MapObj : public Util::GameObject{
public:
    glm::vec2 m_WorldPos;
};

#endif