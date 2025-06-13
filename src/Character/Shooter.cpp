#include "Character/Shooter.hpp"


Shooter::Shooter(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World) 
    : Mob(path, Hp, player, World){
    top = 35, bottom = 84, left = 74, right = 27;
    m_DetectRange = 700.0f;
    m_Transform.translation = {1.0f, 1.0f};
    m_AtkRange = 400.0f;
    m_AtkCoolDownTimer.SetTime(1000, 1000);
    m_AtkPoint = 20;

    m_HpUI = std::make_shared<EnemyHP>(Hp, Hp, m_WorldPos);
    AddChild(m_HpUI);
    m_World.AddObj(m_HpUI);

    LOG_DEBUG("Shooter init");
}

Shooter::~Shooter(){
    LOG_DEBUG("Deleted Shooter");
    m_World.RemoveObj(m_HpUI);
}

void Shooter::Attacked(int Damage, glm::vec2 Dir, float Velocity){
    if (GetState() == c_state::atk){
        m_Hp -= Damage;
        return;
    }

    if (GetState() == c_state::atked){
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (temp == nullptr) return;
        if (temp->GetCurrentFrameIndex() >= temp->GetFrameCount()-1){
            temp->SetCurrentFrame(0);
            SetState(c_state::idle);
        }
        return;
    }
    LOG_DEBUG(m_Hp);

    if (IsContainState(c_state::atked)) SetState(c_state::atked);
    else InitState(c_state::atked, {4}, {RESOURCE_DIR"/shooter/Atked/Atked_"});
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->Play();
    m_Hp -= Damage;
    VelocityX += Velocity * (Dir.x > 0) ? 1 : -1;
}

void Shooter::Attack(float dt) {
    auto anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);

    // 第一次進入攻擊狀態
    if (GetState() != c_state::atk) {
        if (IsContainState(c_state::atk)) SetState(c_state::atk);
        else InitState(c_state::atk, {33}, {RESOURCE_DIR"/shooter/shoot/shoot_"});

        anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (anim) anim->Play();
        m_AtkFlag = false; // 準備等待開火幀
        return;
    }

    // 進入這邊表示正在播放 atk 動畫
    if (anim) {
        // 在指定幀開火
        if (anim->GetCurrentFrameIndex() == FIRE_FRAME && !m_AtkFlag) {
            FireProjectile();
            m_AtkFlag = true;
        }

        // 播完動畫後恢復 idle 並設 cooldown
        if (anim->GetCurrentFrameIndex() >= anim->GetFrameCount() - 1) {
            SetState(c_state::idle);
            m_state = mob_state::wander;
            VelocityX = 0;
            m_AtkCoolDownTimer.ResetTime();  // reset cooldown
            m_AtkFlag = false;
        }
    }
}

void Shooter::FireProjectile(){
    int Dir = m_Transform.scale.x > 0 ? 1 : -1; //1 for right, -1 for left
    glm::vec2 bulletPos = m_WorldPos + glm::vec2(Dir * 50.0f, 0.f); //projectile spawn position

    auto projectile = std::make_shared<Projectile>(bulletPos, m_Transform.scale, 20.0f, 50,
        Projectile::Faction::Enemy, m_World, 3.0f, RESOURCE_DIR"/shooter/arrow.png");
    m_World.Projectiles->AddObj(projectile);
}

void Shooter::Move(float dt){
    if (GetState() == c_state::atk || //atk cannot move
        GetState() == c_state::atked) return;   //atked cannot move

    if (m_state == mob_state::wander){
        Wander(RESOURCE_DIR"/shooter/move/move_", 15);
    }
    else if (m_state == mob_state::trace){
        c_state moveState = (m_Transform.scale.x > 0) ? c_state::R_move : c_state::L_move;
        if (IsContainState(moveState)) SetState(moveState);
        else InitState(moveState, {15}, {RESOURCE_DIR"/shooter/move/move_"}); //init move state

        VelocityX += (moveState == c_state::R_move) ? AccelerationX * dt : -AccelerationX * dt;
        if (std::abs(VelocityX) > MaxSpeed) VelocityX = VelocityX > 0 ? MaxSpeed : -MaxSpeed;
    }
}

void Shooter::Update(float dt) {
    // 地面與物理處理
    InGround = Physics::IsOnGround(this, m_World.SolidObjs, m_World.OneSidedPlatforms);
    Physics::ApplyGravity(VelocityY, InGround, Gravity, MaxFallSpeed);
    if (InGround) Physics::SlowDown(VelocityX, Friction);

    // 當前正在播放攻擊動畫
    if (GetState() == c_state::atk) {
        Attack(dt);  // 繼續動畫與判斷射擊時機
    }
    // 當前正在播放受傷動畫
    else if (GetState() == c_state::atked) {
        Attacked(0, glm::vec2(0, 0), 0.0f);  // 用來檢查動畫是否結束
    }
    else {
        // 攻擊冷卻完畢才會進行判斷與攻擊
        if (IsSameLevelNearBy(m_player, m_DetectRange)) {
            LookAtPlayer();
            m_state = mob_state::trace;

            if (IsSameLevelNearBy(m_player, m_AtkRange)) {
                VelocityX = 0;
                if (m_AtkCoolDownTimer.IsTimeout()) {
                    Attack(dt);  // 初始化攻擊
                }
            }
            else Move(dt);  // 追蹤玩家
        }
        else {
            m_state = mob_state::wander;
            Move(dt);  // 隨機漫遊
        }
    }

    // 移動與交互處理
    PushPlayer();
    FixPos(dt);
    m_WorldPos += glm::vec2(VelocityX, VelocityY) * dt;
    m_HpUI->Update(m_Hp);
}
