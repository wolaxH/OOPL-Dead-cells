#ifndef MOB_HPP
#define MOB_HPP

#include "Character.hpp"

class Mob : public Character{
public:
    Mob() = default;
    ~Mob() = default;

    virtual void DetectPlayer() = 0;

private:

};


#endif