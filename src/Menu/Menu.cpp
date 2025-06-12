#include "Menu/Menu.hpp"

Menu::Menu(const std::vector<std::shared_ptr<Button>>& buttons, const std::shared_ptr<Util::GameObject>& Bg)
     : Buttons(buttons), m_Bg(Bg){
        m_selectlight = std::make_unique<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/menu/selectbar.png"), 2.0f);
        //m_selectlight->m_Transform.translation = {-426.0f, 0.0f};
        AddChild(m_selectlight);
        m_Bg = std::make_shared<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/menu/menu_bg.png"), 1.0f);
        m_Bg->m_Transform.scale = {1/1.5f, 1/1.5f};

        AddChild(m_Bg);
        for (auto& button : buttons){
            AddChild(button);
        }
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