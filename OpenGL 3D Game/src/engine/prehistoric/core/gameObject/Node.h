#ifndef NODE_H
#define NODE_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "engine/prehistoric/core/model/transform.h"
#include "engine/prehistoric/common/rendering/Renderer.h"

class Engine;

class Node
{
public:
    Node() : parent(nullptr) {}
    //Node(const Node& node);
    //Node(Node&& node);

    virtual ~Node();

    virtual void PreUpdate(Engine* engine);
    virtual void PreRender(Renderer* renderer);

    Node* AddChild(const std::string& key, Node* child);

    inline Transform getWorldTransform() const { return worldTransform; }

    inline std::unordered_map<std::string, Node*> getChildren() const { return children; }

    inline void Move(const Vector3f& d) { worldTransform.setPosition(worldTransform.getPosition() + d);}
    inline void Rotate(const Vector3f& d) { worldTransform.setRotation(worldTransform.getRotation() + d);}
    inline void Scale(const Vector3f& d) { worldTransform.setScaling(worldTransform.getScaling() + d);}

    inline void SetPosition(const Vector3f& d) { worldTransform.setPosition(d); }
    inline void SetRotation(const Vector3f& d) { worldTransform.setRotation(d); }
    inline void SetScale(const Vector3f& d) { worldTransform.setScaling(d); }

    //Node& operator=(Node node);
protected:
    std::unordered_map<std::string, Node*> children;
    Node* parent;

    Transform worldTransform;
};

#endif