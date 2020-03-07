#include "Node.h"

Node::Node()
{
    parent = nullptr;

    worldTransform = new Transform();
    localTransform = new Transform();
}

Node::~Node()
{
    for (unsigned int i = 0; i < children.size(); i++)
        delete children[i];

    delete worldTransform;
    delete localTransform;
}

void Node::PreInput(const float delta)
{
    for(unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->PreInput(delta);
    }
}

void Node::PreUpdate(const float delta)
{
    for (unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->PreUpdate(delta);
    }
}

void Node::PreRender(RenderingEngine* renderingEngine)
{
    for (unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->PreRender(renderingEngine);
    }
}

Node* Node::AddChild(Node* child)
{
    child->parent = this;
    children.push_back(child);    
    return this;
}