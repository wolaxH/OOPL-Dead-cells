#include "App.hpp"

#include "json.hpp"

#include <fstream>

#include "Util/Logger.hpp"
void App::InGameInit() {

    std::vector<std::string> Img;
    Img.reserve(46);
    for (int i = 0; i < 46; i++){
        Img.push_back(RESOURCE_DIR"/Beheaded/idle/idle_" + std::to_string(i) + ".png");
    }
    player = std::make_shared<Player>(Img, 100, SolidObjs, OneSidedPlatforms);
    player->SetPos({0, 0});
    player->SetZIndex(5);
    player->SetVisible(true);
    root.AddChild(player);
    camera.SetPos(player->m_WorldPos);

    Img.clear();
    for (int i = 0; i < 24; i++){
        Img.push_back(RESOURCE_DIR"/Zombie/idle/idle_" + std::to_string(i) + ".png");
    }
    zombie = std::make_shared<Zombie>(Img, 100, player, SolidObjs, OneSidedPlatforms);
    zombie->SetPos({0, 0});
    zombie->SetZIndex(100);
    zombie->SetVisible(true);
    MapObjs.push_back(zombie);
    // root.AddChild(zombie);


    //bg
    glm::vec2 temp_pos(1, 1);
    bg1 = std::make_shared<BG>(RESOURCE_DIR"/bg/bg1.png", temp_pos, 1.0f);
    bg2 = std::make_shared<BG>(RESOURCE_DIR"/bg/bg2.png", temp_pos, 1.0f);
    bg2->m_WorldPos = {1518.0f, 144.0f};
    bg3 = std::make_shared<BG>(RESOURCE_DIR"/bg/back3.png", temp_pos, 1.0f);
    bg3->m_Transform.scale = {1.5f, 1.5f};
    bg3->m_WorldPos = {3717.0f, 391.5f};
    BGs.push_back(bg1);
    BGs.push_back(bg2);
    BGs.push_back(bg3);
    MapObjs.push_back(bg1);
    MapObjs.push_back(bg2);
    MapObjs.push_back(bg3);
    root.AddChild(bg1);
    root.AddChild(bg2);
    root.AddChild(bg3);



    /**
     * To create a one sided platform object
     */
    std::ifstream file(std::string(DATA_DIR) + "/" + "OSPs.json");
    nlohmann::json j;
    file >> j;
    OneSidedPlatforms.reserve(j.size());
    for (size_t i = 0; i < j.size(); i++){
        auto OSP = std::make_shared<OneSidedPlatform>(RESOURCE_DIR"/bg/red.png");
        OneSidedPlatforms.push_back(OSP);
    }
    for (size_t i = 0; i < OneSidedPlatforms.size(); i++){
        OneSidedPlatforms[i]->get_data_from_json("OSPs.json", i);
        MapObjs.push_back(OneSidedPlatforms[i]);
    }
    file.close();



    /**
     * To create a solid object
     */
    file.open(std::string(DATA_DIR) + "/" + "SolidObjs.json");
    file >> j;
    SolidObjs.reserve(j.size());
    for (size_t i = 0; i < j.size(); i++){
        auto Solid = std::make_shared<SolidObj>(RESOURCE_DIR"/bg/green.png");
        SolidObjs.push_back(Solid);   
    }

    for (size_t i = 0; i < SolidObjs.size(); i++){
        SolidObjs[i]->get_data_from_json("SolidObjs.json", i);
        MapObjs.push_back(SolidObjs[i]);
    }

    
    /**
     * Add all solid objects to the root object for rendering.
     */
    std::vector<std::shared_ptr<Util::GameObject>> temps;
    for (auto& temp : SolidObjs){ temps.push_back(temp);}
    for (auto& temp : OneSidedPlatforms){ temps.push_back(temp);}
    root.AddChildren(temps);

    
   
   
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
}


