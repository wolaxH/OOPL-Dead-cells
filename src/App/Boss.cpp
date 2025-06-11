#include "Character/Boss.hpp"

/**
 * TODO:
 * *加入attack2(斬擊)
 * *根據大小調整m_Tranform.scale
 * *    攻擊碰撞箱大小
 * *    m_AtkInfo, top, bottom, left, right
 * *    
 */

Boss::Boss(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World)
    : Mob(path, Hp, player, World) {

    /**
     * m_AtkPoint, m_AtkRange should be Correction by the second seg atk
     */
    m_DetectRange = 2500.f;
    m_Transform.scale = {1.0f, 1.0f};
    m_AtkInfo.push_back({680.0f, 60, 22, RESOURCE_DIR"/Boss/attack1/atk1_"});
    m_AtkInfo.push_back({1.0f, 1, 1, RESOURCE_DIR"/Boss/attack2/atk2_"});
    m_AtkCoolDownTimer.SetTime(700, 1200);
    
    top = 50 * m_Transform.scale.y;
    bottom = 50 * m_Transform.scale.y;
    left =  50 * m_Transform.scale.x;
    right = 50 * m_Transform.scale.x;
    
    m_HpUI = std::make_shared<BossHp>(m_Hp);
    AddChild(m_HpUI);
}

void Boss::Attacked(int Damage, glm::vec2 Dir, float Velocity){
    m_Hp -= Damage;
}

void Boss::Attack(float dt){
    if (GetState() != c_state::atk) {
        if (IsContainState(c_state::atk)){
            SetState(c_state::atk);
            std::vector<std::string> paths;
            for (size_t i = 0 ; i < m_AtkInfo[m_CurrentAtkID].FramesCount; i++){
                paths.push_back(m_AtkInfo[m_CurrentAtkID].Path + std::to_string(i) + ".png");
            }
            auto drawable = std::make_shared<Util::Animation>(paths, true, 20, false, 0);
            ChangeDrawable(AccessKey(), drawable, c_state::atk);
        }
        else InitState(c_state::atk, {m_AtkInfo[m_CurrentAtkID].FramesCount}, {m_AtkInfo[m_CurrentAtkID].Path});
        
        auto anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (anim) anim->Play();
        m_AtkFlag = false;
        return;
    }

    auto anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    if (anim) {
        auto CurrentFrame = anim->GetCurrentFrameIndex();
        if (CurrentFrame >= 10 && CurrentFrame <= 15 && !m_AtkFlag) {  // 第 10 ~ 15 幀造成傷害
            
            Rect HitBox = Rect::CreateRect(m_WorldPos + glm::vec2(100 * m_Transform.scale.x, 0), 200, 100);
            Rect playerHitBox = Rect::CreateRect(m_player->m_WorldPos, 
                m_player->left + m_player->right, m_player->top + m_player->bottom);

            if (HitBox.Intersects(playerHitBox)) {
                m_player->Attacked(m_AtkInfo[m_CurrentAtkID].AtkPoint, m_Transform.scale, 15.0f);
                m_AtkFlag = true;
            }
        }
        if (anim->GetCurrentFrameIndex() >= anim->GetFrameCount() - 1) {
            SetState(c_state::idle);
            m_AtkCoolDownTimer.ResetTime();
            m_AtkFlag = false;
        }
    }
}

void Boss::Move(float dt){
    if (GetState() == c_state::atk) return; //atk cannot move
    if (GetState() == c_state::atked) return;   //atked cannot move

    //trace player
    if (IsNearBy(m_player, m_DetectRange)){  
        //c_state = move
        m_state = mob_state::trace;

        c_state moveState = (m_Transform.scale.x > 0) ? c_state::R_move : c_state::L_move;
        if (IsContainState(moveState)) SetState(moveState);
        else InitState(moveState, {28}, {RESOURCE_DIR"/Boss/move/move_"}); //init move state

        VelocityX += (moveState == c_state::R_move) ? AccelerationX * dt : -AccelerationX * dt;
        if (std::abs(VelocityX) > MaxSpeed) VelocityX = VelocityX > 0 ? MaxSpeed : -MaxSpeed;
    }
    else{   //遊蕩狀態
        Wander(RESOURCE_DIR"/Zombie/move/move_", 22);
    }
}

void Boss::Update(float dt){
    InGround = Physics::IsOnGround(this, m_World.SolidObjs, m_World.OneSidedPlatforms);
    Physics::ApplyGravity(VelocityY, InGround, Gravity, MaxFallSpeed);
    if (InGround) Physics::SlowDown(VelocityX, Friction);

    if (!m_AtkCoolDownTimer.IsTimeout()){
        if (GetState() == c_state::atked) Attacked(0, glm::vec2(0, 0), 0.0f);
        else SetState(c_state::idle);  // 在 cooldown 時完全 idle
        m_AtkFlag = false;        // 允許 cooldown 結束後重新攻擊
        PushPlayer();
        FixPos(dt);
        m_WorldPos += glm::vec2(VelocityX, VelocityY) * dt;
        return;
    }

    // atk behavior
    if (GetState() == c_state::atk){
        Attack(dt);
    }
    else if (GetState() == c_state::atked){
        Attacked(0, glm::vec2(0, 0), 0.0f);
    }
    else{
        if (IsNearBy(m_player, m_DetectRange)) {
            LookAtPlayer();
            m_state = mob_state::trace;

            if (IsNearBy(m_player, m_AtkRange)) {
                VelocityX = 0;
                std::random_device rd;         // 隨機種子
                std::mt19937 gen(rd());        // 梅森旋轉演算法的亂數產生器
                std::uniform_int_distribution<> dis(0, 1); // 產生 0 或 1 的均勻分布

                m_CurrentAtkID = dis(gen);
                Attack(dt);  // 初始化攻擊
            }
            else Move(dt);  // 追蹤玩家
        }
        else{
            SetState(c_state::idle);
        }
        //不會Wander
    }
    
    PushPlayer();
    FixPos(dt);

    m_WorldPos += glm::vec2(VelocityX, VelocityY) * dt;
    m_HpUI->Update(m_Hp);
}