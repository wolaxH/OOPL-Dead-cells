#include "UI/BossHp.hpp"

BossHp::BossHp(int Hp) : m_Hp(Hp), m_MaxHp(Hp){
    auto Img = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/life.png");
    m_HpBar = std::make_shared<Util::GameObject>(Img, 30.2f);
    m_HpBar->m_Transform.scale = glm::vec2(300.0f, 4.0f);
    m_HpBar->m_Transform.translation = glm::vec2(0.f, 290.0f);

    Img = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/lifeBg.png");
    m_HpBg = std::make_shared<Util::GameObject>(Img, 30.1f);
    m_HpBg->m_Transform.scale = glm::vec2(30.0f, 3.0f);
    m_HpBg->m_Transform.translation = glm::vec2(0.f, 290.0f);

    AddChild(m_HpBar);
    AddChild(m_HpBg);
}


void BossHp::Update(int Hp){
    if (Hp != m_Hp) m_Hp = Hp;

    m_HpBar->m_Transform.scale.x = m_Hp/m_MaxHp;
    m_HpBar->m_Transform.translation.x = -(float)(1 - (float)m_Hp/ (float)m_MaxHp) * 300.f * 0.5;
}