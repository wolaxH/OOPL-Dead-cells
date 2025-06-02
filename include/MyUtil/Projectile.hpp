#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Abstract/MapObj.hpp"
#include "MyUtil/Collision.hpp"
#include "MyUtil/GameWorldContext.hpp"
#include "MyUtil/Timer.hpp"
#include "Util/Image.hpp"

class Projectile : public MapObj{
public:
    enum class Faction{
        Player,
        Enemy
    };

    Projectile(glm::vec2 Pos, glm::vec2 dir, float velocity, 
        int damage, Faction faction, GameWorldContext& world, 
        float lifeTime = 50.0f, std::string path = "");
    ~Projectile() = default;

    void Update(float dt);

    bool IsDestroyed() const noexcept { return m_IsDestroyed; }

private:
    float m_LifeTime;
    int m_Dir;  // 1 for right, -1 for left
    float m_Velocity;
    int m_Damage;
    bool m_IsDestroyed = false;
    Faction m_Faction;
    GameWorldContext& m_World;
    Timer m_Timer;
};

#endif