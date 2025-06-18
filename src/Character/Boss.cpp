#include "Character/Boss.hpp"


Boss::Boss(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World)
    : Mob(path, Hp, player, World) {


    m_DetectRange = 2500.f;
    m_AtkRange = 300.f;
    m_Transform.scale = {1.0f, 1.0f};

    std::vector<std::string> paths;
    for (int i = 0; i < 22; i++){
        paths.push_back(RESOURCE_DIR"/Boss/attack1/atk1_" + std::to_string(i) + ".png");
    }
    auto anim = std::make_shared<Util::Animation>(paths, true, 50, false);
    m_AtkManager[1] = {640.0f, 60, anim, 98.f, {11, 12}};
    paths.clear();
    for (int i = 0; i < 30; i++){
        paths.push_back(RESOURCE_DIR"/Boss/attack2/atk2_" + std::to_string(i) + ".png");
    }
    anim = std::make_shared<Util::Animation>(paths, true, 50, false);
    m_AtkManager[2] = {900.0f, 70, anim, 70.f, {15, 16, 17}};

    m_AtkCoolDownTimer.SetTime(300, 1000);
    
    top = 150 * m_Transform.scale.y;
    bottom = 150 * m_Transform.scale.y;
    left =  90 * m_Transform.scale.x;
    right = 90 * m_Transform.scale.x;
    
    m_score = 100;
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
            ChangeDrawable(AccessKey(), m_AtkManager[m_CurrentAtkID].anim, c_state::atk);
        }
        else InitState(c_state::atk, m_AtkManager[m_CurrentAtkID].anim);
        
        auto anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (anim) anim->Play();
        m_AtkFlag = false;
        m_WorldPos.x += m_Transform.scale.x > 0 ? 50 : -50;
        return;
    }

    auto anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    if (anim) {
        auto CurrentFrame = anim->GetCurrentFrameIndex();
        bool IsAtkable = std::find(
                        m_AtkManager[m_CurrentAtkID].AtkableFrame.begin(), 
                        m_AtkManager[m_CurrentAtkID].AtkableFrame.end(), 
                        CurrentFrame) 
                    != m_AtkManager[m_CurrentAtkID].AtkableFrame.end();

        if (IsAtkable && !m_AtkFlag){
            Rect HitBox = Rect::CreateRect(m_WorldPos + glm::vec2(m_AtkManager[m_CurrentAtkID].Offset * m_Transform.scale.x, -100)
            ,  m_AtkManager[m_CurrentAtkID].AtkRange, 100);
            Rect playerHitBox = Rect::CreateRect(m_player->m_WorldPos, 
                m_player->left + m_player->right, m_player->top + m_player->bottom);

            if (HitBox.Intersects(playerHitBox)) {
                m_player->Attacked(m_AtkManager[m_CurrentAtkID].AtkPoint, m_Transform.scale, 15.0f);
                m_AtkFlag = true;
            }
        }
        if (anim->GetCurrentFrameIndex() >= anim->GetFrameCount() - 1) {
            SetState(c_state::idle);
            m_AtkCoolDownTimer.ResetTime();
            m_AtkFlag = false;
            m_WorldPos.x -= m_Transform.scale.x > 0 ? 50 : -50;
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

    if (IsNearBy(m_player, m_DetectRange)) m_HpUI->Pop();
    else m_HpUI->Hide();

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
        VelocityX = 0;
        VelocityY = 0;
    }
    else if (GetState() == c_state::atked){
        Attacked(0, glm::vec2(0, 0), 0.0f);
    }
    else{
        if (IsNearBy(m_player, m_DetectRange)) {
            m_Visible = true;
            LookAtPlayer();
            m_state = mob_state::trace;

            if (IsNearBy(m_player, m_AtkRange)) {
                VelocityX = 0;
                std::random_device rd;         // 隨機種子
                std::mt19937 gen(rd());        // 梅森旋轉演算法的亂數產生器
                std::uniform_int_distribution<> dis(1, 2); // 產生 1 或 2 的均勻分布
                LOG_DEBUG("Attack");

                m_CurrentAtkID = dis(gen);
                Attack(dt);  // 初始化攻擊
                VelocityY = 0;
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