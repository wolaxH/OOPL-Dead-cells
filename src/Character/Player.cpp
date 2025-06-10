#include "Character/Player.hpp"
#include "Character/Mob.hpp"

#include "Util/Input.hpp"

#include "Util/Logger.hpp"


#define IS_LEFT_PRESSED()   Util::Input::IsKeyPressed(Util::Keycode::LEFT)  || Util::Input::IsKeyPressed(Util::Keycode::A)
#define IS_RIGHT_PRESSED()  Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)
#define IS_DOWN_PRESSED()   Util::Input::IsKeyPressed(Util::Keycode::DOWN)  || Util::Input::IsKeyPressed(Util::Keycode::S)
#define IS_UP_DOWN()        Util::Input::IsKeyDown(Util::Keycode::UP)       || Util::Input::IsKeyDown(Util::Keycode::W) || Util::Input::IsKeyDown(Util::Keycode::SPACE)
#define IS_ROLL_DOWN()      Util::Input::IsKeyDown(Util::Keycode::LSHIFT)   || Util::Input::IsKeyDown(Util::Keycode::LCTRL)

Player::Player(std::vector<std::string>& path, int Hp, GameWorldContext& World): 
    Character(path, Hp, World){
        m_Transform.scale = {2.0f, 2.0f};
        m_Transform.translation = {0.0f, -100.0f};
        top = 50 * m_Transform.scale.y;
        bottom = 0 * m_Transform.scale.y;
        left = 10 * m_Transform.scale.x;
        right = 10 * m_Transform.scale.x;
        
        m_PlayerINFO = std::make_shared<PlayerUI>();

        AddChild(m_PlayerINFO);
}


void Player::Attack(float dt){
    if (GetState() == c_state::roll     || //翻滾狀態不能攻擊
        GetState() == c_state::clinb    || //攀爬狀態不能攻擊
        GetState() == c_state::block    ) return; 
    

    std::shared_ptr<Weapon> UsedWeapon = nullptr;
    //使用武器1 
    if (Util::Input::IsKeyDown(Util::Keycode::J)){
        UsedWeapon = std::dynamic_pointer_cast<Weapon>(m_Skill1);
    }//使用武器2
    else if(Util::Input::IsKeyDown(Util::Keycode::K)){
        UsedWeapon = std::dynamic_pointer_cast<Weapon>(m_skill2);
    }

    //武器動畫與 c_State 設定
    if (UsedWeapon){
        int slot = UsedWeapon == m_Skill1 ? 0 : 1;
        if (!m_AttackManager.IsAttacking()) m_AttackManager.StartAttack(slot, UsedWeapon);
        
        if (IsContainState(c_state::atk)) SetState(c_state::atk);
        else InitState(c_state::atk, nullptr);
    }

    //攻擊操作
    if (GetState() == c_state::atk && m_AttackManager.IsAtkAble()){
        auto currentWeapon = m_AttackManager.GetCurrentWeapon();
        if (currentWeapon == nullptr){
            m_AttackManager.Update(dt);
            return;
        }
        //player 攻擊產生的碰撞箱
        
        bool IsWeaponUsed = false;
        currentWeapon->Use( m_World.Mobs->GetObjs(), m_WorldPos, IsWeaponUsed, 
                            m_Transform.scale, m_AttackManager.GetComboIndex());

        if (IsWeaponUsed) m_AttackManager.UpdateAtkTimes();
    }
    m_AttackManager.Update(dt);
}

void Player::Block(){
    if (GetState() == c_state::roll) return; //翻滾狀態不能擋格
    if (GetState() == c_state::clinb) return; //攀爬狀態不能擋格
    if (GetState() == c_state::atk) return; //攻擊狀態不能擋格

    std::shared_ptr<Shield> UsedShield = nullptr;
    bool JPressed =  Util::Input::IsKeyPressed(Util::Keycode::J);
    bool KPressed =  Util::Input::IsKeyPressed(Util::Keycode::K);
    if (JPressed) UsedShield = std::dynamic_pointer_cast<Shield>(m_Skill1);
    else if (KPressed) UsedShield = std::dynamic_pointer_cast<Shield>(m_skill2);


    if (GetState() == c_state::block) {
        // 按鍵放開 → 播放 block end → block end 動畫結束 → 回 idle
        bool JUp = Util::Input::IsKeyUp(Util::Keycode::J);
        bool KUp = Util::Input::IsKeyUp(Util::Keycode::K);
        auto currAnim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);

        // blockend to idle( if Animation is ended)
        if (m_CurrentShield->GetState() == Shield::State::BlockEnd){
            if (currAnim->GetCurrentFrameIndex() == currAnim->GetFrameCount() -1){
                SetState(c_state::idle);
                m_CurrentShield = nullptr;
            }
            return;
        }   //Shield from block to blocckend
        else if ((m_CurrentShield == m_Skill1 && JUp) || (m_CurrentShield == m_skill2 && KUp)) {
            ChangeDrawable(AccessKey(), m_CurrentShield->GetBlockEndDrawable(), c_state::block);
            m_CurrentShield->BlockEnd();
            m_Defense = 0;
        }
        return;
    }   //from other c_state to block
    else{
        if (UsedShield == nullptr) return;
        if (IsContainState(c_state::block)){
            ChangeDrawable(AccessKey(), UsedShield->GetBlockDrawable(), c_state::block);
            SetState(c_state::block);
        }
        else InitState(c_state::block, UsedShield->GetBlockDrawable());
        m_Defense = UsedShield->GetDefense();
        m_CurrentShield = UsedShield;
        UsedShield->Block();
    }
}

