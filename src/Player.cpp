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



/*-----------------------------------update-----------------------------------*/
void Player::Update(){
    Move();
    applyGravity();
    FixPos();

    m_WorldPos.x += VelocityX;
    m_WorldPos.y += VelocityY;
}