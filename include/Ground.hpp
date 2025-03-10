#ifndef GROUND_HPP
#define GROUND_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"


class Ground : public Util::GameObject{
public:
    Ground(const std::string& path);
    ~Ground() =  default;

};

#endif