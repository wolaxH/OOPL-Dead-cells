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
    float x, y;

    for (auto& OSP : r_OneSidedPlatforms){
        OSP_Pos = OSP->m_WorldPos;
        OSP_scale = abs(OSP->GetScaledSize());

        x = !((Pos.x < OSP_Pos.x - OSP_scale.x/2 - 1) || (Pos.x > OSP_Pos.x + OSP_scale.x/2 + 1));
        y = (Pos.y > OSP_Pos.y - OSP_scale.y/2) && (Pos.y < OSP_Pos.y + OSP_scale.y/2 + 2);
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
    // if (GetState() == c_state::roll) return; //滾動時不能移動


    /**
     * 這裡的do while 是為了當無法左右移動時，jump() 也能被執行
     * 正個do while 裡的內容若涉及更改c_state都要求InGround() == true
     * VelocityX 則不要求InGround() == true
     * for jump 打斷roll
     */
    do{
        if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)){    //press right
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
        else if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)){  //press right
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
    
    
    if (Util::Input::IsKeyDown(Util::Keycode::UP)){Jump();}
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

    if (InGround()) return;
    if (GetState() == c_state::clinb){
        VelocityY = 0;
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        //結束動畫
        if (temp->GetCurrentFrameIndex() == temp->GetFrameCount() - 1) SetState(c_state::idle);
    }

    /**
     * This is the temporary solution for the one sided platform.
     */

    std::vector<std::shared_ptr<SolidObj>> temps;
    temps.reserve(r_SolidObjs.size() + r_OneSidedPlatforms.size());
    temps.insert(temps.end(), r_SolidObjs.begin(), r_SolidObjs.end());
    temps.insert(temps.end(), r_OneSidedPlatforms.begin(), r_OneSidedPlatforms.end());

    
    for (auto Solid :temps){
        if ((m_WorldPos.y + top*m_Transform.scale.y > Solid->m_WorldPos.y + Solid->top*Solid->m_Transform.scale.y &&
            m_WorldPos.y < Solid->m_WorldPos.y + Solid->top*Solid->m_Transform.scale.y) &&
            ((m_WorldPos.x - abs(left*m_Transform.scale.x) < Solid->m_WorldPos.x + abs(Solid->right*Solid->m_Transform.scale.x) + 3 &&
            m_WorldPos.x > Solid->m_WorldPos.x - abs(Solid->left*Solid->m_Transform.scale.x)) ||
            (m_WorldPos.x + right*m_Transform.scale.x > Solid->m_WorldPos.x - Solid->left*Solid->m_Transform.scale.x - 3 && 
            m_WorldPos.x < Solid->m_WorldPos.x + Solid->right*Solid->m_Transform.scale.x))){

                //rendering, c_state
                if (IsContainState(c_state::clinb)){
                    SetState(c_state::clinb, {}, false);
                }
                else{
                    InitState(c_state::clinb, {4}, {RESOURCE_DIR"/Beheaded/jump/jumpThrough_"});
                    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
                    temp->SetInterval(50);
                }

                //set pos
                m_WorldPos.y = Solid->m_WorldPos.y + Solid->top*Solid->m_Transform.scale.y + 10;
                VelocityY = 0, VelocityX = 0;
                if(m_Transform.scale.x > 0){
                    m_WorldPos.x = Solid->m_WorldPos.x - Solid->left*Solid->m_Transform.scale.x + 1;
                }
                else if (m_Transform.scale.x < 0){
                    m_WorldPos.x = Solid->m_WorldPos.x + Solid->right*Solid->m_Transform.scale.x - 1;
                }
                break;
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
    FixPos();

    m_WorldPos.x += VelocityX;
    m_WorldPos.y += VelocityY;

    TestP();
}