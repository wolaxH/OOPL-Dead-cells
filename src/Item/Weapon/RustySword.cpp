#include "Item/Weapon/RustySword.hpp"

RustySword::RustySword() : Weapon( RESOURCE_DIR"/Item/RustySword/Icon.png", 20, 50.0f, "Knife"){
    m_AtkPoint = 20;
    m_AtkRange = 70.0f; //temp
}

void RustySword::Use(const glm::vec2& Pos, const glm::vec2& Dir){
    
}