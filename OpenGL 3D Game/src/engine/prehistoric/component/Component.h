#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Engine;

#include "engine/prehistoric/core/gameObject/Node.h"

typedef void(*UpdateFunction)(GameObject*, float); //The object, and the delta time

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
	UpdateFunction updateFunction; //An optional script for every component to run, which does not apply to a component type but a specific component, like the sun's movement
};

#endif