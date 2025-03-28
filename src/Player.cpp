#include "Player.hpp"

#include "Util/Input.hpp"

Player::Player(std::vector<std::string>& path, int Hp) : Character(path, Hp){
    m_Transform.scale = {1.5f, 1.5f};
    top = 60, bottom = 0, left = 10, right = 10;
}

void Player::Attack(){
    
}


/*-----------------------------------move-----------------------------------*/

void Player::Move(){    //WIP jump and fall
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)){    //press right
        
        m_Transform.scale.x = -1.5; //turn the player Img
        
        if (GetState() != c_state::L_move && InGround()){   //  change the state or add a new one
            if (IsContainState(c_state::L_move)){ SetState(c_state::L_move);}
            else{ InitState(c_state::L_move, {20}, {RESOURCE_DIR"/Beheaded/runA/runA_"});}
        }
        
        //  chaenge pos
        if (VelocityX > -1*MaxSpeed) VelocityX += -1*AccelerationX;
        else if (VelocityX < -1*MaxSpeed) VelocityX = -1*MaxSpeed;
    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)){  //press right
        
        m_Transform.scale.x = 1.5;  //turn the player Img
        
        if (GetState() != c_state::R_move && InGround()){   //  change the state or add a new one
            if (IsContainState(c_state::R_move)){ SetState(c_state::R_move);}
            else{ InitState(c_state::L_move, {20}, {RESOURCE_DIR"/Beheaded/runA/runA_"});}
        }
        
        //  set Velocity
        if (VelocityX < MaxSpeed) VelocityX += AccelerationX;
        else if (VelocityX > MaxSpeed) VelocityX = MaxSpeed;
    }
    else{   //do nothing
        if (InGround()){    // idle
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
    }
    
    if (Util::Input::IsKeyDown(Util::Keycode::UP)){Jump();}
    else if (!InGround()){
        if (GetState() != c_state::fall && VelocityY <= 0){   //fall
            if(IsContainState(c_state::fall)){
                SetState(c_state::fall);
                std::shared_ptr<Util::Animation> temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
                temp->Play();
            }
            else{
                std::vector<std::string> Img = {RESOURCE_DIR"/Beheaded/jump/jumpTrans_", RESOURCE_DIR"/Beheaded/jump/jumpDown_"};
                InitState(c_state::fall, {5, 5}, Img);
            }
        }
    }
}

void Player::Jump(){    //WIP add anima
    if (InGround()) jumpStep = 0;   
    //if (jumpStep == 2) return;
    
    jumpStep++;
    if (GetState() != c_state::jump){
        if (IsContainState(c_state::jump)){
            SetState(c_state::jump);
            std::shared_ptr<Util::Animation> temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
            temp->Play();
        }
        else{ 
            InitState(c_state::jump, {5}, {RESOURCE_DIR"/Beheaded/jump/jumpUp_"});
        }
    }
    VelocityY = 12.5f; 
}


/*
    Clinb:
        setPos
        當動畫撥放完之前不能move
        動畫播完後c_state = idle
*/

void Player::Clinb(){
    if (InGround()) return;
    if (GetState() == c_state::clinb){
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (temp->GetCurrentFrameIndex() == temp->GetFrameCount() - 1){ //結束動畫
            SetState(c_state::idle);
            std::shared_ptr<Util::Animation> temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
            temp->Play();
        }
    }

    for (auto Solid : SolidObjs){
        if ((m_WorldPos.y + top*m_Transform.scale.y > Solid->m_WorldPos.y + Solid->top*Solid->m_Transform.scale.y &&
            m_WorldPos.y < Solid->m_WorldPos.y + Solid->top*Solid->m_Transform.scale.y) &&
            ((m_WorldPos.x - left*m_Transform.scale.x < Solid->m_WorldPos.x + Solid->right*Solid->m_Transform.scale.x + 3 &&
            m_WorldPos.x > Solid->m_WorldPos.x - Solid->left*Solid->m_Transform.scale.x) ||
            (m_WorldPos.x + right*m_Transform.scale.x > Solid->m_WorldPos.x - Solid->left*Solid->m_Transform.scale.x - 3 && 
            m_WorldPos.x < Solid->m_WorldPos.x + Solid->right*Solid->m_Transform.scale.x))){
            
                if (IsContainState(c_state::clinb)){
                    SetState(c_state::clinb);
                    std::shared_ptr<Util::Animation> temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
                    temp->Play();
                }
                else{
                    InitState(c_state::clinb, {4}, {RESOURCE_DIR"/Beheaded/jump/jumpThrough_"});
                }

                m_WorldPos.y = Solid->m_WorldPos.y + Solid->top*Solid->m_Transform.scale.y + 1;
                VelocityY = 0;
                if(m_Transform.scale.x > 0){
                    m_WorldPos.x = Solid->m_WorldPos.x - Solid->left*Solid->m_Transform.scale.x + 1;
                    VelocityX = 0;
                }
                else if (m_Transform.scale.x < 0){
                    m_WorldPos.x = Solid->m_WorldPos.x + Solid->right*Solid->m_Transform.scale.x - 1;
                    VelocityX = 0;
                }
                break;
        }
    }
}



/*-----------------------------------update-----------------------------------*/
void Player::Update(){
    if (!(GetState() == c_state::clinb)){
        Move();
    }
    applyGravity();
    Clinb();
    FixPos();

    m_WorldPos.x += VelocityX;
    m_WorldPos.y += VelocityY;
}