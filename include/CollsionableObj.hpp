#ifndef COLLSIONABLEOBJ_HPP
#define COLLSIONABLEOBJ_HPP

#include "Util/GameObject.hpp"

class CollsionableObj : public Util::GameObject{
public:
    glm::vec2 m_WorldPos;
};

#endif