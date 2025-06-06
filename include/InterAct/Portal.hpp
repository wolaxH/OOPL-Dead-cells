#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "Abstract/MapObj.hpp"
#include "Character/Player.hpp"
#include "InterAct/IInterAct.hpp"

#include "Util/Text.hpp"

class Portal : public MapObj, public IInterAct{
public:
    enum class Type{
        entrance,
        exit
    };

    Portal(std::shared_ptr<Player> player, glm::vec2 Pos, Type type);
    ~Portal() = default;

    void InterAct() override;

    void Update() override;
    
    static void PairingPortal();

    void SetPair(std::shared_ptr<Portal> other){
        if (other->GetType() == m_Type) return;
        m_PairPortal = other;
    }

    Type GetType() const noexcept {return m_Type;}
    
private:
    void PopUpText();

    void Close();

private:
    Type m_Type;
    std::weak_ptr<Player> m_player;
    std::weak_ptr<Portal> m_PairPortal;
    std::shared_ptr<MapObj> m_Text;
    int m_Light;    
    std::shared_ptr<Util::Text> m_TextDrawable;
};

#endif
