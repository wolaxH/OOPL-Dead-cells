#ifndef DROPS_HPP
#define DROPS_HPP

#include "Abstract/MapObj.hpp"
#include "Item.hpp"


#include "Util/Text.hpp"

/**
 * Drops是掉落物的基底類別
 * 目前只有武器和卷軸兩種物品
 * 本身需要有drawable(圖片)
 */

class Drops : public MapObj{
public:
    Drops(std::string& path){
        m_Drawable = std::make_shared<Util::Image>(path);
    }

    std::shared_ptr<Item> GetItem() const {return m_Item;}
private:
    std::shared_ptr<Item> m_Item;
};

#endif