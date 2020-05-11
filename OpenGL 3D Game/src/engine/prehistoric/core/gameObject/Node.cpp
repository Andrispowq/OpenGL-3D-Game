#include "engine/prehistoric/core/util/Includes.hpp"
#include "Node.h"

Node::Node()
{
    parent = nullptr;

    worldTransform = new Transform();
    localTransform = new Transform();
}

Node::~Node()
{
    for (auto child : children)
    {
        delete child.second;
    }

    delete worldTransform;
    delete localTransform;
}

void Node::PreInput(const float delta)
{
    for (auto child : children)
    {
        child.second->PreInput(delta);
    }
}

void Node::PreUpdate(const float delta)
{
    for (auto child : children)
    {
        child.second->PreUpdate(delta);
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