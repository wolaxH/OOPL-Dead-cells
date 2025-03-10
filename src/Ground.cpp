#include "Ground.hpp"


Ground::Ground(const std::string& path){
    m_Drawable = std::make_shared<Util::Image>(path);
    m_ZIndex = 10.0f;
    m_Transform.scale = {0.5f, 0.2f};
    m_Transform.translation = {0.0f, -260.0f};
}

