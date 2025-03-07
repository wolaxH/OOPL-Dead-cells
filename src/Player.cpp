#include "Player.hpp"

#include "Util/Input.hpp"

Player::Player(std::vector<std::string>& path, int Hp) : Character(path, Hp){
    m_Transform.scale = {1.5f, 1.5f};
}

void Player::Move(){
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)){    //press right
        
        //turn the player Img
        m_Transform.scale.x = -1.5; 

        //  change the state or add a new one
        if (GetState() != c_state::L_move){
            if (IsContainState(c_state::L_move)){SetState(c_state::L_move);}
            else{
                std::vector<std::string> Img;
                Img.reserve(20);
                for (int i = 0; i < 20; i++){
                    //turn
                    Img.push_back(RESOURCE_DIR"/Beheaded/runA/runA_" + std::to_string(i) + ".png");
                }
                SetState(c_state::L_move, Img);
            }
        }

        //  chaenge pos
        if (VelocityX > -1*MaxSpeed) VelocityX += -1*AccelerationX;
        else if (VelocityX < -1*MaxSpeed) VelocityX = -1*MaxSpeed;
        
    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)){  //press right
        
        //turn the player Img
        m_Transform.scale.x = 1.5;
        
        //  change the state or add a new one
        if (GetState() != c_state::R_move){
            if (IsContainState(c_state::R_move)){SetState(c_state::R_move);}
            else{
                std::vector<std::string> Img;
                Img.reserve(16);
                for (int i = 0; i < 16; i++){
                    Img.push_back(RESOURCE_DIR"/Beheaded/runB/runB_" + std::to_string(i) + ".png");
                }
                SetState(c_state::R_move, Img);
            }
        }
        
        //  set Velocity
        if (VelocityX < MaxSpeed) VelocityX += AccelerationX;
        else if (VelocityX > MaxSpeed) VelocityX = MaxSpeed;
    }
    else{   //do nothing
        
        //  set state
        if (GetState() != c_state::idle){
            SetState(c_state::idle);
        }

        //slow down
        if (VelocityX == 0){
            return;
        }
        else if (VelocityX > 0){
            VelocityX -= Friction;
            if (VelocityX < 0) VelocityX = 0;
        }
        else if(VelocityX < 0){
            VelocityX += Friction;
            if (VelocityX > 0) VelocityX = 0;
        }
    }
    m_Transform.translation.x += VelocityX;
}

void Player::Jump(){

    
}

void Player::Update(){
    Move();
    if (Util::Input::IsKeyPressed(Util::Keycode::UP)){
        Jump();
    }
}

