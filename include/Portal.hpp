#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "Abstract/MapObj.hpp"
#include "Character/Player.hpp"

#include "Util/Text.hpp"

class Portal : public MapObj{
public:
    enum class Type{
        entrance,
        exit
    };

    Portal(std::shared_ptr<Player> player, glm::vec2 Pos, Type type);
    ~Portal() = default;

    void Interact();

    void Update();
    
private:
    void PopUpText();

    void Close();

private:
    Type m_Type;
    std::weak_ptr<Player> m_player;
    std::weak_ptr<Portal> m_PairPortal;
    std::shared_ptr<MapObj> m_Text;
    int m_Light;    
};

#endif
