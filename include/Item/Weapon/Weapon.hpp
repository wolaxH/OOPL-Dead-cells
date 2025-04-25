#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Item/Item.hpp"

#include "Util/Animation.hpp"


class Weapon : public Item{
public:
    Weapon(std::string& path, std::vector<std::string> PlayerAtkPaths, 
        int AtkPoint, float AtkRange, std::string& Describe, std::string& DescribeBoxPath)
        : Item(path), 
        m_AtkPoint(AtkPoint), 
        m_AtkRange(AtkRange){

            m_Describe = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 20, Describe, Util::Color(255, 255, 255));
            m_DescribeBox = std::make_shared<Util::Image>(DescribeBoxPath);

            Player_Drawable = std::make_shared<Util::Animation>(PlayerAtkPaths, false);
    }

    auto GetPlayerDrawable() const {return Player_Drawable;}
private:
    int m_AtkPoint; //攻擊力
    float m_AtkRange; //攻擊範圍

    std::shared_ptr<Util::Animation> Player_Drawable; //玩家的攻擊動畫
};

#endif