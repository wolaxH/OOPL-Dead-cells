#include "App.hpp"


void App::MainPageInit(){

    
    //Init Menu
    LOG_TRACE("Init_Menu");

    //Init WIP
    WIP = std::make_shared<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/bg/WIPpage.png"), 10.0f);
    WIP->SetVisible(false);
    WIP->m_Transform.scale = {2.0f, 2.0f};
    WIP->m_Transform.translation = {100.0f, 0.0f};
    root.AddChild(WIP);

    //Init PLaybutton
    auto buttondrawable = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 30, "Play", Util::Color(255,255,255));
    auto PlayButton = std::make_shared<Button>(buttondrawable, 3.0f, "Play", [this](){
        root.RemoveChild(StartMenu);
        m_CurrentState = State::INIT_UPDATE;
    });
    PlayButton->m_Transform.translation = {-550.0f, 0.0f};

    //Init Optionbutton
    buttondrawable = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 30, "Option", Util::Color(255,255,255));
    auto OptionButton = std::make_shared<Button>(buttondrawable, 3.0f, "Option", [this](){
        WIPFlag = true;
        WIP->SetVisible(true);
    });
    OptionButton->m_Transform.translation = {-550.0f, -100.0f};

    //Init Exitbutton
    buttondrawable = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 30, "Exit", Util::Color(255,255,255));
    auto ExitButton = std::make_shared<Button>(buttondrawable, 3.0f, "Exit", [this](){
        m_CurrentState = State::END;
    });
    ExitButton->m_Transform.translation = {-550.0f, -200.0f};
    
    auto Bg = std::make_shared<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/menu/menu_bg.png"), 2.f);
    Bg->m_Transform.scale = glm::vec2(1/1.5f, 1/1.5f);

    auto SelectBar = std::make_shared<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/menu/selectbar.png"), 2.0);
    StartMenu = std::make_shared<Menu>(SelectBar, Bg, std::vector<std::shared_ptr<Button>>{PlayButton, OptionButton, ExitButton}, glm::vec2(125, 21));

    root.AddChild(StartMenu);
    m_CurrentState = State::MENU;
}