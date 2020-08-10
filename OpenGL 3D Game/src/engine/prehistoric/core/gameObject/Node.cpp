#include "engine/prehistoric/core/util/Includes.hpp"
#include "Node.h"

#include "engine/prehistoric/core/Engine.h"

Node::Node(const Node& node)
{    
    std::string name = "\0";
    for (auto& child : parent->children)
    {
        if (child.second.get() == &node)
        {
            name = child.first;
        }
    }

    if (name == "\0")
    {
        PR_LOG_ERROR("ERROR: The name of node (at %x) couldn't be aquired!\n", &node);
    }

    node.parent->AddChild(name + "_cpy", this);
}

void Node::PreUpdate(Engine* engine)
{
    for (auto& child : children)
    {
        child.second->PreUpdate(engine);
    }
}

void Node::PreRender(Renderer* renderer)
{
    for (auto& child : children)
    {
        child.second->PreRender(renderer);
    }
}

Node* Node::AddChild(const std::string& key, Node* child)
{
    child->parent = this;
    children.emplace(key, child);
    return this;
}