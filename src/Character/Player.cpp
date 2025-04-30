#include "Character/Player.hpp"

#include "Util/Input.hpp"

#include "Util/Logger.hpp"


#define IS_LEFT_PRESSED()   Util::Input::IsKeyPressed(Util::Keycode::LEFT)  || Util::Input::IsKeyPressed(Util::Keycode::A)
#define IS_RIGHT_PRESSED()  Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)
#define IS_DOWN_PRESSED()   Util::Input::IsKeyPressed(Util::Keycode::DOWN)  || Util::Input::IsKeyPressed(Util::Keycode::S)
#define IS_UP_DOWN()        Util::Input::IsKeyDown(Util::Keycode::UP)       || Util::Input::IsKeyDown(Util::Keycode::W) || Util::Input::IsKeyDown(Util::Keycode::SPACE)
#define IS_ROLL_DOWN()      Util::Input::IsKeyDown(Util::Keycode::LSHIFT)   || Util::Input::IsKeyDown(Util::Keycode::LCTRL)

Player::Player(std::vector<std::string>& path, int Hp, 
    const std::vector<std::shared_ptr<SolidObj>>& SolidObjs, 
    const std::vector<std::shared_ptr<OneSidedPlatform>>& OSP,
    std::vector<std::shared_ptr<Drops>>& Drops)
    : Character(path, Hp, SolidObjs, OSP), 
    r_WorldDrops(Drops){
        m_Transform.scale = {2.0f, 2.0f};
        m_Transform.translation = {0.0f, -100.0f};
        top = 60, bottom = 0, left = 10, right = 10;

        m_PlayerINFO = std::make_shared<PlayerUI>();
        
        AddChild(m_PlayerINFO);
}

//WIP
void Player::Attack(){}

bool Player::IsNearbyDrops(std::shared_ptr<Drops> drops){
    glm::vec2 D = m_WorldPos - drops->m_WorldPos;
    return glm::length(D) <= 50.0f;
}

void Player::PickUpDrops(std::shared_ptr<Drops> drops){
    //如果是武器
    auto NewWeapon = std::dynamic_pointer_cast<Weapon>(drops->ToItem());
    
    if (NewWeapon){
        if (!m_Weapon1) m_Weapon1 = NewWeapon;
        else if (!m_Weapon2) m_Weapon2 = NewWeapon;
        else{ //WIP
            //彈出更換武器視窗


            //將被替換的武器變成掉落物
            auto temp = m_Weapon1->ToDrops();
            temp->m_WorldPos = m_WorldPos + glm::vec2(0, 10);
            r_WorldDrops.push_back(temp);
            m_Weapon1 = NewWeapon;
            r_WorldDrops.erase(std::remove(r_WorldDrops.begin(), r_WorldDrops.end(), drops), r_WorldDrops.end());
            ChangeDrawable(AccessKey(), m_Weapon1->GetPlayerDrawable());
        }
    } //如果是卷軸
    else{ //WIP
        //卷軸效果加乘
    }
}

void Player::PickUp(){
    if (!InGround()) return;
    if (GetState() == c_state::roll) return; //翻滾狀態不能撿東西
    if (GetState() == c_state::atk) return; //攻擊狀態不能撿東西

    if (Util::Input::IsKeyPressed(Util::Keycode::R)){
        for (auto& drop : r_WorldDrops){
            if (IsNearbyDrops(drop)){
                PickUpDrops(drop);
                return;
            }
        }
    }
}

/*-----------------------------------util-----------------------------------*/

void Player::TestP(){
    if (Util::Input::IsKeyDown(Util::Keycode::P)){
        m_PlayerINFO->SetHp(m_PlayerINFO->GetCurrentHp() - 5);
        if (m_PlayerINFO->GetCurrentHp() <= 0){
            m_PlayerINFO->SetHp(100);
        }
        //LOG_DEBUG(m_WorldPos);
    }

}

void Player::SlowDown(){

    //slow down
    if (VelocityX > 0){

        VelocityX -= Friction;
        if (VelocityX < 0) VelocityX = 0;
    }
    else if(VelocityX < 0){
        VelocityX += Friction;
        if (VelocityX > 0) VelocityX = 0;
    } 
}

