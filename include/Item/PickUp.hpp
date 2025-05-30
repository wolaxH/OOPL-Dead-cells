#ifndef PICKUP_HPP
#define PICKUP_HPP

#include "Abstract/MapObj.hpp"
#include "MyUtil/GameWorldContext.hpp"

#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

class Drops;
class Item;

/**
 * Item 代表被玩家撿起的物品
 * 不隨camera移動
 * 因此是GameObject的子類別
 */
class Item : public Util::GameObject, public std::enable_shared_from_this<Item>{
public:
    Item(const std::string& path){
        m_Drawable = std::make_shared<Util::Image>(path);
        
        auto I = std::make_shared<Util::Image>(RESOURCE_DIR"/UI/notifBox.png");
        m_DescribeBox = std::make_shared<Util::GameObject>(I, 15.2f);
        m_DescribeBox->SetVisible(false);
        
        //建立DescribeBox 跟 Describe
        AddChild(m_DescribeBox);
        AddChild(m_Describe);
    }
    virtual ~Item() = default;
    
    std::shared_ptr<Drops> ToDrops() {return std::make_shared<Drops>(shared_from_this());}

    auto GetIcon() const {return m_Drawable;}
    
    void SetDescribeVisible(bool visible){
        m_Describe->SetVisible(visible);
        m_DescribeBox->SetVisible(visible);
    }
    
protected:
    friend class Drops;
    /**
     * Describe與DescribeBox需要依照物品的種類來改變
     * 故只在Item 類別中宣告
     */
    //Text
    std::shared_ptr<Util::GameObject> m_Describe;
    //Image
    std::shared_ptr<Util::GameObject> m_DescribeBox;
};  // Item


/**
 * Drops是掉落物的基底類別
 * 目前只有武器和卷軸兩種物品
 * 本身需要有drawable(圖片)
 */
class Drops : public MapObj{
public:
    Drops(std::shared_ptr<Item> item) : 
        m_Item(item){
        m_Drawable = item->m_Drawable;
        m_Transform.scale = {2, 2};
        
    }

    void PopUpDescribeBox(){ m_Item->SetDescribeVisible(true);}

    void HideDescribeBox(){ m_Item->SetDescribeVisible(false);}

    std::shared_ptr<Item> ToItem() {return m_Item;}
    
    static GameWorldContext* m_World;
private:
    std::shared_ptr<Item> m_Item;
};  // Drops



#endif // PICKUP_HPP