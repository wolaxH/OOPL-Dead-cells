#ifndef SKILLSLOT_HPP
#define SKILLSLOT_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class SkillSlot : public Util::GameObject{
public:
    SkillSlot(){
        std::shared_ptr<Util::Image> resource = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/skillBg.png");
        m_SkillBg = std::make_shared<Util::GameObject>(resource, 60.0f);
        m_SkillBg->m_Transform.scale = glm::vec2(3.75f, 3.75f);


        resource = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/skillSlotBg.png");
        m_SkillSlotBg = std::make_shared<Util::GameObject>(resource, 60.1f);

        m_SkillBg->m_Transform.scale *= 2.5f;
        m_SkillSlotBg->m_Transform.scale *= 2.5f;

        AddChild(m_SkillBg);
        AddChild(m_SkillSlotBg);
    }
    ~SkillSlot() = default;

    void SetPos(const glm::vec2& pos){
        m_SkillBg->m_Transform.translation = pos;
        m_SkillSlotBg->m_Transform.translation = pos;
    }
private:
    std::shared_ptr<Util::GameObject> m_SkillSlotBg;
    std::shared_ptr<Util::GameObject> m_SkillBg;
};


#endif // SKILLSLOT_HPP