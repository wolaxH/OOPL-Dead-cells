#include "App.hpp"

void App::InGameInit() {

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
        MapObjs.push_back(SolidObjs[i]);
    }

    
    /*
     *  open SolidObjs hintbox

     std::vector<std::shared_ptr<Util::GameObject>> temps;
     for (auto& temp : SolidObjs){ temps.push_back(temp);}
     root.AddChildren(temps);
    */
   
   
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
}