void Player::Attacked(int Damage, glm::vec2 Dir, float Velocity){
    VelocityX += Dir.x > 0 ? Velocity : -1*Velocity;
    float hurt = 0;

    if (m_CurrentShield &&
    ((Dir.x > 0 && m_Transform.scale.x < 0) || (Dir.x < 0 && m_Transform.scale.x > 0))){    //Shield is used and Dir is correct
        Rect HitBox;
        Rect MobRect;
        switch (m_CurrentShield->GetState()){
        case Shield::State::Block:
            hurt = abs((float)Damage * (1-m_Defense));
            break;        
        case Shield::State::BlockEnd:
            VelocityX = 0;

            HitBox = m_CurrentShield->GetParryHitBox(m_WorldPos, m_Transform.scale);
            for (auto& mob : m_World.Mobs->GetObjs()){
                auto MobObj = std::dynamic_pointer_cast<Mob>(mob);
                if (MobObj == nullptr) continue;

                MobRect = Rect::CreateRect(MobObj->m_WorldPos, MobObj->top + MobObj->bottom, MobObj->left + MobObj->right);
                if (HitBox.Intersects(MobRect)){
                    m_CurrentShield->Parry(MobObj, m_Transform.scale);
                }
            }
            return; //blockend狀態不會受到傷害
        default:
            return;
        }
    }
    else{   //一般被攻擊行為
        SetState(c_state::idle);
        Jump();
        VelocityY = 10.f;
        hurt = Damage;
    }

    m_Hp -= hurt;
    m_PlayerINFO->SetHp(m_Hp);
}

void Player::PickUpDrops(std::shared_ptr<Drops> drops){
    auto NewItem = drops->ToItem();

    //如果是武器
    if (NewItem){
        if (m_Skill1 == nullptr){
            m_Skill1 = NewItem;
            m_PlayerINFO->SetSkill(m_Skill1, 0);
        }
        else if (m_skill2 == nullptr){
            m_skill2 = NewItem;
            m_PlayerINFO->SetSkill(m_skill2, 1);
        }
        else{ //WIP
            //彈出更換武器視窗
            static int select = 0;
            select = 1 - select;

            //將被替換的武器變成掉落物
            std::shared_ptr<Item>& selectSkill = select == 0 ? m_Skill1 : m_skill2;
            auto temp = selectSkill->ToDrops();
            temp->m_WorldPos = m_WorldPos + glm::vec2(10, 40);
            m_World.WorldDrops->AddObj(temp);
            selectSkill = NewItem;
            
            //更換slot圖案
            m_PlayerINFO->SetSkill(selectSkill, select);
        }
        m_World.WorldDrops->RemoveObj(drops);
    } //如果是卷軸
    else{ //WIP
        //卷軸效果加乘
    }
}

void Player::PickUp(){
    if (GetState() == c_state::roll) return; //翻滾狀態不能撿東西
    if (GetState() == c_state::atk) return; //攻擊狀態不能撿東西
    if (GetState() == c_state::block) return;

    if (Util::Input::IsKeyDown(Util::Keycode::R)){
        for (auto& temp : m_World.WorldDrops->GetObjs()){
            auto drop = std::dynamic_pointer_cast<Drops>(temp);
            if (IsNearBy(drop, 50.0f)){
                LOG_DEBUG("Get Item");
                PickUpDrops(drop);
                return;
            }
        }
    }
}

void Player::InterAct(){
    for (auto& InterActAble : m_World.InterActAbles){
        auto Obj = std::dynamic_pointer_cast<MapObj>(InterActAble);
        if (Obj && IsNearBy(Obj, 100.0)){
            InterActAble->InterAct();
        }
    }
}

