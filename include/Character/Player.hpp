#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character/Character.hpp"
#include "Item/PickUp.hpp"
#include "Item/Weapon/Abstract/Weapon.hpp"
#include "Item/Weapon//WeaponTypes.hpp"
#include "Item/HealBottle.hpp"
#include "MyUtil/Timer.hpp"
#include "MyUtil/AtkManager.hpp"
#include "MyUtil/GameWorldContext.hpp"
#include "UI/PlayerUI.hpp"



class Player : public Character, public std::enable_shared_from_this<Player>{
public:
    Player(std::vector<std::string>& path, int Hp, GameWorldContext& World);
    ~Player() noexcept = default;

    void Init(){m_AttackManager = AttackManager(std::weak_ptr<Player>(shared_from_this()));}
    
    void Update(float dt) override;
    
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
    
    void Attacked(int Damage, glm::vec2 Dir, float Velocity = 0.0f) override;

    bool IsAtkedable() const noexcept { return m_Atkedable; }

    void Die();

    std::shared_ptr<Core::Drawable> GetDeadDrawable(){
        if (IsAlive()) return nullptr;
        return m_Drawable;
    }

    Timer m_DeathTimer;

    void AddScore(int score){
        m_score = score;
    }

    int GetScore(){return m_score;}

    void Rebrith();

    bool GetHasRebirth() {return m_Hasrebirth;}
private:
    
    void PickUp();
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
    void Move(float dt) override;
    
    /**
     * 這沒什麼好寫的同move, 且只有在 move 中call
     * 我現在眼睛好痛
     * by Etho 2025/04/17
     */
    void Jump();
    
    /**
     * 同move, 且只有在 move 中call
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

    bool IsLeaveOSP();
    
    void RequastToChangeDrawable(std::shared_ptr<Util::Animation> PD){
        ChangeDrawable(AccessKey(), PD, c_state::atk);
    }
    
    //a special function for test, development function, to Log player current position
    void TestP();
    
    /**
     * attack event for weapon
     */
    void Attack(float dt) override;

    /**
     * Block event for shield
     */
    void Block();

    void InterAct();

    void Drink();
    
private:
    friend class AttackManager;
    
    std::shared_ptr<HealBottle> m_HealBottle;
    std::shared_ptr<Item> m_Skill1 = nullptr, m_skill2 = nullptr; //武器槽
    std::shared_ptr<PlayerUI> m_PlayerINFO; //玩家UI
    AttackManager m_AttackManager; //攻擊管理器
    float m_Defense = 0; //防禦力
    std::shared_ptr<Shield> m_CurrentShield = nullptr; //當前使用的shield 如果沒有就nullptr
    
    bool m_IsAerialLock = false;       // 當前是否正在空中浮空
    bool m_ComboFloatUsed = false;     // 本次落地前是否已經使用浮空
    bool m_AerialComboStarted = false; // 是否正在空中攻擊

    bool m_HasDrunkThisHealAnim = false;

    int jumpStep = 0;   //double jump counter
    Timer timer;   //for count the roll cooling time
    bool m_Atkedable = true;

    bool m_IsDead = false;
    bool m_HasDied = false;

    bool m_Hasrebirth = false; 

    bool m_CheatingMode = false;
    int m_score;
};
#endif