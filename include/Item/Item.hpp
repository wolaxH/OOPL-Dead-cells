#ifndef ITEM_HPP
#define ITEM_HPP

#include "Util/Image.hpp"
#include "Util/GameObject.hpp"

/**
 * Item 代表被玩家撿起的物品
 * 不隨camera移動
 * 因此是GameObject的子類別
 */
class Item : public Util::GameObject{
public:
    Item(std::string& path){
        m_Drawable = std::make_shared<Util::Image>(path);
    }

protected:

    /**
     * Describe與DescribeBox需要依照物品的種類來改變
     * 故不在Item 類別中定義
     */
    std::shared_ptr<Util::Text> m_Describe;
    std::shared_ptr<Util::Image> m_DescribeBox;
};

#endif