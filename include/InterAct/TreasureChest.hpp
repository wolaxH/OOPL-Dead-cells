#ifndef TREASURECHEST_HPP
#define TREASURECHEST_HPP

#include "InterAct/IInterAct.hpp"
#include "Abstract/MapObj.hpp"
#include "Item/PickUp.hpp"
#include "MyUtil/GameWorldContext.hpp"

class TreasureChest : public MapObj, public IInterAct{
public:
    enum class Type{
        opened,
        closed
    };

    TreasureChest(std::shared_ptr<Item> Treasure, GameWorldContext& WsorldCtx);

    void InterAct() override;

    void Update() override;

private:
    std::shared_ptr<Item> m_Treasure;
    Type m_Type;
    GameWorldContext& m_World;
};

#endif