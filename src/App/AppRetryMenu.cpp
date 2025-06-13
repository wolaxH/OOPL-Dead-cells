#include "App.hpp"

void App::RetryMenuInit(){

    auto a = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 102, "菜", Util::Color(255,255,255));
    m_noob = std::make_shared<Util::GameObject>(a, 99);
    m_noob->m_Transform.translation = glm::vec2(0, 150);

    auto Img = std::make_shared<Util::Image>(RESOURCE_DIR"/menu/retryBg.png");
    auto Bg = std::make_shared<Util::GameObject>(Img, 99);
    Bg->m_Transform.scale = glm::vec2(6, 6);
    // Bg->m_Transform.translation = glm::vec2(0, 300);

    auto SelectBar = std::make_shared<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/menu/selectMid.png"), 2.0f);
    SelectBar->m_Transform.scale *= 3.5;

    auto text = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 30, "Retry", Util::Color(255,255,255));
    auto retryButton = std::make_shared<Button>(text, 100, "Retry", [this](){
        m_CurrentState = State::INIT_RETRY;
        root.RemoveChild(m_RetryMenu);
        root.RemoveChild(m_noob);
    });
    retryButton->m_Transform.translation = {0, -100};

    text = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 30, "Quit", Util::Color(255,255,255));
    auto quitButton = std::make_shared<Button>(text, 100, "Quit", [this](){
        m_CurrentState = State::END;
    });
    quitButton->m_Transform.translation = glm::vec2(0, -200);

    m_RetryMenu = std::make_shared<Menu>(SelectBar, Bg, std::vector<std::shared_ptr<Button>>{retryButton, quitButton});
    root.AddChild(m_RetryMenu);
    root.AddChild(m_noob);
    m_CurrentState = State::RETRY_MENU;
}


void App::RetryMenu(){

    m_RetryMenu->Update();
    root.Update();
    
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}