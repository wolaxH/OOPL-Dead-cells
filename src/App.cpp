#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

std::vector<std::shared_ptr<SolidObj>> SolidObjs;
std::vector<std::shared_ptr<Mob>> Mobs;

void App::Init_Start(){

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

void App::Start(){

    if (WIPFlag){
        LOG_DEBUG("WIPFlag is true, WIP is visible");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        WIP->SetVisible(false);
        WIPFlag = false;
        LOG_DEBUG("WIPEND");
    }
    StartMenu->Update();
    root.Update();
    
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::Init_Update() {

    std::vector<std::string> Img;
    Img.reserve(46);
    for (int i = 0; i < 46; i++){
        Img.push_back(RESOURCE_DIR"/Beheaded/idle/idle_" + std::to_string(i) + ".png");
    }
    player = std::make_shared<Player>(Img, 100);
    player->SetPos({0, 0});
    player->SetZIndex(5);
    player->SetVisible(true);
    root.AddChild(player);
    camera.SetPos(player->m_WorldPos);

    Img.clear();
    for (int i = 0; i < 24; i++){
        Img.push_back(RESOURCE_DIR"/Zombie/idle/idle_" + std::to_string(i) + ".png");
    }
    zombie = std::make_shared<Zombie>(Img, 100, player);
    zombie->SetPos({0, 0});
    zombie->SetZIndex(100);
    zombie->SetVisible(true);
    MapObjs.push_back(zombie);
    root.AddChild(zombie);


    ground = std::make_shared<SolidObj>(RESOURCE_DIR"/bg/green.png");
    MapObjs.push_back(ground);
    SolidObjs.push_back(ground);

    ground2 = std::make_shared<SolidObj>(RESOURCE_DIR"/bg/green.png");
    ground2->m_Transform.translation = {640.0f - camera.GetPos().x, 0.0f - camera.GetPos().y};
    ground2->m_WorldPos = ground2->m_Transform.translation;
    ground2->m_Transform.scale = {0.2f, 2.0f};
    MapObjs.push_back(ground2);
    SolidObjs.push_back(ground2);

    ground3 = std::make_shared<SolidObj>(RESOURCE_DIR"/bg/green.png");
    ground3->m_Transform.translation = {-640.0f - camera.GetPos().x, 0.0f - camera.GetPos().y};
    ground3->m_WorldPos = ground3->m_Transform.translation;
    ground3->m_Transform.scale = {0.2f, 2.0f};
    MapObjs.push_back(ground3);
    SolidObjs.push_back(ground3);

    std::vector<std::shared_ptr<Util::GameObject>> temps;
    for (auto& temp : SolidObjs){ temps.push_back(temp);}
    root.AddChildren(temps);
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    player->Update();
    camera.Update(player);
    for (auto& temp : MapObjs){
        temp->m_Transform.translation = temp->m_WorldPos - camera.GetPos();
    }
    zombie->Update();
    root.Update();

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
