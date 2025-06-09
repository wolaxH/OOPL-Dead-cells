#ifndef ISHOOTABLE_HPP
#define ISHOOTABLE_HPP

#include "pch.hpp"

class ISootable{
public:
    virtual void Shoot(const glm::vec2& Dir, const glm::vec2& Pos) = 0;
};

#endif