bool Player::IsOnOSP(){
    if (!InGround()) return false;

    glm::vec2 Pos = m_WorldPos;
    glm::vec2 OSP_Pos;
    glm::vec2 OSP_scale;
    bool x, y;

    for (auto& OSP : r_OneSidedPlatforms){
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
    if (InGround()) return false;

    glm::vec2 Pos = m_WorldPos;
    glm::vec2 OSP_scale;
    bool x, y;

    for (auto& OSP : r_OneSidedPlatforms){
        if (!IsNearBy(OSP, 640.0f)) continue;
        OSP_scale = abs(OSP->GetScaledSize());

        x = !((Pos.x < OSP->m_WorldPos.x - OSP_scale.x/2 - 1) || (Pos.x > OSP->m_WorldPos.x + OSP_scale.x/2 + 1));
        y = (Pos.y < OSP->m_WorldPos.y - OSP_scale.y/2.0f) && (Pos.y + top*m_Transform.scale.x > OSP->m_WorldPos.y - 20.0f);

        if (x && y) return true;
    }
    return false;
}

/*-----------------------------------move-----------------------------------*/

void Player::Move(){
    /**
     * 特定狀態不能移動
     */
    if (GetState() == c_state::clinb) return; //攀爬時不能移動


    if ((IS_DOWN_PRESSED()) && (GetState() == c_state::crouch)) {
        SlowDown();
        if ((IS_UP_DOWN()) && IsOnOSP()){

            m_WorldPos.y -= 10;
            fall();
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
     * 正個do while 裡的內容若涉及更改c_state都要求InGround() == true
     * VelocityX 則不要求InGround() == true
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
            if (GetState() != c_state::L_move && InGround()){
                if (IsContainState(c_state::L_move)){ SetState(c_state::L_move);}
                else{ InitState(c_state::L_move, {20}, {RESOURCE_DIR"/Beheaded/runA/runA_"});}
            }
            
            //  chaenge pos
            if (VelocityX > -1*MaxSpeed) VelocityX += -1*AccelerationX;
            else if (VelocityX < -1*MaxSpeed) VelocityX = -1*MaxSpeed;
        } //press right
        else if (IS_RIGHT_PRESSED()){  
            //與翻滾不同方向才會打斷翻滾狀態
            if (GetState() == c_state::roll && m_Transform.scale.x > 0) break;
            
            //turn the player Img
            if (m_Transform.scale.x < 0) m_Transform.scale.x *= -1;
            
            //  change the state or add a new one
            if (GetState() != c_state::R_move && InGround()){
                if (IsContainState(c_state::R_move)){ SetState(c_state::R_move);}
                else{ InitState(c_state::L_move, {20}, {RESOURCE_DIR"/Beheaded/runA/runA_"});}
            }
            
            //  set Velocity
            if (VelocityX < MaxSpeed) VelocityX += AccelerationX;
            else if (VelocityX > MaxSpeed) VelocityX = MaxSpeed;
        } //do nothing
        else if (InGround() && GetState() != c_state::roll){  
            // idle
            if (GetState() != c_state::idle && GetState() != c_state::crouch) SetState(c_state::idle);//  set state
            
            SlowDown();  
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
        if (InGround()){

            if (IsContainState(c_state::crouch)) SetState(c_state::crouch);
            else InitState(c_state::crouch, {6}, {RESOURCE_DIR"/Beheaded/crouch/crouch_"});

            auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
            temp->SetCurrentFrame(0);
            temp->Pause();
        }
    } //在地面跟翻滾狀態不會進Fall state
    else if (!InGround() && GetState() != c_state::roll){
        //fall
        if (GetState() != c_state::fall && VelocityY <= 0){
            //rendering, c_state
            fall();
        }
    }
}

void Player::Jump(){
    if (InGround()) jumpStep = 0;   
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
    if (InGround()) return; //平地不會攀爬

    if (GetState() == c_state::clinb){
        VelocityY = 0;
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        //結束動畫
        if (temp->GetCurrentFrameIndex() == temp->GetFrameCount() - 1) SetState(c_state::idle);
    }
    if (IsUnderOSP()) return;

    std::vector<std::shared_ptr<SolidObj>> temps;
    temps.reserve(r_SolidObjs.size() + r_OneSidedPlatforms.size());
    temps.insert(temps.end(), r_SolidObjs.begin(), r_SolidObjs.end());
    temps.insert(temps.end(), r_OneSidedPlatforms.begin(), r_OneSidedPlatforms.end());

    float solidTop;
    float solidLeft;
    float solidRight;
    
    float playerTop;

    bool inYRange;
    bool leftCheck;
    bool rightCheck;


    for (auto& Solid : temps) {
        if (!IsNearBy(Solid, 3000.0f)) continue; //如果不在附近就跳過
        solidTop = Solid->top * Solid->m_Transform.scale.y;
        solidLeft = Solid->left * Solid->m_Transform.scale.x;
        solidRight = Solid->right * Solid->m_Transform.scale.x;

        playerTop = m_WorldPos.y + top * m_Transform.scale.y;

        inYRange = playerTop > Solid->m_WorldPos.y + solidTop &&
                        m_WorldPos.y < Solid->m_WorldPos.y + solidTop;
        //玩家朝左
        leftCheck = m_WorldPos.x - std::abs(left * m_Transform.scale.x) <
                         Solid->m_WorldPos.x + std::abs(solidRight) + 3 &&
                         m_WorldPos.x > Solid->m_WorldPos.x + std::abs(solidRight) - 1 &&    //m_WorldPos.x > Solid->m_WorldPos.x - std::abs(solidLeft)
                         m_Transform.scale.x < 0;
        //玩家朝右
        rightCheck = m_WorldPos.x + std::abs(right * m_Transform.scale.x) >
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
    }

    float P_Head = m_WorldPos.y + top * m_Transform.scale.y;
    float P_Bottom = m_WorldPos.y;

    bool IsHeadOver, IsbottomUnder, IsXInRange;

    for (auto& OSP : r_OneSidedPlatforms){
        IsHeadOver = P_Head > OSP->m_WorldPos.y;
        IsbottomUnder = P_Bottom < OSP->m_WorldPos.y-3;
        IsXInRange = !((m_WorldPos.x < OSP->m_WorldPos.x - OSP->GetScaledSize().x/2 - 1) || (m_WorldPos.x > OSP->m_WorldPos.x + OSP->GetScaledSize().x/2 + 1));
        if ((IsbottomUnder &&  IsHeadOver && IsXInRange) ||
            (!InGround() && IsUnderOSP())){

                if (GetState() != c_state::clinbOSP) m_WorldPos.y += 50;

                //rendering, c_state
                if (IsContainState(c_state::clinbOSP)) SetState(c_state::clinbOSP);
                else InitState(c_state::clinbOSP, {2}, {RESOURCE_DIR"/Beheaded/land/land_"});

                //Move logic
                
                VelocityY = 10.0f;
                return;
            }
    }
}

void Player::roll(){

    //roll end detect
    // if (!InGround() && GetState() != c_state::roll) return;
    if (GetState() == c_state::roll){
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (temp->GetCurrentFrameIndex() == temp->GetFrameCount() - 1){ //結束動畫
            if (m_Transform.scale.x > 0) VelocityX = MaxSpeed;
            else VelocityX = -1*MaxSpeed;
            
            if (InGround()) SetState(c_state::idle);
            else SetState(c_state::fall, {}, false);
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
    //set Velocity end
    timer.SetChangeTime(700, 700);
    timer.ResetCheckTime();
}

/*-----------------------------------update-----------------------------------*/
void Player::Update(){
    Move();
    applyGravity();

    if (IS_ROLL_DOWN() || GetState() == c_state::roll){
        roll();
    }
    
    Clinb();
    ClinbOSP();
    FixPos();

    m_WorldPos.x += VelocityX;
    m_WorldPos.y += VelocityY;

    TestP();


    //PickUp();
}