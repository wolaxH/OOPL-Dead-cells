#include "Portal.hpp"

Portal::Portal(std::shared_ptr<Player> player, glm::vec2 Pos, Type type) : m_player(player), m_Type(type){
    m_WorldPos = Pos;
    m_Text = std::make_shared<MapObj>();
    m_Text->m_WorldPos = m_WorldPos + glm::vec2(0, 100);


    auto text = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 24, "是否前往Boss關卡", Util::Color(255, 255, 255));
    m_Text->SetDrawable(text);
    m_Text->SetVisible(false);
    m_Drawable = std::make_shared<Util::Image>(RESOURCE_DIR"/Portal.png");
    AddChild(m_Text);
}

void Portal::Interact(){
    if (m_Type != Type::exit){
        std::shared_ptr<Player> player = m_player.lock();
        std::shared_ptr<Portal> exit = m_PairPortal.lock();
        if (player){
            player->m_WorldPos = exit->m_WorldPos + glm::vec2(0, 50);
        }
        LOG_DEBUG("You can't interact with exit Portal");
    }
}

void Portal::PopUpText(){
    if (m_Light < 255) m_Light++;
    auto text = std::dynamic_pointer_cast<Util::Text>(m_Text);
    text->SetColor(Util::Color(m_Light, m_Light, m_Light));
    m_Text->SetVisible(true);
}

void Portal::Close(){
    if (m_Light > 127){
        m_Light--;
        auto text = std::dynamic_pointer_cast<Util::Text>(m_Text);
        text->SetColor(Util::Color(m_Light, m_Light, m_Light));
    }
    else{
        m_Text->SetVisible(false);
    }
}

void Portal::Update(){
    std::shared_ptr<Player> player = m_player.lock();
    std::shared_ptr<Portal> exit = m_PairPortal.lock();

    glm::vec2 D = m_WorldPos - player->m_WorldPos;
    m_Text->SetVisible(glm::length(D) <= 100 && m_Type == Type::entrance);
    auto text = std::dynamic_pointer_cast<Util::Text>(m_Drawable);


    
}