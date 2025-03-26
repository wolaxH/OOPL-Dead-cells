#include "SolidObj.hpp"


SolidObj::SolidObj(const std::string& path){
    m_Drawable = std::make_shared<Util::Image>(path);
    m_ZIndex = 10.0f;
    m_Transform.scale = {1.0f, 0.2f};
    m_Transform.translation = {0.0f, -260.0f};
    m_WorldPos = m_Transform.translation;
    top = m_Drawable->GetSize().y / 2;
    bottom = top;
    left = m_Drawable->GetSize().x /2;
    right = left;
}

