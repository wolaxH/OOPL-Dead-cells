#include "App.hpp"

void App::InGameUpdate(float dt) {
    
    player->Update(dt);
    
    
    //mobs
    for (auto& temp : m_World.Mobs->GetObjs()){
        auto mob = std::dynamic_pointer_cast<Mob>(temp);
        mob->Update(dt);
    }
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
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}