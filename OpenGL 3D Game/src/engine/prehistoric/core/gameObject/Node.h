#ifndef NODE_H
#define NODE_H

#include "engine/prehistoric/core/util/Includes.hpp"
#include "engine/prehistoric/core/model/Transform.h"
#include "engine/prehistoric/common/rendering/RenderingEngine.h"

class Engine;

class Node
{
public:
    Node();
    virtual ~Node();

    virtual void PreUpdate(Engine* engine);
    virtual void PreRender(RenderingEngine* renderingEngine);

    Node* AddChild(const std::string& key, Node* child);

    inline Transform* getWorldTransform() const { return worldTransform; }

    inline std::unordered_map<std::string, Node*> GetChildren() const { return children; }

    inline void Move(const Vector3f& d) { worldTransform->SetPosition(worldTransform->GetPosition() + d);}
    inline void Rotate(const Vector3f& d) { worldTransform->SetRotation(worldTransform->GetRotation() + d);}
    inline void Scale(const Vector3f& d) { worldTransform->SetScaling(worldTransform->GetScaling() + d);}

    inline void SetPosition(const Vector3f& d) { worldTransform->SetPosition(d); }
    inline void SetRotation(const Vector3f& d) { worldTransform->SetRotation(d); }
    inline void SetScale(const Vector3f& d) { worldTransform->SetScaling(d); }
protected:
    std::unordered_map<std::string, Node*> children;
    Node * parent;

    Transform* worldTransform;
};

#endif