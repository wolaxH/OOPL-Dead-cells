#include "Item/HealBottle.hpp"

/**
 * m_drawable(std::vector) to 
 * m_Drawable(std::unorder_map)
 * apply Drink to UI
 */
HealBottle::HealBottle(){

    for (int i = 0; i < static_cast<int>(state::count); ++i) {
        state val = static_cast<state>(i);
        m_Drawables[val] = std::make_shared<Util::Image>(RESOURCE_DIR"/Item/drink/bottle" + std::to_string(i) + ".png");
    }

    m_Drawable = m_Drawables[state::full];
    m_heal = 160;
}

void HealBottle::ToNextState(){
    int currentStateNum = static_cast<int>(m_state);
    if (currentStateNum >= static_cast<int>(state::empty)) return;
    m_state = static_cast<state>(++currentStateNum);
}

void HealBottle::Drink(int& Player_Hp){
    if (!IsNonEmpty()) return;
    Player_Hp += m_heal;
    if (Player_Hp > 200) Player_Hp = 200;
    ToNextState();
    try{
        m_Drawable = m_Drawables.at(m_state);
    }
    catch(const std::out_of_range &e){
        LOG_ERROR("HealBottle::GetcurrentDrawable() m_Drawables out_of_range");
    }
}