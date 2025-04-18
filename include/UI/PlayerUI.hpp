#ifndef PLAYERUI_HPP
#define PLAYERUI_HPP

#include "Util/GameObject.hpp"

#include "UI/Hp.hpp"



class PlayerUI : public Util::GameObject{
public:
    PlayerUI(){
        hp = Hp();
    }
    ~PlayerUI() = default;

    void AddToRender(Util::Renderer& root){
        hp.AddToRender(root);
    }
    void SetHp(int newhp){
        hp.SetHp(newhp);
    }

private:
    Hp hp;
    
};

#endif