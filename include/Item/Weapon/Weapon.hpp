#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Item/PickUp.hpp"

#include "Util/Animation.hpp"


class Weapon : public Item{
public:
    Weapon(const std::string& path, int AtkPoint, float AtkRange, const std::string& Describe)
        : Item(path), m_AtkPoint(AtkPoint), m_AtkRange(AtkRange){
            auto T = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 20, Describe, Util::Color(255, 255, 255));
            m_Describe = std::make_shared<Util::GameObject>(T, 15.0f);
            m_Describe->SetVisible(false);
    }
    virtual ~Weapon() noexcept = default;

    virtual void Use(const glm::vec2& Pos, const glm::vec2& Dir) = 0;
    
    auto GetIcon() const {return m_Drawable;}

    auto GetPlayerDrawable() const {return Player_Drawable;}
protected:
    int m_AtkPoint; //攻擊力
    float m_AtkRange; //攻擊範圍

    std::shared_ptr<Util::Animation> Player_Drawable; //玩家的攻擊動畫
};

#endif