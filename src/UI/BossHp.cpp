#include "UI/BossHp.hpp"

BossHp::BossHp(int Hp) : m_Hp(Hp), m_MaxHp(Hp){
    auto Img = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/life.png");
    m_HpBar = std::make_shared<Util::GameObject>(Img, 30.2f);
    m_HpBar->m_Transform.scale = glm::vec2(430.0f, 2.0f);
    m_HpBar->m_Transform.translation = glm::vec2(0.f, 290.0f);
    m_HpBar->SetVisible(false);

    Img = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/lifeBg.png");
    m_HpBg = std::make_shared<Util::GameObject>(Img, 30.1f);
    m_HpBg->m_Transform.scale = glm::vec2(40.0f, 1.5f);
    m_HpBg->m_Transform.translation = glm::vec2(0.f, 290.0f);
    m_HpBg->SetVisible(false);

    AddChild(m_HpBar);
    AddChild(m_HpBg);
}


void BossHp::Update(int Hp){
    if (Hp != m_Hp) m_Hp = Hp;

    m_HpBar->m_Transform.scale.x = (float)m_Hp / (float)m_MaxHp * 430.f;
    m_HpBar->m_Transform.translation.x = -(float)(1 - (float)m_Hp/ (float)m_MaxHp) * 430.f * 0.5;
}