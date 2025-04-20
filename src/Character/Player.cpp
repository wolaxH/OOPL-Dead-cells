#include "Character/Player.hpp"

#include "Util/Input.hpp"

#include "Util/Logger.hpp"

Player::Player(std::vector<std::string>& path, int Hp, 
    const std::vector<std::shared_ptr<SolidObj>>& SolidObjs, 
    const std::vector<std::shared_ptr<OneSidedPlatform>>& OSP)
    : Character(path, Hp, SolidObjs, OSP){
        m_Transform.scale = {2.0f, 2.0f};
        m_Transform.translation = {0.0f, -100.0f};
        top = 60, bottom = 0, left = 10, right = 10;
}

//WIP
void Player::Attack(){}

/*-----------------------------------util-----------------------------------*/

void Player::TestP(){
    if (Util::Input::IsKeyDown(Util::Keycode::P)) LOG_DEBUG(m_WorldPos);
}


bool Player::IsOnOSP(){
    if (!InGround()) return false;

    glm::vec2 Pos = m_WorldPos;
    glm::vec2 OSP_Pos;
    glm::vec2 OSP_scale;
    bool x, y;

    for (auto& OSP : r_OneSidedPlatforms){
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



    /**
     * 這裡的do while 是為了當無法左右移動時，jump() 也能被執行
     * 正個do while 裡的內容若涉及更改c_state都要求InGround() == true
     * VelocityX 則不要求InGround() == true
     * for jump 打斷roll
     */
    do{
        if (Util::Input::IsKeyPressed(Util::Keycode::LEFT) || Util::Input::IsKeyPressed(Util::Keycode::A)){    //press right
            if (GetState() == c_state::roll && m_Transform.scale.x < 0) break;;
            
            if (m_Transform.scale.x > 0) m_Transform.scale.x *= -1; //turn the player Img
            
            if (GetState() != c_state::L_move && InGround()){   //  change the state or add a new one
                if (IsContainState(c_state::L_move)){ SetState(c_state::L_move);}
                else{ InitState(c_state::L_move, {20}, {RESOURCE_DIR"/Beheaded/runA/runA_"});}
            }
            
            //  chaenge pos
            if (VelocityX > -1*MaxSpeed) VelocityX += -1*AccelerationX;
            else if (VelocityX < -1*MaxSpeed) VelocityX = -1*MaxSpeed;
        }
        else if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)){  //press right
            if (GetState() == c_state::roll && m_Transform.scale.x > 0) break;
            
            if (m_Transform.scale.x < 0) m_Transform.scale.x *= -1;  //turn the player Img
            
            if (GetState() != c_state::R_move && InGround()){   //  change the state or add a new one
                if (IsContainState(c_state::R_move)){ SetState(c_state::R_move);}
                else{ InitState(c_state::L_move, {20}, {RESOURCE_DIR"/Beheaded/runA/runA_"});}
            }
            
            //  set Velocity
            if (VelocityX < MaxSpeed) VelocityX += AccelerationX;
            else if (VelocityX > MaxSpeed) VelocityX = MaxSpeed;
        }
        else if (InGround() && GetState() != c_state::roll){   //do nothing
            //滾動狀態不會進入idle狀態
    
            // idle
            if (GetState() != c_state::idle) SetState(c_state::idle);//  set state
            
            if (VelocityX > 0){ //slow down
                VelocityX -= Friction;
                if (VelocityX < 0) VelocityX = 0;
            }
            else if(VelocityX < 0){
                VelocityX += Friction;
                if (VelocityX > 0) VelocityX = 0;
            }    
        }
    }while (false);
    
    
    if (Util::Input::IsKeyDown(Util::Keycode::UP) || Util::Input::IsKeyPressed(Util::Keycode::W)){Jump();}
    else if (!InGround() && GetState() != c_state::roll){
        if (GetState() != c_state::fall && VelocityY <= 0){   //fall
            //rendering, c_state
            if(IsContainState(c_state::fall)) SetState(c_state::fall, {}, false);
            else{
                std::vector<std::string> Img =  {RESOURCE_DIR"/Beheaded/jump/jumpTrans_",
                                                 RESOURCE_DIR"/Beheaded/jump/jumpDown_"};
                InitState(c_state::fall, {5, 5}, Img);
            }
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
        solidTop = Solid->top * Solid->m_Transform.scale.y;
        solidLeft = Solid->left * Solid->m_Transform.scale.x;
        solidRight = Solid->right * Solid->m_Transform.scale.x;

        playerTop = m_WorldPos.y + top * m_Transform.scale.y;

        inYRange = playerTop > Solid->m_WorldPos.y + solidTop &&
                        m_WorldPos.y < Solid->m_WorldPos.y + solidTop;

        leftCheck = m_WorldPos.x - std::abs(left * m_Transform.scale.x) <
                         Solid->m_WorldPos.x + std::abs(solidRight) + 3 &&
                         m_WorldPos.x > Solid->m_WorldPos.x + std::abs(solidLeft);

        rightCheck = m_WorldPos.x + right * m_Transform.scale.x >
                          Solid->m_WorldPos.x - solidLeft - 3 &&
                          m_WorldPos.x < Solid->m_WorldPos.x - solidRight;

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

    if (Util::Input::IsKeyDown(Util::Keycode::LSHIFT) || 
        Util::Input::IsKeyDown(Util::Keycode::LCTRL) ||
        GetState() == c_state::roll){
        roll();
    }
    
    Clinb();
    ClinbOSP();
    FixPos();

    m_WorldPos.x += VelocityX;
    m_WorldPos.y += VelocityY;

    TestP();
}