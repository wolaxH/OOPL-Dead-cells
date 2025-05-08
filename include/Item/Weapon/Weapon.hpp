#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Item/PickUp.hpp"
#include "MyUtil/Rect.hpp"

#include "Util/Animation.hpp"

class Mob;

class Weapon : public Item{
public:
    Weapon(const std::string& path, int AtkPoint, float AtkRange, const std::string& Describe)
        : Item(path), m_AtkPoint(AtkPoint), m_AtkRange(AtkRange){
            auto T = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 20, Describe, Util::Color(255, 255, 255));
            m_Describe = std::make_shared<Util::GameObject>(T, 15.0f);
            m_Describe->SetVisible(false);
    }
    virtual ~Weapon() noexcept = default;

    virtual void Use(std::shared_ptr<Mob>& mob) = 0;
    
    auto GetIcon() const {return m_Drawable;}

    std::vector<std::shared_ptr<Util::Animation>> GetPlayerDrawable() const {return m_Player_Drawables;}

    Rect virtual GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir) = 0;

protected:
    int m_AtkPoint; //攻擊力
    float m_AtkRange; //攻擊範圍


    std::vector<std::shared_ptr<Util::Animation>> m_Player_Drawables; //玩家的攻擊動畫
};

#endif