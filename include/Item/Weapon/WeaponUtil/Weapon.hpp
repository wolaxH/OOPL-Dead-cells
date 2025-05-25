#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <map>

#include "Item/PickUp.hpp"
#include "MyUtil/Rect.hpp"

#include "Util/Animation.hpp"

class Mob;

class Weapon : public Item{
public:
    Weapon(const std::string& path, const std::string& Describe)
        : Item(path){
            auto T = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 20, Describe, Util::Color(255, 255, 255));
            m_Describe = std::make_shared<Util::GameObject>(T, 15.0f);
            m_Describe->SetVisible(false);
    }
    virtual ~Weapon() noexcept = default;

    int GetSegCount() const noexcept { return m_SegCount;}
    
    std::vector<std::shared_ptr<Util::Animation>> GetPlayerDrawable() const noexcept {return m_Player_Drawables;}

    std::vector<size_t> GetAtkableFrames(int combo) const {
        if (combo < m_AtkableFrames.size()) return m_AtkableFrames[combo];
    }

    std::vector<int> GetAtkTimes() const noexcept{return m_AtkTimes;}
    

    virtual void Use(std::shared_ptr<Mob>& mob, const glm::vec2& Dir = {1, 1}, int combo = 0) = 0;

    Rect virtual GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir, int combo = 0) = 0;

protected:
    std::vector<int> m_AtkPoint; //攻擊力
    std::vector<std::shared_ptr<Util::Animation>> m_Player_Drawables; //玩家的攻擊動畫
    std::vector<std::vector<size_t>> m_AtkableFrames;   //玩家攻擊有效的frames
    std::vector<int> m_AtkTimes;
    int m_SegCount;
};

#endif