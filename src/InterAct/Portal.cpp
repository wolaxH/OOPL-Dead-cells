#include "InterAct/Portal.hpp"

Portal::Portal(std::shared_ptr<Player> player, glm::vec2 Pos, Type type, std::string path) : m_Type(type), m_player(player){
    m_WorldPos = Pos;
    m_Drawable = std::make_shared<Util::Image>(path);
    m_Light = 127;
    m_ZIndex = 25.0;

    m_Text = std::make_shared<MapObj>();
    m_Text->m_WorldPos = m_WorldPos + glm::vec2(0, 100);
    m_TextDrawable= std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 24, "是否前往Boss關卡", Util::Color(255, 255, 255));
    m_Text->SetDrawable(m_TextDrawable);
    m_Text->SetVisible(false);
    m_Text->SetZIndex(25.1f);
    AddChild(m_Text);
    m_Transform.scale = {1.2, 1.2};
}

void Portal::InterAct(){
    if (m_Type != Type::exit){
        std::shared_ptr<Player> player = m_player.lock();
        std::shared_ptr<Portal> exit = m_PairPortal.lock();
        if (player && exit){
            player->m_WorldPos = exit->m_WorldPos + glm::vec2(0, 50);
        }
    }
}

void Portal::PopUpText(){
    if (m_Light < 255) m_Light += 4;
    m_TextDrawable->SetColor(Util::Color(m_Light, m_Light, m_Light));
    m_Text->SetDrawable(m_TextDrawable);
    m_Text->SetVisible(true);
}

void Portal::Close(){
    if (m_Light > 127){
        m_Light -= 4;
        m_TextDrawable->SetColor(Util::Color(m_Light, m_Light, m_Light));
        m_Text->SetDrawable(m_TextDrawable);
    }
    else{
        m_Text->SetVisible(false);
    }
}

void Portal::Update(){
    std::shared_ptr<Player> player = m_player.lock();
    std::shared_ptr<Portal> exit = m_PairPortal.lock();

    glm::vec2 D = m_WorldPos - player->m_WorldPos;
    glm::length(D) <= 100 && m_Type == Type::entrance ? PopUpText() : Close();
}