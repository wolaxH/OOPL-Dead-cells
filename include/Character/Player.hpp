#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character/Character.hpp"
#include"MyUtil/Timer.hpp"


class Player : public Character{
public:
    Player(std::vector<std::string>& path, int Hp, 
        const std::vector<std::shared_ptr<SolidObj>>& SolidObjs, 
        const std::vector<std::shared_ptr<OneSidedPlatform>>& OSP);
    ~Player() noexcept = default;

    void Attack() override;

    void Update() override;

    void Attacked(int damage){Hp -= damage;}

    //for mob push
    void Pushed(){ 
        if (GetState() == c_state::roll) return;    //翻滾狀態不回被怪物推動減少速度

        if (VelocityX > 0){
            VelocityX *= 0.5f;
            if (VelocityX < 5) VelocityX = 5.0f;
        }
        else if (VelocityX < 0){
            VelocityX *= 0.5f;
            if (VelocityX > -5) VelocityX = -5.0f;
        }
    }
    
private:
    /**
     * 設置c_state 跟 Rednering
     * 向所面相的方向移動一段距離 (set VelocityX)
     * 結束時候設置c_state = idle
     * 可被打斷
     */
    void roll();

    /**
     * setPos
     * 當動畫撥放完之前不能move
     * 動畫播完後c_state = idle
     */
    void Clinb();   //攀爬

    /**
     * set Velocity
     * also set c_state
     * clinb and roll 等特殊狀態期間無法move
     */
    void Move() override;

    /**
     * 這沒什麼好寫的同move, 且只有在 move 中call
     * 我現在眼睛好痛
     * by Etho 2025/04/17
     */
    void Jump();

    //Util function ; OSP mean OneSidedPlatform
    /**
     * return ture if player stand on any OSP
     */
    bool IsOnOSP();
    /**
     * return true if player under any OSP, and the padding of OSP is 5px
     */
    bool IsUnderOSP();


    //a special function for test, development function, to Log player current position
    void TestP();

private:
    
    int jumpStep = 0;   //double jump counter
    Timer timer;   //for count the roll cooling time
};
#endif