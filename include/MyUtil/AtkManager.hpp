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

struct AttackData {
    std::vector<size_t> AttackableFrams;
    bool HitableFlag = false;
};


/**TODO:
 * Weapon 有接口可以獲得該段攻擊中可造成傷害的frams(vector)
 * AtkUpdate 當到達特定fram 時call weapon->Use()
 */

class AttackManager {
public:

    AttackManager() = default;
    AttackManager(std::weak_ptr<Player> player);
    ~AttackManager() = default;

    bool IsAttacking() const {return m_IsAttacking;}

    void Update(float dt);
    void StartAttack(int SlotNumber, std::shared_ptr<Weapon> weapon);
    void Interrupt();
    int GetComboIndex() noexcept { return m_ComboIndex;}

    bool GetHitable() const noexcept { return m_CurrentAtkData.HitableFlag;}

    std::shared_ptr<Weapon> GetCurrentWeapon() const {return m_Weapon;}

private:
    int m_ComboIndex = 0;
    float m_ComboTimer = 0.0f;
    bool m_NextSegFlag = false;

    bool m_IsAttacking = false; //是否正在攻擊
    int m_WeaponSlotNUmber = 0;
    std::shared_ptr<Weapon> m_Weapon = nullptr;
    std::weak_ptr<Player> m_Player;

    AttackData m_CurrentAtkData;

    void ResetCombo();
};

#endif // ATKMANAGER_HPP