#ifndef ONESIDEDPLATFORM_HPP
#define ONESIDEDPLATFORM_HPP

#include "SolidObj.hpp"

class OneSidedPlatform : public SolidObj{
public:
    OneSidedPlatform(const std::string& path) : SolidObj(path){}
    ~OneSidedPlatform() = default;

};

#endif // ONESIDEDPLATFORM_HPP