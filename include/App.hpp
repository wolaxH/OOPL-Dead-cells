#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "Util/Logger.hpp"


#include "Character/Player.hpp"
#include "SolidObj.hpp"
#include "MyUtil/Camera.hpp"
#include "Character/Zombie.hpp"
#include "Menu/Button.hpp"
#include "Menu/Menu.hpp"
#include "BackGround.hpp"
#include "OneSidedPlatform.hpp"
#include "UI/PlayerUI.hpp"


#include "json.hpp"

#include <fstream>
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

    void MainPageInit();
    void MainPage();    

    void InGameInit();   //into game
    void InGameUpdate(float dt);

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    template <typename T>
    void InitColliders(const std::string& fileName, std::vector<std::shared_ptr<T>>& container){
        std::string path;
        if constexpr (std::is_same_v<T, SolidObj>){
            path = RESOURCE_DIR"/bg/green.png";
        } else if constexpr (std::is_same_v<T, OneSidedPlatform>){
            path = RESOURCE_DIR"/bg/red.png";
        } else {
            throw std::runtime_error("Unsupported type for InitColliders");
        }

        std::ifstream file(std::string(DATA_DIR) + "/" + fileName);
        nlohmann::json j;
        file >> j;

        container.reserve(j.size());
        for (size_t i = 0; i < j.size(); i++){
            auto obj = std::make_shared<T>(path);
            container.push_back(obj);
            MapObjs.push_back(obj);
            obj->get_data_from_json(fileName, i);
            obj->SetVisible(false);
        } 
    }
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
    Camera camera;

    std::shared_ptr<Player> player;
    std::shared_ptr<Zombie> zombie;

    std::vector<std::shared_ptr<MapObj>> MapObjs;
    std::vector<std::shared_ptr<SolidObj>> SolidObjs;
    std::vector<std::shared_ptr<OneSidedPlatform>> OneSidedPlatforms;
    std::vector<std::shared_ptr<Mob>> Mobs;
    //這個GameObject 很特別，他被當作Drops 的容器使用，只會用GetChildren 跟 AddChild 跟 RemoveChild
    std::shared_ptr<Util::GameObject> WorldDrops;   
    
    std::vector<std::shared_ptr<BG>> BGs;
};

#endif
