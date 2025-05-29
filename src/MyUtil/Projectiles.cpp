#include "MyUtil/Projectile.hpp"

#include "Character/Player.hpp"

Projectile::Projectile(glm::vec2 Pos, glm::vec2 dir, float velocity, 
    int damage, Faction faction, GameWorldContext& world, 
    float lifeTime, std::string path) :
    m_LifeTime(lifeTime), m_Dir(dir.x > 0 ? 1 : -1), m_Velocity(velocity), 
    m_Damage(damage), m_Faction(faction), m_World(world) 
{
    m_Drawable = std::make_shared<Util::Image>(path);
    m_ZIndex = 30.0f;
    m_Visible = true;
    m_WorldPos = Pos;
    m_Transform.scale = {1.0f * dir.x, 1.0f};
    left = right = 10;
    top = bottom = 5;
}

void Projectile::Update(float dt){
    if (m_IsDestroyed) return;

    m_WorldPos.x += m_Velocity * m_Dir * dt;
    m_LifeTime -= dt;

    if (m_LifeTime <= 0.f){
        m_IsDestroyed = true;
        return;
    }

    if (m_Faction == Faction::Enemy){
        std::shared_ptr<Player> player = m_World.m_Player.lock();
        if (player == nullptr) return;
        if (Collision::IsIntersect(this, player.get())){
            player->Attacked(m_Damage, glm::vec2(m_Dir, 0.f), 5.0f);
            m_IsDestroyed = true;
        }
    }

    auto objs = m_World.SolidObjs;
    objs.insert(objs.end(), m_World.OneSidedPlatforms.begin(), m_World.OneSidedPlatforms.end());
    for (auto& temp : objs){
        glm::vec2 D = m_WorldPos - temp->m_WorldPos;
        if (glm::length(D) >= 1000.0f) continue;
        if (Collision::IsIntersect(this, temp.get())){
            m_IsDestroyed = true;
            break;
        }
    }
}