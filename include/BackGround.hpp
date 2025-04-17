#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Abstract/MapObj.hpp"

#include "Util/Image.hpp"

class BG : public MapObj {
public:
    BG(const std::string& path, glm::vec2 pos = {0, 0}, const glm::vec2& scale = {0.8f, 0.8f}){
        m_Drawable = std::make_shared<Util::Image>(path);
        m_ZIndex = 1.0f;
        m_WorldPos = pos;
        m_Transform.scale = scale;
    }
    ~BG() = default;

};

#endif