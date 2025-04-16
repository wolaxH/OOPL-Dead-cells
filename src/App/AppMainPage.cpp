#include "App.hpp"

void App::MainPage(){

    if (WIPFlag){
        LOG_DEBUG("WIPFlag is true, WIP is visible");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        WIP->SetVisible(false);
        WIPFlag = false;
        LOG_DEBUG("WIPEND");
    }
    StartMenu->Update();
    root.Update();
    
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}