#include "App.hpp"

void App::RetryInit(){
    player = nullptr;

    m_World.Clear();



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
    
    

    std::vector<glm::vec2> zombie_positions = {
        //{1200, 500}, {1500, 500}, {1800, 520},{2346,500},
        {4812, 879},{4770, 879},{5481,20},{6600,-1353},{5255,1027},
        {7778,-2159},{4887,-180},{7511,-2059},{7920,-2059},{8388,-2543}


    };

    Img.clear();
    for (int i = 0; i < 24; i++){
        Img.push_back(RESOURCE_DIR"/Zombie/idle/idle_" + std::to_string(i) + ".png");
    }

    for (const auto& pos : zombie_positions) {
        auto zombie = std::make_shared<Zombie>(Img, 200, player, m_World);
        zombie->SetPos(pos);
        m_World.Mobs->AddObj(zombie);
    }


    std::vector<glm::vec2> shooter_positions = {
        //{2000, 600}, {1154, -836}, 
        {2416, 212},{2776, 877},{4812, 879},{9004,-2347},{6258,-2675},
        {5200,-2344},{5200,-2344},{6697,-2142},{6011,-200},{3154, 492},{5432, 10},{5346,769},{5255,1027},{6181,-1211},
        {6591,-1353}};

    Img.clear();
    for (int i = 0; i < 6; i++){
        Img.push_back(RESOURCE_DIR"/shooter/idle/idle_" + std::to_string(i) + ".png");
    }

    for (const auto& pos : shooter_positions) {
        auto shooter = std::make_shared<Shooter>(Img, 200, player, m_World);
        shooter->SetPos(pos);
        m_World.Mobs->AddObj(shooter);
    }


    Img.clear();
    for (int i = 0; i < 26; i++){
        Img.push_back(RESOURCE_DIR"/Boss/Idle/idle_" + std::to_string(i) + ".png");
    }
    std::shared_ptr<Boss> boss = std::make_shared<Boss>(Img, 2000, player, m_World);
    boss->m_WorldPos = glm::vec2(16000, 200);
    m_World.Mobs->AddObj(boss);

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

    for (const auto& temp : m_World.WorldDrops->GetObjs()){
        auto drop = std::dynamic_pointer_cast<MapObj>(temp);
        if (drop) MapObjs.push_back(drop);
    }

    for (const auto& temp : m_World.Mobs->GetObjs()){
        auto mob = std::dynamic_pointer_cast<MapObj>(temp);
        if (mob) MapObjs.push_back(mob);
    }

    m_CurrentState = State::UPDATE;
}