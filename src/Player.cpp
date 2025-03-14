#include "Player.hpp"

#include "Util/Input.hpp"

Player::Player(std::vector<std::string>& path, int Hp) : Character(path, Hp){
    m_Transform.scale = {1.5f, 1.5f};
}

void Player::InitState(c_state state, const std::vector<std::size_t>& frames, const std::vector<std::string>& paths){

    std::vector<std::string> Img;
    std::vector<std::string> temp;

    for (std::size_t i =0; i < frames.size(); i++){
        temp.clear();
        for (std::size_t j = 0; j < frames[i]; j++){temp.push_back(paths[i] + std::to_string(j) + ".png");}
        Img.insert(Img.end(), temp.begin(), temp.end());
    }

    if (state == c_state::jump || state == c_state::fall) SetState(state, Img, false);
    else SetState(state, Img);
}

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
        
        // idle
        if (InGround()){
            //  set state
            if (GetState() != c_state::idle) SetState(c_state::idle);
    
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

void Player::Jump(){    //WIP add double junp anima
    if (InGround()) jumpStep = 0;   
    if (jumpStep == 2) return;

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

void Player::applyGravity(){    //WIP add fall anima and state (may in Move func)
    if (!InGround()){
        VelocityY -= Gravity;
        //if (VelocityY <= 0){}
        if (VelocityY < -1*MaxFallSpeed) VelocityY = -1*MaxFallSpeed;
    }
    else{
        if (VelocityY < 0) VelocityY = 0;
    }
}

void Player::FixPos(){
    int breakFlag = 0;
    
    for (auto&CollsionableObj : CollsionableObjs){
        breakFlag = 0;
        
        m_WorldPos.x += VelocityX;
        if (IsCollsion(CollsionableObj)){
            m_WorldPos.x -= VelocityX;
            VelocityX = 0;
            breakFlag++;
        }
        m_WorldPos.x -= VelocityX;
        
        m_WorldPos.y += VelocityY;
        if (IsCollsion(CollsionableObj)){
            m_WorldPos.y -= VelocityY;
            VelocityY = 0;
            breakFlag++;
        }
        m_WorldPos.y -= VelocityY;
        
        if (breakFlag == 2) break;
    }
}

bool Player::InGround(){

    glm::vec2 Pos = m_WorldPos;
    glm::vec2 other_Pos;

    glm::vec2 other_distance;
    float x, y;

    for (auto& CollsionableObj : CollsionableObjs){
        other_Pos = CollsionableObj->m_WorldPos;
        other_distance = CollsionableObj->GetScaledSize() / 2.0f;

        x = !((Pos.x < other_Pos.x - other_distance.x - 1) || (Pos.x > other_Pos.x + other_distance.x + 1));
        y = (Pos.y > other_Pos.y - other_distance.y) && (Pos.y < other_Pos.y + other_distance.y + 2);
        if (x && y) return true;
    }
    return false;
}

void Player::Update(){
    Move();
    applyGravity();
    FixPos();

    m_WorldPos.x += VelocityX;
    m_WorldPos.y += VelocityY;
}