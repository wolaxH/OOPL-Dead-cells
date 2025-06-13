#include "App.hpp"

void App::InGameUpdate(float dt) {

    if (!player->GetHasRebirth() || player->GetState() == c_state::rebirth){
        player->Rebrith();
        //camera    
        camera.Update(player);
        for (auto& temp : MapObjs){
            temp->m_Transform.translation = temp->m_WorldPos - camera.GetPos();
        }
        root.Update();
        return;
    }

    player->Update(dt);

    //projectiles
    for (auto& temp : m_World.Projectiles->GetObjs()){
        auto projectile = std::dynamic_pointer_cast<Projectile>(temp);
        if (projectile) projectile->Update(dt);
    }

    m_World.Projectiles->RemoveObjs([](const std::shared_ptr<Util::GameObject>& temp){
        auto projectile = std::dynamic_pointer_cast<Projectile>(temp);
        return projectile && projectile->IsDestroyed();
    });

    //InterActs
    for (auto& temp : m_World.InterActAbles){
        if (temp) temp->Update();
    }
        
    //mobs
    for (auto& temp : m_World.Mobs->GetObjs()){
        auto mob = std::dynamic_pointer_cast<Mob>(temp);
        mob->Update(dt);
        if (!mob->IsAlive()) player->AddScore(mob->GetScore());
    }
    m_World.Mobs->RemoveObjs([](const std::shared_ptr<Util::GameObject>& temp){
        auto mob = std::dynamic_pointer_cast<Mob>(temp);
        return mob && !mob->IsAlive();
    });    

    //Drops
    for (auto& temp : m_World.WorldDrops->GetObjs()){
        auto drop = std::dynamic_pointer_cast<Drops>(temp);
        drop->Update();
    }

    //camera    
    camera.Update(player);
    for (auto& temp : MapObjs){
        temp->m_Transform.translation = temp->m_WorldPos - camera.GetPos();
    }
    
    //render
    root.Update();

    

    /**
     * @brief switch solid objects visable, press Q.
     */
    if (Util::Input::IsKeyDown(Util::Keycode::Q)){
        for (auto& temp : m_World.SolidObjs){
            temp->SetVisible(!temp->GetVisible());
        }
        for (auto& temp : m_World.OneSidedPlatforms){
            temp->SetVisible(!temp->GetVisible());
        }
    }

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (!player->IsAlive()) {
        if (player->GetState() != c_state::death) {
            player->Die(); // 初始化死亡動畫
        }
        else if (player->m_DeathTimer.IsTimeout()){
            m_CurrentState = State::INIT_RETRY_MENU;
        }
    }
    else if (!m_Boss->IsAlive()){
        m_CurrentState = State::SUMMARY_INIT;
    }
}

