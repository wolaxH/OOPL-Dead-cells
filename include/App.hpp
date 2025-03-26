#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"

#include "Player.hpp"
#include "SolidObj.hpp"
#include "Camera.hpp"
#include "MapObj.hpp"
#include "Zombie.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;
    Util::Renderer root;
    std::shared_ptr<Player> player;
    std::shared_ptr<SolidObj> ground, ground2, ground3;
    std::shared_ptr<Zombie> zombie;
    std::vector<std::shared_ptr<MapObj>> MapObjs;
    Camera camera;
};

#endif
