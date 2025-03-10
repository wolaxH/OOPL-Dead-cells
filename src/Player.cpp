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
        if (VelocityX > 0){
            VelocityX -= Friction;
            if (VelocityX < 0) VelocityX = 0;
        }
        else if(VelocityX < 0){
            VelocityX += Friction;
            if (VelocityX > 0) VelocityX = 0;
        }
    }
    
    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)) VelocityY = -2;
    else if (Util::Input::IsKeyDown(Util::Keycode::UP)){
        Jump();
    }
}

void Player::Jump(){
    VelocityY = 10; //WIP
}

void Player::applyGravity(){
    if (!InGround()){
        VelocityY -= Gravity;
        if (VelocityY < -1*MaxFallSpeed) VelocityY = -1*MaxFallSpeed;
    }
    else{
        if (VelocityY < 0) VelocityY = 0;
    }
}

bool Player::InGround(){
    glm::vec2 Pos = m_Transform.translation;
    glm::vec2 other_Pos;

    float other_distance_Y, other_distance_X;

    float x, y;

    for (auto& ground : grounds){
        other_Pos = ground->GetTransform().translation;
        other_distance_Y = ground->GetScaledSize().y/2;
        other_distance_X = ground->GetScaledSize().x/2;
        x = !((Pos.x < other_Pos.x - other_distance_X-1) || (Pos.x > other_Pos.x + other_distance_X+1));
        y = (Pos.y > other_Pos.y - other_distance_Y) && (Pos.y < other_Pos.y + other_distance_Y+2);
        if (x && y) return true;
    }
    return false;
}

void Player::FixPos(std::shared_ptr<Util::GameObject> other){
    /*
    glm::vec2 Pos = m_Transform.translation;
    glm::vec2 other_Pos = other->GetTransform().translation;

    float other_distance_X = other->GetScaledSize().x/ 2;
    float other_distance_Y = other->GetScaledSize().y/ 2;
    
    bool x = ((Pos.x < other_Pos.x + other_distance_X) && (Pos.x + 20 > other_Pos.x - other_distance_X)) ||
             ((Pos.x > other_Pos.x - other_distance_X) && (Pos.x - 20 < other_Pos.x + other_distance_X));

    bool y = ((Pos.y < other_Pos.y + other_distance_Y) && (Pos.y > other_Pos.y - other_distance_Y)) ||
             ((Pos.y > other_Pos.y - other_distance_Y) && (Pos.y < other_Pos.y + other_distance_Y));
    
    if (x && y){
        if (VelocityY < 0){
            m_Transform.translation.y = other_Pos.y + other_distance_Y ;
            VelocityY = 0;
        }
        else if (VelocityY > 0 && VelocityY < 9){
            m_Transform.translation.y = other_Pos.y - other_distance_Y;
            VelocityY = 0;
        }
        else if (VelocityX > 0 && Pos.y < other_Pos.y + other_distance_Y-5){
            m_Transform.translation.x = other_Pos.x - other_distance_X - 20;
            VelocityX = 0;
        }
        else if (VelocityX < 0 && Pos.y < other_Pos.y + other_distance_Y-5){
            m_Transform.translation.x = other_Pos.x + other_distance_X + 20;
            VelocityX = 0;
        }
    }
    */
   int breakFlag = 0;
    for (auto&ground : grounds){
        breakFlag = 0;

        m_Transform.translation.x += VelocityX;
        if (IsCollsion(ground)){
            m_Transform.translation.x -= VelocityX;
            VelocityX = 0;
            breakFlag++;
        }
        m_Transform.translation.x -= VelocityX;

        m_Transform.translation.y += VelocityY;
        if (IsCollsion(ground)){
            m_Transform.translation.y -= VelocityY;
            VelocityY = 0;
            breakFlag++;
        }
        m_Transform.translation.y -= VelocityY;

        if (breakFlag == 2) break;
    }
}

void Player::Update(std::shared_ptr<Util::GameObject> other){
    Move();
    applyGravity();
    FixPos(other);
    m_Transform.translation.x += VelocityX;
    m_Transform.translation.y += VelocityY;
}

