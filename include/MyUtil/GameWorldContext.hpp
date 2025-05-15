#ifndef GAMEWORLDCONTEXT_HPP
#define GAMEWORLDCONTEXT_HPP

#include "pch.hpp"
#include "SolidObj.hpp"
#include "OneSidedPlatform.hpp"
#include "MyUtil/RemovableManager.hpp"

class GameWorldContext{
public:
    GameWorldContext(std::vector<std::shared_ptr<MapObj>>& MapObjs){
        Mobs = std::make_shared<RemovableManager>(MapObjs);
        WorldDrops = std::make_shared<RemovableManager>(MapObjs);
    }
    ~GameWorldContext() = default;
    

    std::vector<std::shared_ptr<SolidObj>> SolidObjs = {};
    std::vector<std::shared_ptr<OneSidedPlatform>> OneSidedPlatforms = {};
    std::shared_ptr<RemovableManager> Mobs;
    std::shared_ptr<RemovableManager> WorldDrops;
};

#endif