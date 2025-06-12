#ifndef REMOVABLEMANAGER_HPP
#define REMOVABLEMANAGER_HPP

#include "Util/GameObject.hpp"
#include "Abstract/MapObj.hpp"
#include <functional>

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

    void RemoveObjs(const std::function<bool(const std::shared_ptr<Util::GameObject>&)>& predicate){
        std::vector<std::shared_ptr<Util::GameObject>> toRemove;
        for (auto& obj : m_Children) {
            if (predicate(obj)) {
                toRemove.push_back(obj);
            }
        }
        for (auto& obj : toRemove) {
            RemoveObj(obj);
        }
    }

    /**
     * This method will NOT remove any Obj or ref in Mapobj
     * only remove all the Children 
     */
    void Clear(){
        for (auto Obj : m_Children){
            auto temp = std::dynamic_pointer_cast<MapObj>(Obj);
            if (temp){
                auto it = std::find(m_MapObjs.begin(), m_MapObjs.end(), temp);
                if (it != m_MapObjs.end()) m_MapObjs.erase(it);
            }
        }
        m_Children.clear();
    }

    std::vector<std::shared_ptr<Util::GameObject>>& GetObjs() noexcept {return m_Children;}

private:
    std::vector<std::shared_ptr<MapObj>>& m_MapObjs;
};

#endif