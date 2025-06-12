#ifndef PLAYERUI_HPP
#define PLAYERUI_HPP

#include "Util/GameObject.hpp"

#include "UI/Hp.hpp"
#include "UI/SkillSlot.hpp"
#include "Item/PickUp.hpp"
#include "Item/HealBottle.hpp"

class PlayerUI : public Util::GameObject{
public:
    PlayerUI(const std::shared_ptr<HealBottle>& healBottle);
    ~PlayerUI() = default;

    void SetHp(int newhp){ m_Hp->SetHp(newhp);}

    void SetSkill(std::shared_ptr<Item> item, int inde = 0);

    int GetCurrentHp() const {return m_Hp->GetCurrentHp();}

    void UpdateWaterSlot(const std::shared_ptr<HealBottle>& healBottle);
private:
    std::shared_ptr<Hp> m_Hp;
    std::shared_ptr<SkillSlot> m_WaterSlot; 
    std::vector<std::shared_ptr<SkillSlot>> m_SkillSlot;    //capacity = 2
};

#endif