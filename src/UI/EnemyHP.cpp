#include "UI/EnemyHP.hpp"

EnemyHP::EnemyHP(int Hp, int MaxHp, const glm::vec2& enemyPos)
     : m_HeartPoint(Hp), m_EnemyPos(enemyPos), m_MaxHp(MaxHp){
    auto Img = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/life.png");

    m_Hp = std::make_shared<MapObj>();
    m_Hp->SetDrawable(Img);
    m_Hp->m_Transform.scale = glm::vec2(46.f, .8f);
    m_Hp->SetZIndex(30.2f);

    Img = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/lifeBg.png");
    m_HpBG = std::make_shared<MapObj>();
    m_HpBG->SetDrawable(Img);
    m_HpBG->m_Transform.scale = glm::vec2(4.5, .8f);
    m_HpBG->SetZIndex(30.1);
    AddChild(m_Hp);
    AddChild(m_HpBG);
}

void EnemyHP::Sync(){
    m_Hp->m_WorldPos = m_WorldPos;
    m_Hp->m_Transform.scale.x = ((float)m_HeartPoint / (float)m_MaxHp) * 46.0f;
    m_Hp->m_WorldPos.x = m_EnemyPos.x - float(1 - (float)m_HeartPoint/ (float)m_MaxHp) * 46.0f * 0.5f;

    m_HpBG->m_WorldPos = m_WorldPos;
}

void EnemyHP::Update(int currentHp){
    if (currentHp != m_HeartPoint) m_HeartPoint = currentHp;
    m_WorldPos = m_EnemyPos + glm::vec2(0, 50);
    Sync();
}