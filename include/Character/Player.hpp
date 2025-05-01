#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character/Character.hpp"
#include"MyUtil/Timer.hpp"
#include "Item/PickUp.hpp"
#include "Item/Weapon/Weapon.hpp"
#include "UI/PlayerUI.hpp"



class Player : public Character{
public:
    Player(std::vector<std::string>& path, int Hp, 
        const std::vector<std::shared_ptr<SolidObj>>& SolidObjs, 
        const std::vector<std::shared_ptr<OneSidedPlatform>>& OSP,
        std::vector<std::shared_ptr<Drops>>& Drops);
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
    
    void PickUp();

    /**Get item logic
     * * 1. 玩家靠近掉落物      complete
     * * 2. 掉落物出現撿取提示
     * * 3. 玩家按下R           complete
     * * 4. 根據掉落物的物品觸發不同邏輯
     * * * 4.1. 若為武器且武器slot滿了需要彈出更換視窗，且被替換的武器需要變回掉落物    complete
     * * * 4.2. 若為卷軸則直接使用
     * * 5. 掉落物消失     complete
     */
    bool IsNearbyDrops(std::shared_ptr<Drops> drops);

    /**
     * When Press R, call this function
     */
    void PickUpDrops(std::shared_ptr<Drops> drops);

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
    void Clinb();

    /**
     * 
     */
    void ClinbOSP();

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

    /**
     * 這沒什麼好寫的同move, 且只有在 move 中call
     * only set or init state
     * 往下的功能由applyGravity完成
     */
    void fall();

    //Util function ; OSP mean OneSidedPlatform
    /**
     * return ture if player stand on any OSP
     */
    bool IsOnOSP();
  
    /**
     * return true if player under any OSP, and the padding of OSP is 5px
     */
    bool IsUnderOSP();


    /**
     * To make player slowdown
     */
    void SlowDown();


    //a special function for test, development function, to Log player current position
    void TestP();

private:
    std::shared_ptr<Weapon> m_Weapon1 = nullptr, m_Weapon2 = nullptr; //武器槽
    std::shared_ptr<PlayerUI> m_PlayerINFO; //玩家UI

    std::vector<std::shared_ptr<Drops>>& r_WorldDrops;
    int jumpStep = 0;   //double jump counter
    Timer timer;   //for count the roll cooling time
};
#endif