/*-----------------------------------util-----------------------------------*/

void Player::TestP(){
    if (Util::Input::IsKeyDown(Util::Keycode::P)){
        // m_Hp -= 5;
        // m_PlayerINFO->SetHp(m_Hp);
        // if (m_PlayerINFO->GetCurrentHp() <= 0){
        //     m_PlayerINFO->SetHp(200);
        //     m_Hp = 200;
        // }
        LOG_DEBUG(m_WorldPos);
    }

}

bool Player::IsOnOSP(){
    if (!InGround) return false;

    glm::vec2 Pos = m_WorldPos;
    glm::vec2 OSP_Pos;
    glm::vec2 OSP_scale;
    bool x, y;

    for (auto& OSP : m_World.OneSidedPlatforms){
        if (!IsNearBy(OSP, 640.0f)) continue;
        OSP_Pos = OSP->m_WorldPos;
        OSP_scale = abs(OSP->GetScaledSize());

        x = !((Pos.x < OSP_Pos.x - OSP_scale.x/2 - 1) || (Pos.x > OSP_Pos.x + OSP_scale.x/2 + 1));
        y = (Pos.y > OSP_Pos.y - OSP_scale.y/2) && (Pos.y < OSP_Pos.y + OSP_scale.y/2 + 2);
        if (x && y) return true;
    }
    return false;
}

bool Player::IsUnderOSP(){
    if (InGround) return false;

    glm::vec2 Pos = m_WorldPos;
    glm::vec2 OSP_scale;
    bool x, y;

    for (auto& OSP : m_World.OneSidedPlatforms){
        if (!IsNearBy(OSP, 640.0f)) continue;
        OSP_scale = abs(OSP->GetScaledSize());

        x = !((Pos.x < OSP->m_WorldPos.x - OSP->left - 1) || (Pos.x > OSP->m_WorldPos.x + OSP->right + 1));
        y = (Pos.y - bottom < OSP->m_WorldPos.y - OSP->top) && (Pos.y + top > OSP->m_WorldPos.y - 10.0f);

        if (x && y) return true;
    }
    return false;
}

/*-----------------------------------move-----------------------------------*/

void Player::Move(float dt){
    /**
     * 特定狀態不能移動
     */
    if (GetState() == c_state::clinb) return; //攀爬時不能移動
    if (GetState() == c_state::atk || GetState() == c_state::block){
        Physics::SlowDown(VelocityX, Friction);
        return;
    }


    if ((IS_DOWN_PRESSED()) && (GetState() == c_state::crouch)) {
        Physics::SlowDown(VelocityX, Friction);
        if ((IS_UP_DOWN()) && IsOnOSP()){
            m_WorldPos.y -= 15;
            VelocityY = -5.f;
            fall();
            m_IgnoreOSP = true;
        }
        return;
    }
    
    if (GetState() == c_state::crouch){
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);

        if (Util::Input::IsKeyUp(Util::Keycode::DOWN) || Util::Input::IsKeyUp(Util::Keycode::S)){
            temp->Play();
        }
        else if (temp->GetCurrentFrameIndex() == temp->GetFrameCount()-1){
            SetState(c_state::idle);   
        }
    }


    /**
     * 這裡的do while 是為了當無法左右移動時，jump() 也能被執行
     * 正個do while 裡的內容若涉及更改c_state都要求InGround == true
     * VelocityX 則不要求InGround == true
     * for jump 打斷roll
     */
    do{
        //press right
        if (IS_LEFT_PRESSED()){
            //與翻滾不同方向才會打斷翻滾狀態
            if (GetState() == c_state::roll && m_Transform.scale.x < 0) break;;
            
            //turn the player Img
            if (m_Transform.scale.x > 0) m_Transform.scale.x *= -1;
            
            //  change the state or add a new one
            if (GetState() != c_state::L_move && InGround){
                if (IsContainState(c_state::L_move)){ SetState(c_state::L_move);}
                else{ InitState(c_state::L_move, {20}, {RESOURCE_DIR"/Beheaded/runA/runA_"});}
            }
            
            //  chaenge pos
            if (VelocityX > -1*MaxSpeed) VelocityX += -1*AccelerationX*dt;
            else if (VelocityX < -1*MaxSpeed) VelocityX = -1*MaxSpeed;
        } //press right
        else if (IS_RIGHT_PRESSED()){  
            //與翻滾不同方向才會打斷翻滾狀態
            if (GetState() == c_state::roll && m_Transform.scale.x > 0) break;
            
            //turn the player Img
            if (m_Transform.scale.x < 0) m_Transform.scale.x *= -1;
            
            //  change the state or add a new one
            if (GetState() != c_state::R_move && InGround){
                if (IsContainState(c_state::R_move)){ SetState(c_state::R_move);}
                else{ InitState(c_state::L_move, {20}, {RESOURCE_DIR"/Beheaded/runA/runA_"});}
            }
            
            //  set Velocity
            if (VelocityX < MaxSpeed) VelocityX += AccelerationX * dt;
            else if (VelocityX > MaxSpeed) VelocityX = MaxSpeed;
        } //do nothing
        else if (InGround && GetState() != c_state::roll){  
            // idle
            if (GetState() != c_state::idle && GetState() != c_state::crouch) SetState(c_state::idle);//  set state
            
            Physics::SlowDown(VelocityX, Friction);
        }
    }while (false);
    
    /**
     * 控制上下移動
     */
    if (IS_UP_DOWN()){
        //rendering, c_state, SET VelocityY
        Jump();
    } //Pressed Down
    else if (IS_DOWN_PRESSED()){
        //crouch
        if (InGround){
            if (IsContainState(c_state::crouch)) SetState(c_state::crouch);
            else InitState(c_state::crouch, {6}, {RESOURCE_DIR"/Beheaded/crouch/crouch_"});

            auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
            temp->SetCurrentFrame(0);
            temp->Pause();
        }
    } //在地面跟翻滾狀態不會進Fall state
    else if (!InGround && GetState() != c_state::roll){
        //fall
        if (GetState() != c_state::fall && VelocityY <= 0){
            //rendering, c_state
            fall();
        }
    }
}

