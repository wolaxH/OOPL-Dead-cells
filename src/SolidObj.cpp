#include "SolidObj.hpp"

#include "Util/Logger.hpp"

#include <fstream>



SolidObj::SolidObj(const std::string& path){
    m_Drawable = std::make_shared<Util::Image>(path);
    m_ZIndex = 10.0f;

    top = m_Drawable->GetSize().y / 2;
    bottom = top;
    left = m_Drawable->GetSize().x /2;
    right = left;
}

void SolidObj::get_data_from_json(std::string JsonFileName, int index){
    
   std::ifstream file(std::string(DATA_DIR) + "/" + JsonFileName);
   if (!file.is_open()){
       LOG_ERROR("Failed to open " + JsonFileName +" file.");
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
   top = bottom = GetScaledSize().y / 2;
   right = left = GetScaledSize().x / 2;
}

