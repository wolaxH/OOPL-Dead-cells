#ifndef SOLIDOBJ_HPP
#define SOLIDOBJ_HPP

#include "MapObj.hpp"
#include "Util/Image.hpp"


class SolidObj : public MapObj{
public:
    SolidObj(const std::string& path);
    ~SolidObj() =  default;

};

#endif