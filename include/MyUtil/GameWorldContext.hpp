#ifndef GAMEWORLDCONTEXT_HPP
#define GAMEWORLDCONTEXT_HPP

#include "pch.hpp"
#include "SolidObj.hpp"
#include "OneSidedPlatform.hpp"
#include "InterAct/IInterAct.hpp"
#include "MyUtil/RemovableManager.hpp"
#include "MyUtil/Camera.hpp"

#include "Util/Logger.hpp"

class Player;

class GameWorldContext{
public:
    GameWorldContext(std::vector<std::shared_ptr<MapObj>>& MapObjs, Camera& camera)
     : m_Camera(camera), m_MapObjs(MapObjs){
        Mobs = std::make_shared<RemovableManager>(MapObjs);
        WorldDrops = std::make_shared<RemovableManager>(MapObjs);
        Projectiles = std::make_shared<RemovableManager>(MapObjs);
    }
    ~GameWorldContext() = default;

    void AddObj(std::shared_ptr<MapObj> Obj){
        if (Obj == nullptr) return;
        for (auto& temp : Obj->GetChildren()){
            std::shared_ptr<MapObj> mapobj = std::dynamic_pointer_cast<MapObj>(temp);
            if (temp) AddObj(mapobj);
        }
        m_MapObjs.push_back(Obj);
    }

    void RemoveObj(std::shared_ptr<MapObj> Obj){
        if (Obj == nullptr) return;
        auto it = std::find(m_MapObjs.begin(), m_MapObjs.end(), Obj);
        if (it != m_MapObjs.end()) m_MapObjs.erase(it);
    }

    void TriggerShake(float duration, float strength){
        m_Camera.TriggerShake(duration, strength);
    }

    void Clear(){
        InterActAbles.clear();
        Mobs->Clear();
        WorldDrops->Clear();
        Projectiles->Clear();
        m_Player.reset();
    }

    std::weak_ptr<Player> m_Player;
    std::vector<std::shared_ptr<SolidObj>> SolidObjs = {};
    std::vector<std::shared_ptr<OneSidedPlatform>> OneSidedPlatforms = {};
    std::vector<std::shared_ptr<IInterAct>> InterActAbles = {};
    std::shared_ptr<RemovableManager> Mobs;
    std::shared_ptr<RemovableManager> WorldDrops;
    std::shared_ptr<RemovableManager> Projectiles;
private:
    Camera& m_Camera;
    std::vector<std::shared_ptr<MapObj>>& m_MapObjs;
};

#endif