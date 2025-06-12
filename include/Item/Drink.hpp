#ifndef DRINK_HPP
#define DRINK_HPP

#include "Item/PickUp.hpp"

class Drink : public Item{
public:
    enum class state{
        full,
        half,
        little,
        empty
    };
    Drink();
    ~Drink() = default;

    void Use(int& PLayer_Hp);

private:
    state GetNextState();
private:
    state m_state = state::full;
    std::vector<std::shared_ptr<Util::Image>> m_drawables;
};

#endif