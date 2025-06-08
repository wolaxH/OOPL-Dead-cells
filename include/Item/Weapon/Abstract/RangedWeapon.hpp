// #ifndef RANGEDWEAPON_HPP
// #define RANGEDWEAPON_HPP

// #include "Item/PickUp.hpp"
// #include "MyUtil/Projectile.hpp"
// #include "MyUtil/GameWorldContext.hpp"

// #include "Util/Animation.hpp"

// class RangedWeapon : public Item{
// public:
//     RangedWeapon(const std::string& path, const std::string& Describe, GameWorldContext& GameCtx)
//         : Item(path), m_world(GameCtx){
//             auto T = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 20, Describe, Util::Color(255, 255, 255));
//             m_Describe = std::make_shared<Util::GameObject>(T, 15.0f);
//             m_Describe->SetVisible(false);
//     }
//     virtual ~RangedWeapon() noexcept = default;

//     auto GetPlayerDrawable() const noexcept {return m_PlayerDrawable;}

//     virtual void shoot(glm::vec2 Pos, glm::vec2 Dir) = 0;

// protected:
//     void FireProjectile(const std::string& path, glm::vec2 Pos, glm::vec2 Dir){
//         int projectileDir = Dir.x > 0 ? 1 : -1;
//         glm::vec2 projectilePos = Pos + glm::vec2(projectileDir * 50.0f, 50);

//         auto projectile = std::make_shared<Projectile>(projectilePos, projectileDir, m_Velocity, m_Damage, 
//             Projectile::Faction::Player, m_world, m_LifeTime, path);
//     }

// protected:
//     std::shared_ptr<Util::Animation> m_PlayerDrawable;
//     GameWorldContext& m_world;
//     float m_Velocity;
//     int m_Damage;
//     float m_LifeTime;

// };

// #endif