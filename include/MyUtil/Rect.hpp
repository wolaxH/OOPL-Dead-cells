#ifndef RECT_HPP
#define RECT_HPP

#include "glm/vec2.hpp"

struct Rect
{
    float x, y;
    float width, height;

    bool IsContains(glm::vec2 Pos){
        return Pos.x >= x - width/2  && Pos.x <= x + width/2 &&
               Pos.t >= y - height/2 && Pos.y <= y + height/2;
    }

};


#endif