#include "App.hpp"

void App::SummaryInit(){
    auto drab = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/SummaryBg.png");
    m_SummaryBg = std::make_shared<Util::GameObject>(drab, 98);
    m_SummaryBg->m_Transform.scale = glm::vec2(50, 50 * 9/16);

    auto text = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 50, "Score:\n" + std::to_string(player->GetScore()), Util::Color(255,255,255));
    m_SummaryScore = std::make_shared<Util::GameObject>(text, 99);
    
    root.AddChild(m_SummaryBg);
    root.AddChild(m_SummaryScore);

    m_SummaryTimer.SetTime(5000, 5000);
    m_SummaryTimer.ResetTime();
    m_CurrentState = State::SUMMARY;
}

void App::Summary(){
    if (m_SummaryTimer.IsTimeout()){
        root.RemoveChild(m_SummaryBg);
        root.RemoveChild(m_SummaryScore);
        m_CurrentState = State::INIT_RETRY;
    }
    else{root.Update();}
}