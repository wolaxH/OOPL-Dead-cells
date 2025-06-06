#include "TreasureChest.hpp"

TreasureChest::TreasureChest(std::shared_ptr<Item> Treasure, GameWorldContext& WorldCtx)
     : m_Treasure(Treasure), m_Type(Type::closed), m_World(WorldCtx){
    m_Drawable = std::make_shared<Util::Image>(RESOURCE_DIR"/InterAct/Treasurechest_Closed.png");
}

void TreasureChest::InterAct(){
    if(m_Type != Type::opened){
        m_Drawable = std::make_shared<Util::Image>(RESOURCE_DIR"/InterAct/Treasurechest_Open.png");
        
    }
}

void TreasureChest::Update(){
}
