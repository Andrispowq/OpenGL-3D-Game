#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

#include "engine/prehistoric/core/gameObject/Node.h"

class Component
{
public:
	Component() : parent(nullptr) {}
	virtual ~Component() {}

	virtual void PreInput(const float delta) {}
	virtual void PreUpdate(const float delta) {}
	virtual void PreRender(RenderingEngine* renderingEngine) {}

	inline GameObject* GetParent() const { return parent; }
	inline void SetParent(GameObject* parent) { this->parent = parent; }
protected:
	GameObject* parent;
};

#endif