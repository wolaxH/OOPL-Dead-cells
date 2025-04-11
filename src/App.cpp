#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"


// std::vector<std::shared_ptr<SolidObj>> SolidObjs;
// std::vector<std::shared_ptr<Mob>> Mobs;

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
    player = std::make_shared<Player>(Img, 100, SolidObjs);
    player->SetPos({0, 0});
    player->SetZIndex(5);
    player->SetVisible(true);
    root.AddChild(player);
    camera.SetPos(player->m_WorldPos);

    Img.clear();
    for (int i = 0; i < 24; i++){
        Img.push_back(RESOURCE_DIR"/Zombie/idle/idle_" + std::to_string(i) + ".png");
    }
    zombie = std::make_shared<Zombie>(Img, 100, player, SolidObjs);
    zombie->SetPos({0, 0});
    zombie->SetZIndex(100);
    zombie->SetVisible(true);
    MapObjs.push_back(zombie);
    root.AddChild(zombie);


    //bg
    glm::vec2 temp_pos(1, 1);
    bg1 = std::make_shared<BG>(RESOURCE_DIR"/bg/bg1.png", temp_pos, 1.0f);
    // bg2 = std::make_shared<BG>(RESOURCE_DIR"/bg/bg2.png", temp_pos, 1.0f);
    // bg2->m_WorldPos = {640, 0};
    // bg3 = std::make_shared<BG>(RESOURCE_DIR"/bg/bg3.png", temp_pos, 1.0f);
    // bg3->m_WorldPos = {640*2, 0};
    BGs.push_back(bg1);
    // BGs.push_back(bg2);
    // BGs.push_back(bg3);
    MapObjs.push_back(bg1);
    // MapObjs.push_back(bg2);
    // MapObjs.push_back(bg3);
    root.AddChild(bg1);
    // root.AddChild(bg2);
    // root.AddChild(bg3);

    for (int i = 0; i < 3; i++){
        auto Solid = std::make_shared<SolidObj>(RESOURCE_DIR"/bg/green.png");
        SolidObjs.push_back(Solid);   
    }

    for (int i = 0; i < SolidObjs.size(); i++){
        SolidObjs[i]->get_data_from_json(i);
        //SolidObjs[i]->m_Transform.translation = SolidObjs[i]->m_WorldPos;
        MapObjs.push_back(SolidObjs[i]);
    }

    
    /*
    
    for(auto& temp : SolidObjs){
        temp->m_Transform.translation = temp->m_WorldPos;
        MapObjs.push_back(temp);
    }
    
    */
   //open SolidObjs hintbox

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
