#include "App.hpp"

#include "Core/Context.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;

    while (!context->GetExit()) {
        switch (app.GetCurrentState()) {
            case App::State::INIT_MENU:
                app.Init_Start();
                break;
            case App::State::MENU:
                app.Start();
                break;
            case App::State::INIT_UPDATE:
                app.Init_Update();
                break;
            case App::State::UPDATE:
                app.Update();
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
