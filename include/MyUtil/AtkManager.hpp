#ifndef ATKMANAGER_HPP
#define ATKMANAGER_HPP


#include "Item/Weapon/Weapon.hpp"

#include "Util/Animation.hpp"

class Player;


enum class AttackPhase {
    None,
    A,
    B,
    C,
    D,
    E,
    F
};

class AttackManager {
public:

    AttackManager() = default;
    AttackManager(std::weak_ptr<Player> player);
    ~AttackManager() = default;

    bool IsAttacking() const {return m_IsAttacking;}

    void Update(float dt);
    void StartAttack(int SlotNumber, std::shared_ptr<Weapon> weapon);
    void Interrupt();

private:
    int m_ComboIndex = 0;
    float m_ComboTimer = 0.0f;
    bool m_IsAttacking = false;
    int m_WeaponSlotNUmber = 0;
    std::shared_ptr<Weapon> m_Weapon = nullptr;
    std::weak_ptr<Player> m_Player;

    bool NextSegFlag = false;

    void ResetCombo();
};
    





#endif // ATKMANAGER_HPP