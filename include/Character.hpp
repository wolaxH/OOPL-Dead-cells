#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Util/Animation.hpp"

#include "CollsionableObj.hpp"

#include <unordered_map>


enum class c_state{
    idle,
    L_move,
    R_move,
    jump,
    fall,
    atk
};

class Character : public CollsionableObj{
public:    
    Character(std::vector<std::string>& path, int Hp);
    ~Character() noexcept = default;

    void SetPos(glm::vec2 pos){ m_WorldPos = pos;}

    void virtual Update() = 0;

    void SetState(c_state State, std::vector<std::string> path = {}, bool loop = true);

    c_state GetState(){ return State;}

    bool IsCollsion(std::shared_ptr<CollsionableObj> other);

    void virtual Attack() = 0;

protected:
    bool IsContainState(c_state State){return (D_Manager.find(State) != D_Manager.end()) ? true : false;}

private:
    virtual void FixPos() = 0;

private:
    std::unordered_map<c_state, std::shared_ptr<Core::Drawable>> D_Manager;  //Drawable Manager
    c_state State;      //current stateq
    int Hp;
};

#endif