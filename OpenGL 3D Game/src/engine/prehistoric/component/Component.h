#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Engine;

#include "engine/prehistoric/core/gameObject/Node.h"

class Component
{
public:
	Component() : parent(nullptr) {}
	virtual ~Component() {}

	virtual void PreUpdate(Engine* engine) {}
	virtual void PreRender(RenderingEngine* renderingEngine) {}

	inline GameObject* GetParent() const { return parent; }
	inline void SetParent(GameObject* parent) { this->parent = parent; }
protected:
	GameObject* parent;
};

#endif