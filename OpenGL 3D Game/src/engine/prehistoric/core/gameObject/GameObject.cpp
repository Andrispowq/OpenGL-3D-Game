#include "engine/prehistoric/core/util/Includes.hpp"
#include "GameObject.h"

GameObject::~GameObject()
{
	for (auto kv : components)
	{
		delete kv.second;
	}
}

void GameObject::PreInput(const float delta)
{
	for (auto kv : components)
	{
		kv.second->PreInput(delta);
	}

	Node::PreInput(delta);
}

void GameObject::PreUpdate(const float delta)
{
	for (auto kv : components)
	{
		kv.second->PreUpdate(delta);
	}

	Node::PreUpdate(delta);
}

void GameObject::PreRender(RenderingEngine* renderingEngine)
{
	for (auto kv : components)
	{
		kv.second->PreRender(renderingEngine);
	}

	Node::PreRender(renderingEngine);
}

GameObject* GameObject::AddComponent(const std::string& name, Component* component)
{
	component->SetParent(this);
	components.emplace(name, component);
	return this;
}

Component* GameObject::GetComponent(const std::string& name) const
{
	return components.at(name);
}