#include "Menu/Menu.hpp"

#include "Util/Logger.hpp"

Menu::Menu( const std::shared_ptr<Util::GameObject>& SelectBar, 
            const std::shared_ptr<Util::GameObject>& Bg, 
            const std::vector<std::shared_ptr<Button>>& buttons,
            const glm::vec2& Offset)
        : Buttons(buttons), m_selectlight(SelectBar), m_Bg(Bg), m_Offset(Offset){
        // m_selectlight = std::make_unique<Util::GameObject>(std::make_shared<Util::Image>(RESOURCE_DIR"/menu/selectbar.png"), 2.0f);
        //m_selectlight->m_Transform.translation = {-426.0f, 0.0f};
        AddChild(m_selectlight);

        AddChild(m_Bg);
        for (auto bt : buttons){
            LOG_DEBUG("Bt Init");
            AddChild(bt);
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
    m_selectlight->m_Transform.translation = Buttons[m_CurrentButtonIndex]->m_Transform.translation + m_Offset;
}