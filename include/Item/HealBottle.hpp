#ifndef HEALBOTTLE_HPP
#define HEALBOTTLE_HPP

#include "Item/PickUp.hpp"

#include <unordered_map>

class HealBottle : public Item{
public:
    enum class state{
        full = 0,
        half,
        little,
        empty,
        count
    };
    HealBottle();
    ~HealBottle() = default;

    void Drink(int& Player_Hp);

    bool IsNonEmpty(){ return m_state != state::empty;}

    state GetState() const noexcept{ return m_state;}

private:
    void ToNextState();
private:
    state m_state = state::full;
    std::unordered_map<state, std::shared_ptr<Util::Image>> m_Drawables;
    int m_heal;
};

#endif