#include "Item/Weapon/TwinDaggers.hpp"

TwinDaggers::TwinDaggers() : Weapon(RESOURCE_DIR"/Item/TwinDaggers/Icon.png", 20, 50, "this is a Weapon"){

    std::vector<std::string> path;
    m_Player_Drawables.reserve(3);

}