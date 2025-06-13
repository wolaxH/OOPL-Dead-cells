#include "App.hpp"

#include "Core/Context.hpp"

#include <chrono>

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;

    auto previousTime = std::chrono::high_resolution_clock::now();


    while (!context->GetExit()) {

        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - previousTime;
        previousTime = currentTime;
        float deltaTime = std::min(elapsed.count(), 0.05f) * 60.0f;

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
                app.InGameUpdate(deltaTime);
                break;
            case App::State::INIT_RETRY_MENU:
                app.RetryMenuInit();
                break;
            case App::State::RETRY_MENU:
                app.RetryMenu();
                break;
            case App::State::INIT_RETRY:
                app.RetryWorldInit();
                break;
            case App::State::SUMMARY_INIT:
                app.SummaryInit();
                break;
            case App::State::SUMMARY:
                app.Summary();
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
