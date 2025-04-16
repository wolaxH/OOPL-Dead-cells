#include "App.hpp"

#include "Core/Context.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;

    while (!context->GetExit()) {
        switch (app.GetCurrentState()) {
            case App::State::INIT_MENU:
                app.MainPageInit();
                break;
            case App::State::MENU:
                app.MainPage();
                break;
            case App::State::INIT_UPDATE:
                app.InGameInit();
                break;
            case App::State::UPDATE:
                app.InGameUpdate();
                break;

            case App::State::END:
                app.End();
                context->SetExit(true);
                break;
        }
        context->Update();
    }
    return 0;
}
