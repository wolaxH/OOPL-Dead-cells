#ifndef BOSSHP_HPP
#define BOSSHP_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class BossHp : public Util::GameObject{
public:
    BossHp(int Hp);
    ~BossHp() = default;

    void Update(int Hp);

    void Pop(){
        m_HpBar->SetVisible(true);
        m_HpBg->SetVisible(true);
    }

    void Hide(){
        m_HpBar->SetVisible(false);
        m_HpBg->SetVisible(false);
    }

private:
    int m_MaxHp;
    int m_Hp;
    std::shared_ptr<Util::GameObject> m_HpBar;
    std::shared_ptr<Util::GameObject> m_HpBg;
};

#endif