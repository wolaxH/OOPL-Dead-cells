#include "Character/Shooter.hpp"


Shooter::Shooter(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World) 
    : Mob(path, Hp, player, World){
    top = 50, bottom = 62, left = 13, right = 13;
    m_DetectRange = 700.0f;
    m_Transform.translation = {1.0f, 1.0f};
    m_AtkRange = 400.0f;
    m_AtkCoolDownTimer.SetTime(1000, 1000);
    m_AtkPoint = 20;
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

void Shooter::Attack(float dt){
    if (GetState() == c_state::atk){
        auto anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (anim && anim->GetCurrentFrameIndex() == FIRE_FRAME && !m_AtkFlag) {
            FireProjectile();
            m_AtkFlag = true;
            return;
        }
        if (anim && anim->GetCurrentFrameIndex() >= anim->GetFrameCount() - 1) {
            SetState(c_state::idle);
            m_state = mob_state::wander;
            m_AtkFlag = false;
            m_AtkCoolDownTimer.ResetTime(); //reset cooldown
            VelocityX = 0;
            return;
        }
    }

    LOG_DEBUG("Atk init");
    if (IsContainState(c_state::atk)) SetState(c_state::atk);
    else InitState(c_state::atk, {33}, {RESOURCE_DIR"/shooter/shoot/shoot_"});
    auto anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    if (anim) anim->Play();
}

void Shooter::FireProjectile(){
    int Dir = m_Transform.scale.x > 0 ? 1 : -1; //1 for right, -1 for left
    glm::vec2 bulletPos = m_WorldPos + glm::vec2(Dir * 50.0f, 0.f); //projectile spawn position

    auto projectile = std::make_shared<Projectile>(bulletPos, m_Transform.scale, 2.0f, 50,
        Projectile::Faction::Enemy, m_World, 50.0f, RESOURCE_DIR"/shooter/arrow.png");
    m_World.Projectiles->AddObj(projectile);
}

void Shooter::Move(float dt){
    if (GetState() == c_state::atk) return; //atk cannot move
    if (GetState() == c_state::atked) return;   //atked cannot move

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

void Shooter::LookAtPlayer(){
    float absScale = std::abs(m_Transform.scale.x);
    m_Transform.scale.x = (m_player->m_WorldPos.x < m_WorldPos.x) ? -absScale : absScale;
}

void Shooter::Update(float dt){
    InGround = Physics::IsOnGround(m_WorldPos, m_World.SolidObjs, m_World.OneSidedPlatforms);
    Physics::ApplyGravity(VelocityY, InGround, Gravity, MaxFallSpeed);
    if (InGround) Physics::SlowDown(VelocityX, Friction);

    if (!m_AtkCoolDownTimer.IsTimeout()){
        if (GetState() == c_state::atked) Attacked(0, glm::vec2(0, 0), 0.0f);
        m_AtkFlag = false;        // 允許 cooldown 結束後重新攻擊
        PushPlayer();
        FixPos(dt);
        m_WorldPos.y += VelocityY * dt;
        return;
    }

    if (IsSameLevelNearBy(m_player, m_DetectRange)){
        LookAtPlayer();
        m_state = mob_state::trace;
        if (IsSameLevelNearBy(m_player, m_AtkRange)){
            VelocityX = 0;
            Attack(dt);
        }
        else Move(dt);  //apporach player
    }
    else{   //wander
        m_state = mob_state::wander;
        Move(dt);
    }

    if (GetState() == c_state::atk) Attack(dt);
    if (GetState() == c_state::atked) Attacked(0, glm::vec2(0, 0), 0.0f);

    PushPlayer();
    FixPos(dt);

    m_WorldPos += glm::vec2(VelocityX, VelocityY) * dt;
}