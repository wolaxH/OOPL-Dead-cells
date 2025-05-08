#ifndef SKILLSLOT_HPP
#define SKILLSLOT_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class SkillSlot : public Util::GameObject{
public:
    SkillSlot();
    ~SkillSlot() = default;

    /**
     * @brief Set the position of the skill slot(Bg and SlotBg).
     */
    void SetPos(const glm::vec2& pos);

    void SetSkillIcon(const std::shared_ptr<Util::Image>& Icon);

private:
    std::shared_ptr<Util::GameObject> m_SkillSlotBg;
    std::shared_ptr<Util::GameObject> m_SkillBg;
    std::shared_ptr<Util::GameObject> m_SkillIconBg;
    std::shared_ptr<Util::GameObject> m_SkillIcon;
};


#endif // SKILLSLOT_HPP