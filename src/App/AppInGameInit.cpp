#include "App.hpp"

#include "json.hpp"

#include <fstream>

void App::InGameInit() {

    Drops::m_World = &m_World;

    std::vector<std::string> Img;
    Img.reserve(46);
    for (int i = 0; i < 46; i++){
        Img.push_back(RESOURCE_DIR"/Beheaded/idle/idle_" + std::to_string(i) + ".png");
    }
    player = std::make_shared<Player>(Img, 200, m_World);
    player->Init();
    player->SetPos({0, 100});
    player->SetZIndex(30);
    player->SetVisible(true);
    root.AddChild(player);
    camera.SetPos(player->m_WorldPos);
    

    Img.clear();
    for (int i = 0; i < 24; i++){
        Img.push_back(RESOURCE_DIR"/Zombie/idle/idle_" + std::to_string(i) + ".png");
    }
    zombie = std::make_shared<Zombie>(Img, 200, player, m_World);
    zombie->SetPos({0, 100});
    zombie->SetZIndex(30);
    zombie->SetVisible(true);
    m_World.Mobs->AddObj(zombie);
    

    //test
    auto RW = std::make_shared<RustySword>();
    auto B = std::make_shared<Bone>();
    auto TD = std::make_shared<TwinDaggers>();

    auto t = RW->ToDrops();
    m_World.WorldDrops->AddObj(t);
    LOG_DEBUG("Rw init");

    t = B->ToDrops();
    t->m_WorldPos.x += 100;
    m_World.WorldDrops->AddObj(t);
    LOG_DEBUG("B init");

    t = TD->ToDrops();
    t->m_WorldPos.x -= 400;
    m_World.WorldDrops->AddObj(t);
    LOG_DEBUG("TD init");
    //test


    //bg
    glm::vec2 temp_pos(1, 1);
    BGs.resize(6);
    BGs[0] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg1.png", temp_pos, glm::vec2{0.8f, 0.8f});
    BGs[1] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg2.png", glm::vec2{1616.0f, 174.0f}, glm::vec2{0.9f, 0.9f});
    BGs[2] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg3.png", glm::vec2{3817.0f, 318.5f}, glm::vec2{1.4f, 1.4f});
    BGs[3] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg5.png", glm::vec2{6309.5f, 316.5f}, glm::vec2{1.2f, 1.2f});
    BGs[4] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg6.png", glm::vec2{6309.5f, -970.0f}, glm::vec2{1.2f, 1.2f});
    BGs[5] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg4.png", glm::vec2{1616.0f, -673.0f}, glm::vec2{0.9f, 0.9f});
    for (auto& temp : BGs){
        MapObjs.push_back(temp);
        root.AddChild(temp);
    }


    /**
     * To create a one sided platform object
     */
    InitColliders<OneSidedPlatform>("OSPs.json", m_World.OneSidedPlatforms);


    /**
     * To create a solid object
     */
    InitColliders<SolidObj>("SolidObjs.json", m_World.SolidObjs);
    
    

    /**
     * Add all solid objects to the root object for rendering.
     */
    std::vector<std::shared_ptr<Util::GameObject>> temps;
    for (auto& temp : m_World.SolidObjs){ temps.push_back(temp);}
    for (auto& temp : m_World.OneSidedPlatforms){ temps.push_back(temp);}
    root.AddChildren(temps);


    root.AddChild(m_World.WorldDrops);
    root.AddChild(m_World.Mobs);
    for (const auto& temp : m_World.WorldDrops->GetObjs()){
        auto drop = std::dynamic_pointer_cast<MapObj>(temp);
        if (drop) MapObjs.push_back(drop);
    }

    for (const auto& temp : m_World.Mobs->GetObjs()){
        auto mob = std::dynamic_pointer_cast<MapObj>(temp);
        if (mob) MapObjs.push_back(mob);
    }

    LOG_TRACE("Start");
    
    m_CurrentState = State::UPDATE;
}