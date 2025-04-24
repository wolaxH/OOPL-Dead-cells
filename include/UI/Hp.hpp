#ifndef HP_HPP
#define HP_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Util/Renderer.hpp"

class Hp{
public:
    Hp(){
        auto temp = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/HpBox.png");
        HpBox = std::make_shared<Util::GameObject>(temp, 50.0);
        HpBox->m_Transform.translation = {-380, -330};
        temp = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/HpBar.png");
        HpBar = std::make_shared<Util::GameObject>(temp, 51.0);
        HpBar->m_Transform.translation = {-380, -330};
        HpBar->m_Transform.scale.x = 1.01f;
        text = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 20, "100/100", Util::Color(255, 255, 255));
        Point = std::make_shared<Util::GameObject>(text, 51.1);
        Point->m_Transform.translation = {-380, -330};
    }
    ~Hp() = default;

    void SetHp(int hp){
        if (hp > MaxHp) CurrentHp = MaxHp;
        else if (hp < 0) CurrentHp = 0;
        else CurrentHp = hp;

        /**
         * TODO:
         *  change the scale.x of HpBar
         */
        HpBar->m_Transform.scale.x = hp/100.0f;
        HpBar->m_Transform.translation.x = -380.0f - (1 - hp/100.0f) * 464.6f * 0.5f ;
        
        text->SetText(std::to_string(CurrentHp) + "/" + std::to_string(MaxHp));
    }

    void AddToRender(Util::Renderer& root){
        root.AddChild(HpBox);
        root.AddChild(HpBar);
        root.AddChild(Point);
    }

private:
    std::shared_ptr<Util::GameObject> HpBox;
    std::shared_ptr<Util::GameObject> HpBar;    // the green bar
    std::shared_ptr<Util::GameObject> Point;   //text eg: 100/170
    std::shared_ptr<Util::Text> text;
    int MaxHp = 100, CurrentHp = 100;
};


#endif