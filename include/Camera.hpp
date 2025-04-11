#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "pch.hpp"
#include "Player.hpp"

class Camera {
public:    
    Camera() : m_Pos({0, 0}){}
    ~Camera() = default;

    void ResetPos(){m_Pos = {0, 0};}

    void SetPos(glm::vec2 pos){ m_Pos = pos;}

    glm::vec2 GetPos(){ return m_Pos;}

    void Update(std::shared_ptr<Player> player){
        m_Pos.x = player->m_WorldPos.x;
        m_Pos.y = player->m_WorldPos.y + 100;
    }

private:
    glm::vec2 m_Pos;
};
#endif
