#ifndef SOLIDOBJ_HPP
#define SOLIDOBJ_HPP

#include "MapObj.hpp"
#include "Util/Image.hpp"

#include "json.hpp"

class SolidObj : public MapObj{
public:
    SolidObj(const std::string& path);
    ~SolidObj() =  default;

    void get_data_from_json(int  index = 0);
};

#endif