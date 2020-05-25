#include "engine/prehistoric/core/util/Includes.hpp"
#include "Node.h"

#include "engine/prehistoric/core/Engine.h"

Node::Node()
{
    parent = nullptr;

    worldTransform = new Transform();
}

Node::~Node()
{
    for (auto child : children)
    {
        delete child.second;
    }

    delete worldTransform;
}

void Node::PreUpdate(Engine* engine)
{
    for (auto child : children)
    {
        child.second->PreUpdate(engine);
    }
}

void Node::PreRender(RenderingEngine* renderingEngine)
{
    for (auto child : children)
    {
        child.second->PreRender(renderingEngine);
    }
}

Node* Node::AddChild(const std::string& key, Node* child)
{
    child->parent = this;
    children.emplace(key, child);
    return this;
}