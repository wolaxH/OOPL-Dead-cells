#include "InterAct/TreasureChest.hpp"

TreasureChest::TreasureChest(std::shared_ptr<Item> Treasure, GameWorldContext& WorldCtx)
     : m_Treasure(Treasure), m_Type(Type::closed), m_World(WorldCtx){
    m_Drawable = std::make_shared<Util::Image>(RESOURCE_DIR"/InterAct/Treasurechest_Closed.png");
    m_ZIndex = 15.0f;
    m_Transform.scale = glm::vec2(1.7f, 1.4f);
}

void TreasureChest::InterAct(){
    if(m_Type != Type::opened){
        m_Drawable = std::make_shared<Util::Image>(RESOURCE_DIR"/InterAct/Treasurechest_Open.png");
        m_Type = Type::opened;
        auto drop = m_Treasure->ToDrops();
        drop->m_WorldPos = m_WorldPos + glm::vec2(0, 50);
        m_World.WorldDrops->AddObj(drop);
        m_Treasure = nullptr;
    }
}

void TreasureChest::Update(){
    //Treasure chest has no update behavior
}
