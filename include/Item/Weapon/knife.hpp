#ifndef KNIFE_HPP
#define KNIFE_HPP

#include "Item/Weapon/Weapon.hpp"

class Knife : public Weapon{
public:
    Knife() : 
        Weapon( RESOURCE_DIR"/Weapon/Knife.png", 20, 50.0f, "Knife"){
            //建立player_Drawable
        }
    ~Knife() = default;

private:

};


#endif // KNIFE_HPP