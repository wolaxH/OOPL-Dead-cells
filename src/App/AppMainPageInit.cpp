#include "App.hpp"


void App::MainPageInit(){

    
    //Init Menu
    LOG_TRACE("Init_Menu");
    Menu_Bg = std::make_shared<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/menu/menu_bg.png"), 1.0f);
    Menu_Bg->m_Transform.scale = {1/1.5f, 1/1.5f};
    root.AddChild(Menu_Bg);

    //Init WIP
    WIP = std::make_shared<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/bg/WIPpage.png"), 10.0f);
    WIP->SetVisible(false);
    WIP->m_Transform.scale = {2.0f, 2.0f};
    WIP->m_Transform.translation = {100.0f, 0.0f};
    root.AddChild(WIP);

    //Init PLaybutton
    auto buttondrawable = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 30, "Play", Util::Color(255,255,255));
    PlayButton = std::make_shared<Button>(buttondrawable, 3.0f, "Play", [this](){
        root.RemoveChild(Menu_Bg);
        root.RemoveChild(PlayButton);
        root.RemoveChild(StartMenu);
        root.RemoveChild(ExitButton);
        root.RemoveChild(OptionButton);
        m_CurrentState = State::INIT_UPDATE;
    });
    PlayButton->m_Transform.translation = {-550.0f, 0.0f};
    root.AddChild(PlayButton);

    //Init Optionbutton
    buttondrawable = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 30, "Option", Util::Color(255,255,255));
    OptionButton = std::make_shared<Button>(buttondrawable, 3.0f, "Option", [this](){
        WIPFlag = true;
        WIP->SetVisible(true);
    });
    OptionButton->m_Transform.translation = {-550.0f, -100.0f};
    root.AddChild(OptionButton);

    //Init Exitbutton
    buttondrawable = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 30, "Exit", Util::Color(255,255,255));
    ExitButton = std::make_shared<Button>(buttondrawable, 3.0f, "Exit", [this](){
        m_CurrentState = State::END;
    });
    ExitButton->m_Transform.translation = {-550.0f, -200.0f};
    root.AddChild(ExitButton);
    
    StartMenu = std::make_shared<Menu>(std::vector<std::shared_ptr<Button>>{PlayButton, OptionButton, ExitButton});

    root.AddChild(StartMenu);
    m_CurrentState = State::MENU;
}