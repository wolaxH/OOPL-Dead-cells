#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

std::vector<std::shared_ptr<Ground>> grounds;
std::vector<std::shared_ptr<Util::GameObject>> Collsionable;

void App::Start() {

    std::vector<std::string> playerImg;
    playerImg.reserve(46);
    for (int i = 0; i < 46; i++){
        playerImg.push_back(RESOURCE_DIR"/Beheaded/idle/idle_" + std::to_string(i) + ".png");
    }
    player = std::make_shared<Player>(playerImg, 100);
    player->SetPos({0, 0});    
    /*遊戲pos 設定為225px/2 = 30gamepos ; 1gamepos = 3.75px*/
    player->SetZIndex(5);
    player->SetVisible(true);
    root.AddChild(player);

    ground = std::make_shared<Ground>(RESOURCE_DIR"/bg/green.png");
    grounds.push_back(ground);

    ground2 = std::make_shared<Ground>(RESOURCE_DIR"/bg/green.png");
    ground2->m_Transform.translation = {640.0f, 0.0f};
    ground2->m_Transform.scale = {0.2f, 2.0f};
    grounds.push_back(ground2);

    ground3 = std::make_shared<Ground>(RESOURCE_DIR"/bg/green.png");
    ground3->m_Transform.translation = {-640.0f, 0.0f};
    ground3->m_Transform.scale = {0.2f, 2.0f};
    grounds.push_back(ground3);

    root.AddChild(ground);
    root.AddChild(ground2);
    root.AddChild(ground3);
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    //TODO: do your things here and delete this line <3
    player->Update(ground);
    root.Update();
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
