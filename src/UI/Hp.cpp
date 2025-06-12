#include "UI/Hp.hpp"


Hp::Hp(){
    auto temp = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/HpBox.png");
    HpBox = std::make_shared<Util::GameObject>(temp, 60.0);
    HpBox->m_Transform.translation = {-380, -330};

    temp = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/HpBar.png");
    HpBar = std::make_shared<Util::GameObject>(temp, 60.1);
    HpBar->m_Transform.translation = {-380, -330};
    HpBar->m_Transform.scale.x = 1.01f;

    text = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 20, "200/200", Util::Color(255, 255, 255));
    Point = std::make_shared<Util::GameObject>(text, 60.2);
    Point->m_Transform.translation = {-380, -330};
    
    AddChild(HpBox);
    AddChild(HpBar);
    AddChild(Point);
}

void Hp::SetHp(int hp){
    if (hp > MaxHp) CurrentHp = MaxHp;
    else if (hp < 0) CurrentHp = 0;
    else CurrentHp = hp;

    HpBar->m_Transform.scale.x = hp/200.0f;
    HpBar->m_Transform.translation.x = -380.0f - (1 - hp/200.0f) * 464.6f * 0.5f;
    
    text->SetText(std::to_string(CurrentHp) + "/" + std::to_string(MaxHp));
}