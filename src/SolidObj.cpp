#include "SolidObj.hpp"

#include "Util/Logger.hpp"

#include <fstream>

/*

*/
void from_json(const nlohmann::json& j, glm::vec2& v) {
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
}

SolidObj::SolidObj(const std::string& path){
    m_Drawable = std::make_shared<Util::Image>(path);
    m_ZIndex = 10.0f;
    m_Transform.scale = {1.0f, 0.001f};
    m_Transform.translation = {0.0f, -260.0f};
    m_WorldPos = m_Transform.translation;

    top = m_Drawable->GetSize().y / 2;
    bottom = top;
    left = m_Drawable->GetSize().x /2;
    right = left;
}

void SolidObj::get_data_from_json(int index){
    /*
    
    */
   std::ifstream file(RESOURCE_DIR"/SolidObjs.json");
   if (!file.is_open()){
       LOG_ERROR("Failed to open SolidObjs.json file.");
       return;
   }
   nlohmann::json j;
   file >> j;
   
   if (index < 0 || index >= j.size()){
       LOG_ERROR("索引超出範圍");
       return;
   }

   from_json(j[index]["scale"], this->m_Transform.scale);
   from_json(j[index]["pos"], this->m_WorldPos);
   
}

