#include "App.hpp"

void App::InGameUpdate(float dt) {
    
    player->Update(dt);
    
    camera.Update(player);
    for (auto& temp : MapObjs){
        temp->m_Transform.translation = temp->m_WorldPos - camera.GetPos();
    }
    zombie->Update(dt);
    
    root.Update();
    

    /**
     * @brief switch solid objects visable, press Q.
     */
    if (Util::Input::IsKeyDown(Util::Keycode::Q)){
        for (auto& temp : SolidObjs){
            temp->SetVisible(!temp->GetVisible());
        }
        for (auto& temp : OneSidedPlatforms){
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