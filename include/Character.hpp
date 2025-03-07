#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"

#include <unordered_map>


enum class c_state{
    idle,
    L_move,
    R_move,
    jump,
    fall,
    atk
};


class Character : public Util::GameObject{
public:    
    Character(std::vector<std::string>& path, int Hp);

    ~Character() noexcept = default;

    //void setPath(const std::string& Path){path = Path;}

    void SetPos(glm::vec2 pos){ m_Transform.translation = pos;}

    void virtual update() = 0;

    void SetState(c_state State, std::vector<std::string> path = {});

    c_state GetState(){ return State;}

    bool IsContainState(c_state State){return (D_Manager.find(State) != D_Manager.end()) ? true : false;}

    bool IsCollsion(std::shared_ptr<GameObject> other);

private:
    std::unordered_map<c_state, std::shared_ptr<Core::Drawable>> D_Manager;  //Drawable Manager
    c_state State; 
    int Hp;
};

#endif