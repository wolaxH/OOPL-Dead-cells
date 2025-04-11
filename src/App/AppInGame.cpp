#include "App.hpp"

void App::InGameUpdate() {
    
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