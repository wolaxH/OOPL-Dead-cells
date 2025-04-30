#ifndef PLAYERUI_HPP
#define PLAYERUI_HPP

#include "Util/GameObject.hpp"

#include "UI/Hp.hpp"
#include "UI/SkillSlot.hpp"

class PlayerUI : public Util::GameObject{
public:
    PlayerUI(){
        m_Hp = std::make_shared<Hp>();

        m_WaterSlot = std::make_shared<SkillSlot>();
        m_WaterSlot->SetPos(glm::vec2(-575, -260));

        m_SkillSlot1 = std::make_shared<SkillSlot>();
        m_SkillSlot1->SetPos(glm::vec2(-480, -260));

        m_SkillSlot2 = std::make_shared<SkillSlot>();
        m_SkillSlot2->SetPos(glm::vec2(-385, -260));

        AddChild(m_Hp);
        AddChild(m_WaterSlot);
        AddChild(m_SkillSlot1);
        AddChild(m_SkillSlot2);
    }
    ~PlayerUI() = default;


    void SetHp(int newhp){
        m_Hp->SetHp(newhp);
    }

    int GetCurrentHp() const {return m_Hp->GetCurrentHp();}

private:
    std::shared_ptr<Hp> m_Hp;
    std::shared_ptr<SkillSlot> m_WaterSlot; 
    std::shared_ptr<SkillSlot> m_SkillSlot1;
    std::shared_ptr<SkillSlot> m_SkillSlot2;
};

#endif