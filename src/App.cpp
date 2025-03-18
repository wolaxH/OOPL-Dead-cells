#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

std::vector<std::shared_ptr<CollsionableObj>> CollsionableObjs;
std::vector<std::shared_ptr<Character>> Mobs;

void App::Start() {

    std::vector<std::string> playerImg;
    playerImg.reserve(46);
    for (int i = 0; i < 46; i++){
        playerImg.push_back(RESOURCE_DIR"/Beheaded/idle/idle_" + std::to_string(i) + ".png");
    }
    player = std::make_shared<Player>(playerImg, 100);
    player->SetPos({0, 0});    
    player->SetZIndex(5);
    player->SetVisible(true);
    root.AddChild(player);
    camera.SetPos(player->m_WorldPos);

    ground = std::make_shared<Ground>(RESOURCE_DIR"/bg/green.png");
    CollsionableObjs.push_back(ground);

    ground2 = std::make_shared<Ground>(RESOURCE_DIR"/bg/green.png");
    ground2->m_Transform.translation = {640.0f - camera.GetPos().x, 0.0f - camera.GetPos().y};
    ground2->m_WorldPos = ground2->m_Transform.translation;
    ground2->m_Transform.scale = {0.2f, 2.0f};
    CollsionableObjs.push_back(ground2);

    ground3 = std::make_shared<Ground>(RESOURCE_DIR"/bg/green.png");
    ground3->m_Transform.translation = {-640.0f - camera.GetPos().x, 0.0f - camera.GetPos().y};
    ground3->m_WorldPos = ground3->m_Transform.translation;
    ground3->m_Transform.scale = {0.2f, 2.0f};
    CollsionableObjs.push_back(ground3);

    std::vector<std::shared_ptr<Util::GameObject>> temps;
    for (auto& temp : CollsionableObjs){ temps.push_back(temp);}
    root.AddChildren(temps);
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    player->Update();
    camera.Update(player);
    for (auto& temp : CollsionableObjs){
        temp->m_Transform.translation.x = temp->m_WorldPos.x - camera.GetPos().x;
        temp->m_Transform.translation.y = temp->m_WorldPos.y - camera.GetPos().y;
    }
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
