#include "Item/Drink.hpp"

Drink::Drink(){
    m_drawables.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Item/drink/bottle.png"));
    for (int i = 1; i < 4; i++){
        m_drawables.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Item/drink/bottle" + std::to_string(i) + ".png"));
    }
    m_Drawable = m_drawables[0];
}

Drink::state Drink::GetNextState(){

}

void Drink::Use(int& PLayer_Hp){

}