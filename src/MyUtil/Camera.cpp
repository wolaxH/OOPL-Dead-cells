#include "MyUtil/Camera.hpp"
#include "Character/Player.hpp"

void Camera::TriggerShake(float duration, float strength) {
    m_ShakeTimer.SetTime(duration * 1000, duration * 1000);
    m_ShakeTimer.ResetTime();
    m_ShakeStrength = strength;
}

void Camera::UpdateShake() {
    if (!m_ShakeTimer.IsTimeout()) {
        
        float dx = ((rand() % 200) / 100.0f - 1.0f) * m_ShakeStrength;
        float dy = ((rand() % 200) / 100.0f - 1.0f) * m_ShakeStrength;
        m_ShakeOffset = { dx, dy };
    } else {
        m_ShakeOffset = { 0.0f, 0.0f };
    }
}

void Camera::Update(std::shared_ptr<Player> player) {
    m_Pos.x = player->m_WorldPos.x;
    m_Pos.y = player->m_WorldPos.y + 100;

    UpdateShake();
}