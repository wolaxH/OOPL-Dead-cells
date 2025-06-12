#include "UI/PlayerUI.hpp"

#include "Item/HealBottle.hpp"
PlayerUI::PlayerUI(const std::shared_ptr<HealBottle>& healBottle){
    m_Hp = std::make_shared<Hp>();


    m_WaterSlot = std::make_shared<SkillSlot>();
    m_WaterSlot->SetPos(glm::vec2(-575, -260));
    auto Icon = std::dynamic_pointer_cast<Util::Image>(healBottle->GetIcon());
    m_WaterSlot->SetSkillIcon(Icon);

    m_SkillSlot.reserve(2);
    for (size_t i = 0; i < m_SkillSlot.capacity(); i++){
        m_SkillSlot.push_back(std::make_shared<SkillSlot>());
    }
    m_SkillSlot[0]->SetPos(glm::vec2(-480, -260));
    m_SkillSlot[1]->SetPos(glm::vec2(-385, -260));

    AddChild(m_Hp);
    AddChild(m_WaterSlot);
    AddChild(m_SkillSlot[0]);
    AddChild(m_SkillSlot[1]);
}

void PlayerUI::UpdateWaterSlot(const std::shared_ptr<HealBottle>& healBottle){
    auto Icon = std::dynamic_pointer_cast<Util::Image>(healBottle->GetIcon());
    m_WaterSlot->SetSkillIcon(Icon);
}

void PlayerUI::SetSkill(std::shared_ptr<Item> item, int index){
    auto Icon = std::dynamic_pointer_cast<Util::Image>(item->GetIcon());
    m_SkillSlot[index]->SetSkillIcon(Icon);
}

