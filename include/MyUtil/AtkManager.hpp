#ifndef ATKMANAGER_HPP
#define ATKMANAGER_HPP

enum class AttackPhase {
    None,
    AttackA,
    AttackB,
    AttackC
};

class AttackManager {
public:
    AttackManager();

    void Update(float deltaTime);
    void OnAttackInput();
    void InterruptAttack();

private:
    AttackPhase currentPhase;
    float attackTimer;
    bool canChain;

    void StartAttack(AttackPhase phase);
    void EndAttack();
};
    





#endif // ATKMANAGER_HPP