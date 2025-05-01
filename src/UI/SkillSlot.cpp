#include "UI/SkillSlot.hpp"

SkillSlot::SkillSlot(){
    std::shared_ptr<Util::Image> resource = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/skillBg.png");
    m_SkillBg = std::make_shared<Util::GameObject>(resource, 60.0f);
    m_SkillBg->m_Transform.scale = glm::vec2(3.75f, 3.75f);

    resource = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/skillSlotBg.png");
    m_SkillSlotBg = std::make_shared<Util::GameObject>(resource, 60.1f);

    m_SkillIcon = nullptr;
    m_SkillIconBg = nullptr;

    m_SkillBg->m_Transform.scale *= 2.5f;
    m_SkillSlotBg->m_Transform.scale *= 2.5f;

    AddChild(m_SkillBg);    //60.0f
    AddChild(m_SkillSlotBg);//60.1f
    AddChild(m_SkillIconBg);//60.2f
    AddChild(m_SkillIcon);  //60.3f
}

void SkillSlot::SetPos(const glm::vec2& pos){
    m_SkillBg->m_Transform.translation = pos;
    m_SkillSlotBg->m_Transform.translation = pos;
}

void SkillSlot::SetSkillIcon(const std::shared_ptr<Util::Image>& Icon){
    // slotBg
    m_SkillIcon = std::make_shared<Util::GameObject>(Icon, 60.3f);
}