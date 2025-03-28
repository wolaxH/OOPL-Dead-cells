#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

#include <functional>

class Button : public Util::GameObject{
public:
    Button(const std::shared_ptr<Util::Text>& Text_drawable, float z_index, 
           std::string text, std::function<void()> action)
            : GameObject(Text_drawable, z_index), Label(text), OnClick(action) {}

            
    std::string GetLabel() const { return Label; }

    void Click(){ if (OnClick) OnClick();}
    
private:
    std::string Label;
    std::function<void()> OnClick;
};


#endif