void Player::Jump(){
    if (InGround) jumpStep = 0;   
    // if (jumpStep == 2) return;
    
    jumpStep++;
    if (GetState() != c_state::jump){
        if (IsContainState(c_state::jump)) SetState(c_state::jump, {}, false);
        else InitState(c_state::jump, {5}, {RESOURCE_DIR"/Beheaded/jump/jumpUp_"});
    }
    VelocityY = 12.5f; 
}

void Player::fall(){
    if(IsContainState(c_state::fall)) SetState(c_state::fall, {}, false);
    else{
        std::vector<std::string> Img =  {RESOURCE_DIR"/Beheaded/jump/jumpTrans_",
                                         RESOURCE_DIR"/Beheaded/jump/jumpDown_"};
        InitState(c_state::fall, {5, 5}, Img);
    }
}

void Player::Clinb(){
    if (GetState() == c_state::roll) return; //翻滾時不能攀爬
    if (GetState() == c_state::clinbOSP) return;
    if (IsUnderOSP()) return;
    if (InGround) return; //平地不會攀爬

    if (GetState() == c_state::clinb){
        VelocityY = 0;
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        //結束動畫
        if (temp->GetCurrentFrameIndex() == temp->GetFrameCount() - 1) SetState(c_state::idle);
    }
    if (IsUnderOSP()) return;

    std::vector<std::shared_ptr<SolidObj>> temps;
    temps.reserve(m_World.SolidObjs.size() + m_World.OneSidedPlatforms.size());
    temps.insert(temps.end(), m_World.SolidObjs.begin(), m_World.SolidObjs.end());
    temps.insert(temps.end(), m_World.OneSidedPlatforms.begin(), m_World.OneSidedPlatforms.end());

    float solidTop;
    float solidLeft;
    float solidRight;
    
    float playerTop;

    bool inYRange;
    bool leftCheck;
    bool rightCheck;


    for (auto& Solid : temps) {
        if (!IsNearBy(Solid, 3000.0f)) continue; //如果不在附近就跳過
        solidTop = Solid->top;
        solidLeft = Solid->left;
        solidRight = Solid->right;
        playerTop = m_WorldPos.y + top;

        inYRange = playerTop > Solid->m_WorldPos.y + solidTop &&
                        m_WorldPos.y < Solid->m_WorldPos.y + solidTop;
        //玩家朝左
        leftCheck = m_WorldPos.x - left <
                         Solid->m_WorldPos.x + std::abs(solidRight) + 3 &&
                         m_WorldPos.x > Solid->m_WorldPos.x + std::abs(solidRight) - 1 &&    //m_WorldPos.x > Solid->m_WorldPos.x - std::abs(solidLeft)
                         m_Transform.scale.x < 0;
        //玩家朝右
        rightCheck = m_WorldPos.x + right >
                          Solid->m_WorldPos.x - solidLeft - 3 &&
                          m_WorldPos.x < Solid->m_WorldPos.x - solidLeft + 1 && //m_WorldPos.x < Solid->m_WorldPos.x + solidRight
                          m_Transform.scale.x > 0;

        if (inYRange && (leftCheck || rightCheck)) {
            // 設定狀態
            if (IsContainState(c_state::clinb)) {
                SetState(c_state::clinb, {}, false);
            } else {
                InitState(c_state::clinb, {4}, {RESOURCE_DIR"/Beheaded/jump/jumpThrough_"});
                auto anim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
                anim->SetInterval(50);
            }
            // 設定位置
            m_WorldPos.y = Solid->m_WorldPos.y + solidTop + 10;
            VelocityX = VelocityY = 0;

            if (m_Transform.scale.x > 0) {
                m_WorldPos.x = Solid->m_WorldPos.x - solidLeft + 1;
            } else {
                m_WorldPos.x = Solid->m_WorldPos.x + solidRight - 1;
            }

            break; // 成功攀爬後跳出迴圈
        }
    }
}

