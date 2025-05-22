#include "MyUtil/AtkManager.hpp"

#include "Util/Input.hpp"

#include "Character/Player.hpp"

#include "Util/Logger.hpp"


AttackManager::AttackManager(std::weak_ptr<Player> player){
    m_Player = player;
}

void AttackManager::StartAttack(int SlotNumber, std::shared_ptr<Weapon> weapon){
    m_WeaponSlotNUmber = SlotNumber;
    m_Weapon = weapon;
    m_ComboIndex = 0;
    m_ComboTimer = 0.0f;
    m_IsAttacking = true;
    m_Player.lock()->RequastToChangeDrawable(m_Weapon->GetPlayerDrawable()[0]);

    m_CurrentAtkData.AttackableFrams = weapon->GetAtkableFrames(0);
    m_CurrentAtkData.AtkTimes = weapon->GetAtkTimes()[0];
    m_CurrentAtkData.HasAtkTimes = 0;
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)) m_Player.lock()->VelocityX += 5;
    else if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)  || Util::Input::IsKeyPressed(Util::Keycode::A)) m_Player.lock()->VelocityX -= 5;
}

void AttackManager::Interrupt(){

}

void AttackManager::Update(float dt) {
    if (!m_IsAttacking) return;

    auto player = m_Player.lock();
    if (!player) return;

    auto currentAnim = std::dynamic_pointer_cast<Util::Animation>(player->m_Drawable);
    if (!currentAnim) return;

    Util::Keycode atkKey = (m_WeaponSlotNUmber == 0) ? Util::Keycode::J : Util::Keycode::K;


    auto currentFrame = currentAnim->GetCurrentFrameIndex();
    const auto& atkableFrames = m_CurrentAtkData.AttackableFrams;
    //如果找到
    if (std::find(atkableFrames.begin(), atkableFrames.end(), currentFrame) != atkableFrames.end()){
        m_CurrentAtkData.HitableFlag = true;
    }
    else{
        m_CurrentAtkData.HitableFlag = false;
    }

    if (currentFrame > currentAnim->GetFrameCount() - 6 && Util::Input::IsKeyDown(atkKey)) m_NextSegFlag = true;

    // 檢查當前動畫是否播放完
    if (currentFrame < currentAnim->GetFrameCount() - 1) return;

    m_ComboTimer += dt;

    // 是否還有下一段攻擊動畫
    bool hasNextCombo = (m_ComboIndex + 1 < m_Weapon->GetSegCount());
        
    // 若在攻擊動畫結束時按下按鍵 → 進入下一段
    if (m_NextSegFlag && hasNextCombo) {
        m_ComboIndex++;
        m_ComboTimer = 0.f;
        m_NextSegFlag = false;

        m_CurrentAtkData.AttackableFrams = m_Weapon->GetAtkableFrames(m_ComboIndex);
        m_CurrentAtkData.HitableFlag = false;
        m_CurrentAtkData.AtkTimes = m_Weapon->GetAtkTimes()[m_ComboIndex];
        m_CurrentAtkData.HasAtkTimes = 0;

        auto nextAnim = m_Weapon->GetPlayerDrawable()[m_ComboIndex];
        nextAnim->SetCurrentFrame(0);
        player->RequastToChangeDrawable(nextAnim);

        // 小幅位移（可選擇移到 Use() 處理）
        glm::vec2 Dir = player->m_Transform.scale;
        if (Dir.x > 0) player->VelocityX += 10;
        else if (Dir.x < 0) player->VelocityX -= 10;
    }
    else if (!hasNextCombo || m_ComboTimer >= 1.8f) {
        ResetCombo();
    }
}

void AttackManager::ResetCombo() {
    m_ComboIndex = 0;
    m_ComboTimer = 0.f;
    m_IsAttacking = false;
    if (auto p = m_Player.lock()) {
        p->SetState(c_state::idle);
    }
    m_Weapon.reset();
}
