#include "MyUtil/Collision.hpp"

bool Collision::IsIntersect(const MapObj* a, const MapObj* b) {
    glm::vec2 Pos = a->m_WorldPos;
    glm::vec2 other_Pos = b->m_WorldPos;
    
    glm::vec2 other_scale = abs(b->GetTransform().scale);
    glm::vec2 scale = abs(a->GetTransform().scale); 
    float t_right = a->right * scale.x,  //temp borger
        t_left = a->left * scale.x,
        t_top = a->top * scale.y,
        t_bottom = a->bottom * scale.y;

    float other_right = b->right * other_scale.x,
        other_left = b->left * other_scale.x,
        other_top = b->top * other_scale.y,
        other_bottom = b->bottom * other_scale.y;

    bool x = ((Pos.x < other_Pos.x + other_right) && (Pos.x + t_right > other_Pos.x - other_left)) ||
            ((Pos.x > other_Pos.x - other_left) && (Pos.x - t_left < other_Pos.x + other_right));

    bool y = ((Pos.y < other_Pos.y + other_top) && (Pos.y + t_top > other_Pos.y - other_bottom)) ||
            ((Pos.y > other_Pos.y - other_bottom) && (Pos.y - t_bottom < other_Pos.y + other_top));

    return x && y;    
}

bool Collision::IsIntersectAABB(const AABB& a, const AABB& b) {
    bool x = a.left < b.right && a.right > b.left;
    bool y = a.top > b.bottom && a.bottom < b.top;
    return x && y;
}

bool Collision::IsIntersectAABB(const MapObj* a, const MapObj* b){
    AABB a_AABB = GetAABB(a);
    AABB b_AABB = GetAABB(b);

    return IsIntersectAABB(a_AABB, b_AABB);
}

Collision::AABB Collision::GetAABB(const MapObj* obj){
    return{
        obj->m_WorldPos.x - obj->left,
        obj->m_WorldPos.y + obj->top,
        obj->m_WorldPos.x + obj->right,
        obj->m_WorldPos.y - obj->bottom
    };
}