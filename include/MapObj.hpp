#ifndef MAPOBJ_HPP
#define MAPOBJ_HPP

#include "Util/GameObject.hpp"

class MapObj : public Util::GameObject{
public:
    glm::vec2 m_WorldPos;

    //bool GetVisible() const { return m_Visible; }

    /*此段變數是因為不同素材來源其中心點不同，因此Iscollsion() method 的碰撞距離需要根據不同素材大小作修改*/
    int top, bottom, left, right;
};

#endif