#include "Camera.hpp"

void Camera::Update(std::shared_ptr<Player> player){
    m_Pos = player->m_WorldPos;
}