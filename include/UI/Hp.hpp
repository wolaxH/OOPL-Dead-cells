#ifndef HP_HPP
#define HP_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Util/Renderer.hpp"

class Hp : public Util::GameObject{
public:
    Hp();
    ~Hp() = default;

    void SetHp(int hp);

    int GetCurrentHp() const {return CurrentHp;}

private:
    std::shared_ptr<Util::GameObject> HpBox;
    std::shared_ptr<Util::GameObject> HpBar;    // the green bar
    std::shared_ptr<Util::GameObject> Point;   //text eg: 100/170
    std::shared_ptr<Util::Text> text;
    int MaxHp = 200, CurrentHp = 200;
};


#endif