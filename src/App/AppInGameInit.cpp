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
    player->SetPos({0, 0});
    player->SetZIndex(30.1);

    root.AddChild(player);
    camera.SetPos(player->m_WorldPos);
    m_World.m_Player = player;
    

    Img.clear();
    for (int i = 0; i < 24; i++){
        Img.push_back(RESOURCE_DIR"/Zombie/idle/idle_" + std::to_string(i) + ".png");
    }
    std::shared_ptr<Zombie> zombie = std::make_shared<Zombie>(Img, 200, player, m_World);
    zombie->SetPos({1200, 500});
    m_World.Mobs->AddObj(zombie);

    Img.clear();
    for (int i = 0; i < 6; i++){
        Img.push_back(RESOURCE_DIR"/shooter/idle/idle_" + std::to_string(i) + ".png");
    }
    std::shared_ptr<Shooter> shooter = std::make_shared<Shooter>(Img, 200, player, m_World);
    shooter->SetPos({1200, 500});
    m_World.Mobs->AddObj(shooter);
    

    //Item test
    auto RW = std::make_shared<RustySword>();
    auto B = std::make_shared<Bone>();
    auto TD = std::make_shared<TwinDaggers>();
    auto WS = std::make_shared<WoodenShield>();
    auto bow  = std::make_shared<Bow>(m_World);

    auto t = RW->ToDrops();
    m_World.WorldDrops->AddObj(t);

    t = B->ToDrops();
    t->m_WorldPos.x += 100;
    m_World.WorldDrops->AddObj(t);

    t = TD->ToDrops();
    t->m_WorldPos.x -= 400;
    m_World.WorldDrops->AddObj(t);

    t = WS->ToDrops();
    t->m_WorldPos.x -= 200;
    m_World.WorldDrops->AddObj(t);

    t = bow->ToDrops();
    t->m_WorldPos.x += 200;
    m_World.WorldDrops->AddObj(t);
    //Item test end


    //bg
    glm::vec2 temp_pos(1, 1);
    BGs.resize(12);
    BGs[0] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg1.png", temp_pos, glm::vec2{0.8f, 0.8f});
    BGs[1] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg2.png", glm::vec2{1616.0f, 174.0f}, glm::vec2{0.9f, 0.9f});
    BGs[2] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg3.png", glm::vec2{3817.0f, 318.5f}, glm::vec2{1.4f, 1.4f});
    BGs[3] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg5.png", glm::vec2{6309.5f, 316.5f}, glm::vec2{1.2f, 1.2f});
    BGs[4] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg8.png", glm::vec2{6330.5f, -2250.5f}, glm::vec2{1.2f, 1.2f});
    BGs[5] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg7.png", glm::vec2{8605.5f, -970.5f}, glm::vec2{1.2f, 1.2f});
    BGs[6] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg6.png", glm::vec2{6309.5f, -970.0f}, glm::vec2{1.2f, 1.2f});
    BGs[7] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg9.png", glm::vec2{8605.5f, -2250.5f}, glm::vec2{1.2f, 1.2f});
    BGs[8] = std::make_shared<BG>(RESOURCE_DIR"/bg/bg4.png", glm::vec2{1616.0f, -673.0f}, glm::vec2{0.9f, 0.9f});
    BGs[9] = std::make_shared<BG>(RESOURCE_DIR"/bg/BlackBridge.jpg", glm::vec2(15000, 20), glm::vec2(1.5, 1.5));
    BGs[10]= std::make_shared<BG>(RESOURCE_DIR"/bg/bridge.png", glm::vec2(16735, 255), glm::vec2(1.3, 1.5));
    BGs[11]= std::make_shared<BG>(RESOURCE_DIR"/bg/bridge.png", glm::vec2(13810, 255), glm::vec2(-1.3, 1.5));

    for (auto& temp : BGs){
        MapObjs.push_back(temp);
        root.AddChild(temp);
    }

    //Portal
    auto portalI = std::make_shared<Portal>(player, glm::vec2(9525, -2693), Portal::Type::entrance);
    auto portalO = std::make_shared<Portal>(player, glm::vec2(14255, 0), Portal::Type::exit, RESOURCE_DIR"/InterAct/Portal_close.png");
    portalI->Pair(portalO);
    portalO->Pair(portalI);
    m_World.InterActAbles.push_back(portalI);
    m_World.InterActAbles.push_back(portalO);
    
    
    //TreasureChest
    auto treasure = std::make_shared<RustySword>();
    auto Tchest = std::make_shared<TreasureChest>(treasure, m_World);
    Tchest->m_WorldPos = glm::vec2(6369, 620);
    m_World.InterActAbles.push_back(Tchest);
    
    
    std::vector<std::shared_ptr<Util::GameObject>> temps;
    for (auto& temp : m_World.InterActAbles){
        auto mapObj = std::dynamic_pointer_cast<MapObj>(temp);
        root.AddChild(mapObj);
        MapObjs.push_back(mapObj);
        for (auto& child_temp : mapObj->GetChildren()){
            auto child = std::dynamic_pointer_cast<MapObj>(child_temp);
            MapObjs.push_back(child);
        }
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
    temps.clear();

    temps.insert(temps.end(), m_World.OneSidedPlatforms.begin(), m_World.OneSidedPlatforms.end());
    temps.insert(temps.end(), m_World.SolidObjs.begin(), m_World.SolidObjs.end());
    root.AddChildren(temps);


    root.AddChild(m_World.WorldDrops);
    root.AddChild(m_World.Mobs);
    root.AddChild(m_World.Projectiles);
    for (const auto& temp : m_World.WorldDrops->GetObjs()){
        auto drop = std::dynamic_pointer_cast<MapObj>(temp);
        if (drop) MapObjs.push_back(drop);
    }

    for (const auto& temp : m_World.Mobs->GetObjs()){
        auto mob = std::dynamic_pointer_cast<MapObj>(temp);
        if (mob) MapObjs.push_back(mob);
    }

    std::vector<std::string> T_path;
    for (int i = 0; i < 30; i++){
        T_path.push_back(RESOURCE_DIR"/Boss/attack2/atk2_" + std::to_string(i) + ".png");
    }
    for (int i = 0; i < 28; i++){
        T_path.push_back(RESOURCE_DIR"/Boss/move/move_" + std::to_string(i) + ".png");
    }
    T_Anim = std::make_shared<Util::Animation>(T_path, true, 60);
    T_GameObj = std::make_shared<Util::GameObject>(T_Anim, 100.f);
    T_GameObj->m_Transform.translation = glm::vec2(2, 2);
    root.AddChild(T_GameObj);

    LOG_TRACE("Start");
    
    m_CurrentState = State::UPDATE;
}