#ifndef PICKUP_HPP
#define PICKUP_HPP

#include "Abstract/MapObj.hpp"
#include "MyUtil/GameWorldContext.hpp"
#include "MyUtil/Physics.hpp"

#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Logger.hpp"

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
        m_ZIndex = 20.0f;
        m_Transform.scale = {2, 2};
    }

    void Update(){

        IsOnGround = Physics::IsOnGround(m_WorldPos, m_World->SolidObjs, m_World->OneSidedPlatforms);
        Physics::ApplyGravity(VelocityY, IsOnGround, Gravity, MaxFallSpeed);

        m_WorldPos.y += VelocityY;
        FixPos();
    }

    void FixPos() {

        std::vector<std::shared_ptr<SolidObj>> temp(m_World->SolidObjs);
        temp.insert(temp.end(), m_World->OneSidedPlatforms.begin(), m_World->OneSidedPlatforms.end());
        for (const auto& solid : temp) {
            glm::vec2 solidPos = solid->m_WorldPos;
            glm::vec2 solidSize = abs(solid->GetScaledSize());
    
            bool inXRange = m_WorldPos.x >= solidPos.x - solid->left * solidSize.x &&
                            m_WorldPos.x <= solidPos.x + solid->right * solidSize.x;
    
            bool hittingTop = m_WorldPos.y >= solidPos.y - solid->bottom * solidSize.y &&
                              m_WorldPos.y <= solidPos.y + solid->top * solidSize.y;

    
            if (inXRange && hittingTop && VelocityY < 0) {
                LOG_DEBUG("point");
                m_WorldPos.y = solidPos.y + solidSize.y / 2;
                VelocityY = 0;
                return;
            }
        }
    }    
    
    void PopUpDescribeBox(){ m_Item->SetDescribeVisible(true);}

    void HideDescribeBox(){ m_Item->SetDescribeVisible(false);}

    std::shared_ptr<Item> ToItem() {return m_Item;}
    
    static GameWorldContext* m_World;
private:
    std::shared_ptr<Item> m_Item;
    bool IsOnGround = false;
    float VelocityY = 0;
    const float Gravity = 0.8f;
    const float MaxFallSpeed = 20.0f;
};  // Drops



#endif // PICKUP_HPP