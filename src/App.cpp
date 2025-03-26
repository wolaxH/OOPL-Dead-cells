#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

std::vector<std::shared_ptr<SolidObj>> SolidObjs;
std::vector<std::shared_ptr<Mob>> Mobs;


void App::Start() {

    std::vector<std::string> Img;
    Img.reserve(46);
    for (int i = 0; i < 46; i++){
        Img.push_back(RESOURCE_DIR"/Beheaded/idle/idle_" + std::to_string(i) + ".png");
    }
    player = std::make_shared<Player>(Img, 100);
    player->SetPos({0, 0});
    player->SetZIndex(5);
    player->SetVisible(true);
    root.AddChild(player);
    camera.SetPos(player->m_WorldPos);

    Img.clear();
    for (int i = 0; i < 24; i++){
        Img.push_back(RESOURCE_DIR"/Zombie/idle/idle_" + std::to_string(i) + ".png");
    }
    /*last work in here, try to build a zombie and check the move function*/
    zombie = std::make_shared<Zombie>(Img, 100, player);
    zombie->SetPos({0, 0});
    zombie->SetZIndex(100);
    zombie->SetVisible(true);
    MapObjs.push_back(zombie);
    root.AddChild(zombie);



    ground = std::make_shared<SolidObj>(RESOURCE_DIR"/bg/green.png");
    MapObjs.push_back(ground);
    SolidObjs.push_back(ground);

    ground2 = std::make_shared<SolidObj>(RESOURCE_DIR"/bg/green.png");
    ground2->m_Transform.translation = {640.0f - camera.GetPos().x, 0.0f - camera.GetPos().y};
    ground2->m_WorldPos = ground2->m_Transform.translation;
    ground2->m_Transform.scale = {0.2f, 2.0f};
    MapObjs.push_back(ground2);
    SolidObjs.push_back(ground2);

    ground3 = std::make_shared<SolidObj>(RESOURCE_DIR"/bg/green.png");
    ground3->m_Transform.translation = {-640.0f - camera.GetPos().x, 0.0f - camera.GetPos().y};
    ground3->m_WorldPos = ground3->m_Transform.translation;
    ground3->m_Transform.scale = {0.2f, 2.0f};
    MapObjs.push_back(ground3);
    SolidObjs.push_back(ground3);

    std::vector<std::shared_ptr<Util::GameObject>> temps;
    for (auto& temp : SolidObjs){ temps.push_back(temp);}
    root.AddChildren(temps);
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    player->Update();
    camera.Update(player);
    for (auto& temp : MapObjs){
        temp->m_Transform.translation = temp->m_WorldPos - camera.GetPos();
    }
    zombie->Update();
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
