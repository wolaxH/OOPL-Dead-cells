#include "Item/Drink.hpp"

Drink::Drink(){
    for (int i = 0; i < 4; i++){
        m_drawables.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Item/drink/bottle" + std::to_string(i) + ".png"));
    }
    m_Drawable = m_drawables[0];
    
}

Drink::state Drink::GetNextState(){
    int stateNum = static_cast<int>(m_state);
    return static_cast<state>(++stateNum);
}

void Drink::Use(int& PLayer_Hp){

}