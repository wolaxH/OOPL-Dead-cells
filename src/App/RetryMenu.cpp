#include "App.hpp"

void App::RetryMenu(){
    auto Img = std::make_shared<Util::Image>(RESOURCE_DIR"/menu.retryBg.png");
    auto Bg = std::make_shared<Util::GameObject>(Img, 100);

    auto text = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 30, "Retry", Util::Color(255,255,255));
    auto retrybutton = std::make_shared<Button>(text, 99.0f, "Retry", [this](){
        m_CurrentState = State::INIT_RETRY;
        root.RemoveChild(m_RetryMenu);
    });

    
}