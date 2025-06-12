#include "Item/Weapon/Bow.hpp"

Bow::Bow(GameWorldContext& ctx) : 
    Weapon(RESOURCE_DIR"/Item/Shoot/Icon.png", "A Bow"),
    m_World(ctx)
    {
        m_SegCount = 1;
        m_Player_Drawables.resize(m_SegCount);
        m_AtkPoint.resize(m_SegCount);
        m_AtkTimes.resize(m_SegCount);
        m_AtkableFrames.resize(m_SegCount);

        std::vector<std::string> paths;
        for (int i = 0; i < 24; i++){
            paths.push_back(RESOURCE_DIR"/Item/Shoot/Behavior/Shoot_" + std::to_string(i) + ".png");
        }
        paths.push_back(RESOURCE_DIR"/Item/Shoot/Behavior/Shooting_0.png");
        paths.push_back(RESOURCE_DIR"/Item/Shoot/Behavior/Shooting_1.png");
        m_Player_Drawables[0] = std::make_shared<Util::Animation>(paths, true, 15, false, 0);

        m_AtkPoint[0] = 20;
        m_AtkTimes[0] = 1;
        m_AtkableFrames[0] = {24, 25};
    }

void Bow::Use(std::vector<std::shared_ptr<GameObject>>& Objs, const glm::vec2& Pos, bool& UsedFlag, const glm::vec2& Dir, int combo){
    Shoot(Dir, Pos);
}

void Bow::Shoot(const glm::vec2& Dir, const glm::vec2& Pos){
    glm::vec2 bulletPos = Pos + glm::vec2(0, 60);

    auto projectile = std::make_shared<Projectile>(bulletPos, Dir, 20.0f, m_AtkPoint[0], 
        Projectile::Faction::Player, m_World, 3.0f, RESOURCE_DIR"/Item/Shoot/arrow.png");
    
    m_World.Projectiles->AddObj(projectile);
}
