#ifndef MENU_HPP
#define MENU_HPP

#include "Button.hpp"

#include "Util/GameObject.hpp"
#include "Util/Input.hpp"
#include "Util/Image.hpp"

#include <memory>
#include <vector>

class Menu : public Util::GameObject {
public:
    Menu(const std::vector<std::shared_ptr<Button>>& buttons = {});

    void AddButton(const std::shared_ptr<Button>& button) {Buttons.push_back(button);}

    // void RemoveButton(const std::string& label) {
    //     Buttons.erase(std::remove_if(Buttons.begin(), Buttons.end(),
    //                                   [&label](const Button& button) {
    //                                       return button.GetLabel() == label;
    //                                   }),
    //                    Buttons.end());
    // }

    // void ClickButton(const std::string& label) {
    //     for (auto& button : Buttons) {
    //         if (button.GetLabel() == label) {
    //             button.Click();
    //             break;
    //         }
    //     }
    // }

    void Update();

private:
    std::vector<std::shared_ptr<Button>> Buttons;
    int m_CurrentButtonIndex = 0; // Index of the currently selected button
    std::shared_ptr<Util::GameObject> m_selectlight;
};

#endif