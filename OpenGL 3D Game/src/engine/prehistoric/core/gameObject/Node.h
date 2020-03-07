#ifndef NODE_H
#define NODE_H

#include <vector>
#include "engine/prehistoric/core/model/Transform.h"
#include "engine/prehistoric/common/rendering/RenderingEngine.h"

class Node
{
public:
    Node();
    virtual ~Node();

    virtual void PreInput(const float delta);
    virtual void PreUpdate(const float delta);
    virtual void PreRender(RenderingEngine* renderingEngine);

    Node* AddChild(Node* child);

    inline Transform* GetWorldTransform() const { return worldTransform; }

    inline void Move(const Vector3f& d) { worldTransform->SetPosition(worldTransform->GetPosition() + d);}
    inline void Rotate(const Vector3f& d) { worldTransform->SetRotation(worldTransform->GetRotation() + d);}
    inline void Scale(const Vector3f& d) { worldTransform->SetScaling(worldTransform->GetScaling() + d);}

    inline void SetPosition(const Vector3f& d) { worldTransform->SetPosition(d); }
    inline void SetRotation(const Vector3f& d) { worldTransform->SetRotation(d); }
    inline void SetScale(const Vector3f& d) { worldTransform->SetScaling(d); }
protected:
    std::vector<Node*> children;
    Node * parent;

    Transform* worldTransform;
    Transform* localTransform;
};

#endif