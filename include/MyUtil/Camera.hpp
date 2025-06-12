#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "pch.hpp"

#include "MyUtil/Timer.hpp"

class Player;

class Camera {
public:    
    Camera() : m_Pos({0, 0}), m_ShakeTimer(Timer()), m_ShakeStrength(0.0f) {}
    ~Camera() = default;

    void ResetPos() { m_Pos = {0, 0}; }

    void SetPos(glm::vec2 pos) { m_Pos = pos; }

    glm::vec2 GetPos() const { return m_ShakeOffset + m_Pos; }

    void Update(std::shared_ptr<Player> player);

    void TriggerShake(float duration, float strength);

private:
    glm::vec2 m_Pos;
    glm::vec2 m_ShakeOffset;

    Timer m_ShakeTimer;       // 剩餘震動時間
    float m_ShakeStrength;   // 震動強度

    void UpdateShake();
};

// class Camera {
// public:    
//     Camera() : m_Pos({0, 0}){}
//     ~Camera() = default;

//     void ResetPos(){m_Pos = {0, 0};}

//     void SetPos(glm::vec2 pos){ m_Pos = pos;}

//     glm::vec2 GetPos(){ return m_Pos;}

//     void Update(std::shared_ptr<Player> player){
//         m_Pos.x = player->m_WorldPos.x;
//         m_Pos.y = player->m_WorldPos.y + 100;
//     }

// private:
//     glm::vec2 m_Pos;
// };
#endif
