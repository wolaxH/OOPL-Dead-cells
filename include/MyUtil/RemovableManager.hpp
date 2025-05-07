#ifndef REMOVABLEMANAGER_HPP
#define REMOVABLEMANAGER_HPP

#include "Util/GameObject.hpp"

class RemovableManager : public Util::GameObject{
public:
    RemovableManager()  =  default;
    ~RemovableManager() = default;


    void AddObj(const std::shared_ptr<Util::GameObject>& Obj){if (Obj) AddChild(Obj);}

    void RemoveObj(std::shared_ptr<Util::GameObject> Obj){if (Obj) RemoveChild(Obj);}

    std::vector<std::shared_ptr<Util::GameObject>> GetObjs() const noexcept{return m_Children;}
};

#endif