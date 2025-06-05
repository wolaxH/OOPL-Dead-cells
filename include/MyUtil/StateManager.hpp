#ifndef StateManager_HPP
#define StateManager_HPP

#include <unordered_map>
#include <unordered_set>
//WIP
enum class c_state{ //Character state
    idle,
    L_move,
    R_move,
    jump,
    fall,
    atk,
    clinb,
    clinbOSP,
    crouch,
    roll,
    atked
};

class StateManager{
    public:
    StateManager() = default;
    ~StateManager() = default;
private:
    std::unordered_map<c_state, std::unordered_set<c_state>> interruptibleTable = {
        {c_state::idle, {}},
        {c_state::L_move, {c_state::idle}},
        {c_state::R_move, {c_state::idle}},
        {c_state::jump, {c_state::fall, c_state::idle, c_state::roll}},
        {c_state::fall, {}}

    };    
};
#endif