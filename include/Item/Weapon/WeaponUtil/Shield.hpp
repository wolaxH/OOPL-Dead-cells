#ifndef SHIELD_HPP
#define SHIELD_HPP

#include "Item/PickUp.hpp"
#include "MyUtil/Rect.hpp"

#include "Util/Animation.hpp"

class Mob;

class Shield : public Item{
public:

    enum class State{
        Block,
        BlockEnd
    };

    Shield(const std::string& path, const std::string& Describe)
        : Item(path){
            auto T = std::make_shared<Util::Text>(RESOURCE_DIR"/Cubic_11.ttf", 20, Describe, Util::Color(255, 255, 255));
            m_Describe = std::make_shared<Util::GameObject>(T, 15.0f);
            m_Describe->SetVisible(false);
    }
    virtual ~Shield() noexcept = default;

    std::shared_ptr<Util::Image> GetBlockDrawable() const noexcept {return m_BlockDrawable;}

    std::shared_ptr<Util::Animation> GetBlockEndDrawable() const noexcept {return m_BlockEndDrawable;}

    float GetDefense() const noexcept {return m_defense;}

    virtual void Parry(std::shared_ptr<Mob>& mob, const glm::vec2& Dir = {1, 1}) = 0;

    State GetState() const noexcept {return m_State;}

    void Block() noexcept {m_State = State::Block;}

    void BlockEnd() noexcept {m_State = State::BlockEnd;}

    Rect GetParryHitBox(const glm::vec2& Pos, const glm::vec2& Dir){
        Rect result;
        result.x = Pos.x, result.y = Pos.y + 30;
        result.width = 60;
        result.height = 60;
        result.x += Dir.x > 0 ? 30 : -30;
        result.width *= 2, result.height *= 2;
        return result;
    };

protected:
    int ParryDamage;
    float m_defense = 0.75f;
    std::shared_ptr<Util::Image> m_BlockDrawable;
    std::shared_ptr<Util::Animation> m_BlockEndDrawable;
    State m_State = State::Block;
};

#endif