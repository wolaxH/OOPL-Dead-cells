#ifndef GROUND_HPP
#define GROUND_HPP

#include "CollsionableObj.hpp"
#include "Util/Image.hpp"


class Ground : public CollsionableObj{
public:
    Ground(const std::string& path);
    ~Ground() =  default;

};

#endif