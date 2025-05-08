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

    bool Intersects(const Rect& other) const {
        return !(x + width / 2 < other.x - other.width / 2 ||  // this is left of other
                 x - width / 2 > other.x + other.width / 2 ||  // this is right of other
                 y + height / 2 < other.y - other.height / 2 ||// this is above other
                 y - height / 2 > other.y + other.height / 2); // this is below other
    }

};


#endif