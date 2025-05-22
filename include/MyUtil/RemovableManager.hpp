#ifndef REMOVABLEMANAGER_HPP
#define REMOVABLEMANAGER_HPP

#include "Util/GameObject.hpp"
#include "Abstract/MapObj.hpp"

class RemovableManager : public Util::GameObject{
public:
    RemovableManager(std::vector<std::shared_ptr<MapObj>>& MapObjs) : m_MapObjs(MapObjs){}
    ~RemovableManager() = default;


    void AddObj(const std::shared_ptr<Util::GameObject> Obj){
        if (Obj){
            AddChild(Obj);
            auto temp = std::dynamic_pointer_cast<MapObj>(Obj);
            if (temp) m_MapObjs.push_back(temp);
        }
    }

    void RemoveObj(std::shared_ptr<Util::GameObject> Obj){
        if (Obj){
            RemoveChild(Obj);
            auto temp = std::dynamic_pointer_cast<MapObj>(Obj);
            if (temp){
                auto it = std::find(m_MapObjs.begin(), m_MapObjs.end(), temp);
                if (it != m_MapObjs.end()) m_MapObjs.erase(it);
            }
        }
    }

    std::vector<std::shared_ptr<Util::GameObject>>& GetObjs() noexcept{return m_Children;}

private:
    std::vector<std::shared_ptr<MapObj>>& m_MapObjs;
};

#endif