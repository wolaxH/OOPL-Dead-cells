#include "Menu.hpp"

Menu::Menu(const std::vector<std::shared_ptr<Button>>& buttons) : Buttons(buttons) {
    m_selectlight = std::make_unique<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/menu/selectbar.png"), 2.0f);
    //m_selectlight->m_Transform.translation = {-426.0f, 0.0f};
    AddChild(m_selectlight);
}

void Menu::Update(){
    if (Util::Input::IsKeyDown(Util::Keycode::UP)){
        if (m_CurrentButtonIndex == 0) {
            m_CurrentButtonIndex = Buttons.size() - 1;  // 跳轉到最後一個按鈕
        } else {
            m_CurrentButtonIndex--;
        }
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::DOWN)){
        m_CurrentButtonIndex = (m_CurrentButtonIndex + 1) % Buttons.size();
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::RETURN)){
        Buttons[m_CurrentButtonIndex]->Click();
    }
    m_selectlight->m_Transform.translation = Buttons[m_CurrentButtonIndex]->m_Transform.translation + glm::vec2(125, 21);
}