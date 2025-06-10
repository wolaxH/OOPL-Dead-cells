#ifndef ENEMYHP_HPP
#define ENEMYHP_HPP

#include "Abstract/MapObj.hpp"

#include "Util/Image.hpp"

class EnemyHP : public MapObj{
public:
    EnemyHP(int Hp, int MaxHp, const glm::vec2& enemyPos);
    ~EnemyHP() = default;

    void Update(int currentHp);

private:
    void Sync();

private:
    int m_HeartPoint;
    std::shared_ptr<MapObj> m_HpBG;
    std::shared_ptr<MapObj> m_Hp;
    const glm::vec2& m_EnemyPos;
    const int m_MaxHp;
};

#endif

