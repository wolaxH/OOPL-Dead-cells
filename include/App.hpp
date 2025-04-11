#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"


#include "Player.hpp"
#include "SolidObj.hpp"
#include "Camera.hpp"
#include "MapObj.hpp"
#include "Zombie.hpp"
#include "Button.hpp"
#include "Menu.hpp"
#include "BackGround.hpp"


#include <thread>
#include <chrono>

class App {
public:
    enum class State {
        INIT_MENU,
        MENU,
        INIT_UPDATE,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Init_Start();
    void Start();    

    void Init_Update();   //into game
    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:    //App Objs
    State m_CurrentState = State::INIT_MENU;
    Util::Renderer root;
    std::shared_ptr<Util::GameObject> WIP;
    bool WIPFlag = false;

private:    //Menu Objs
    std::shared_ptr<Util::GameObject> Menu_Bg;
    std::shared_ptr<Menu> StartMenu;
    std::shared_ptr<Button> PlayButton;
    std::shared_ptr<Button> OptionButton;
    std::shared_ptr<Button> ExitButton;

private:    //ingame Objs
    std::shared_ptr<Player> player;
    std::shared_ptr<SolidObj> ground, ground2, ground3;
    std::shared_ptr<Zombie> zombie;
    std::vector<std::shared_ptr<MapObj>> MapObjs;
    Camera camera;
    std::vector<std::shared_ptr<SolidObj>> SolidObjs;
    std::vector<std::shared_ptr<Mob>> Mobs;
    
    std::shared_ptr<BG> bg1, bg2, bg3;
    std::vector<std::shared_ptr<BG>> BGs;
};

#endif
