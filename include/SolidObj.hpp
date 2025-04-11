#ifndef SOLIDOBJ_HPP
#define SOLIDOBJ_HPP

#include "Abstract/MapObj.hpp"
#include "Util/Image.hpp"

#include "json.hpp"

class SolidObj : public MapObj{
public:
    SolidObj(const std::string& path);
    ~SolidObj() =  default;

    void get_data_from_json(int  index = 0);
private:
    void from_json(const nlohmann::json& j, glm::vec2& v) {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
    }
};

#endif