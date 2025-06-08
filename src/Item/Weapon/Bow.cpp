// #include "Item/Weapon/Bow.hpp"

// Bow::Bow(GameWorldContext& GameCtx) : RangedWeapon(RESOURCE_DIR"/Item/Shoot/Icon.png", "A Bow", GameCtx){
//     std::vector<std::string> paths;
//     for (int i = 0; i < 24; i++){
//         paths.push_back(RESOURCE_DIR"/Item/Shoot/Behavior/Shoot_" + std::to_string(i) + ".png");
//     }
//     paths.push_back(RESOURCE_DIR"/Item/Shoot/Behavior/Shooting_0.png");
//     paths.push_back(RESOURCE_DIR"/Item/Shoot/Behavior/Shooting_1.png");
// }

// void Bow::shoot(glm::vec2 Pos, glm::vec2 Dir){
//     FireProjectile(RESOURCE_DIR"/Item/Shoot/arrow", Pos, Dir);
// }