void Player::ClinbOSP(){
    if (VelocityY < 0) return;

    if (GetState() == c_state::clinbOSP && !IsUnderOSP()){
        VelocityY = 0;
        SetState(c_state::idle);
        m_IgnoreOSP = false;
    }

    float P_Head = m_WorldPos.y + top;
    float P_Bottom = m_WorldPos.y - bottom;

    bool IsHeadOver, IsbottomUnder, IsXInRange;

    for (auto& OSP : m_World.OneSidedPlatforms){
        IsHeadOver = P_Head > OSP->m_WorldPos.y - OSP->bottom;
        IsbottomUnder = P_Bottom < OSP->m_WorldPos.y;
        IsXInRange = !((m_WorldPos.x < OSP->m_WorldPos.x - OSP->left - 1) || (m_WorldPos.x > OSP->m_WorldPos.x + OSP->right + 1));
        if (IsbottomUnder &&  IsHeadOver && IsXInRange){
                //rendering, c_state
                if (IsContainState(c_state::clinbOSP)) SetState(c_state::clinbOSP);
                else InitState(c_state::clinbOSP, {2}, {RESOURCE_DIR"/Beheaded/land/land_"});

                //Move logic                
                VelocityY = 7.5f;
                m_IgnoreOSP = true;
                return;
            }
    }
}

void Player::roll(){

    //roll end detect
    // if (!InGround && GetState() != c_state::roll) return;
    if (GetState() == c_state::roll){
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (temp->GetCurrentFrameIndex() == temp->GetFrameCount() - 1){ //結束動畫
            if (m_Transform.scale.x > 0) VelocityX = MaxSpeed;
            else VelocityX = -1*MaxSpeed;
            
            if (InGround) SetState(c_state::idle);
            else SetState(c_state::fall, {}, false);
            m_Atkedable = true;
        }
        return;
    }
    //detect end
    

    //rendering, c_state
    if (!timer.IsTimeout()) return;//翻滾冷卻

    if (IsContainState(c_state::roll)){
        SetState(c_state::roll, {}, false);
    }
    else{
        std::vector<std::string> paths = {  RESOURCE_DIR"/Beheaded/roll/rollstart_",
                                            RESOURCE_DIR"/Beheaded/roll/rolling_", 
                                            RESOURCE_DIR"/Beheaded/roll/rollidle_"};
        InitState(c_state::roll, {4, 6, 5}, paths);
    }
    //rendering, c_state end

    //set Velocity 
    if (m_Transform.scale.x > 0) VelocityX = 15.0f;
    else VelocityX = -15.0f;
    m_Atkedable = false;
    //set Velocity end
    timer.SetTime(700, 700);
    timer.ResetTime();
}

/*-----------------------------------update-----------------------------------*/
void Player::Update(float dt){
    InGround = Physics::IsOnGround(this, m_World.SolidObjs, m_World.OneSidedPlatforms);
    Physics::ApplyGravity(VelocityY, InGround, Gravity, MaxFallSpeed);
    if (InGround) m_IgnoreOSP = false;
    Move(dt);

    if (IS_ROLL_DOWN() || GetState() == c_state::roll){
        roll();
    }
    if (GetState() != c_state::roll) m_Atkedable = true;

    if (Util::Input::IsKeyDown(Util::Keycode::R)){
        InterAct();
    }
    
    Clinb();
    ClinbOSP();
    
    PickUp();
    Attack(dt);
    Block();

    FixPos(dt);
    TestP();

    m_WorldPos.x += VelocityX * dt;
    m_WorldPos.y += VelocityY * dt;
}