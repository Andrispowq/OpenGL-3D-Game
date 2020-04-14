#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "engine/prehistoric/component/gui/GUIElement.h"

class GUIButton : public GUIElement
{
public:
	GUIButton(Window* window, Texture* texture = nullptr, void* data = nullptr, size_t dataSize = 0, bool visible = true)
		: GUIElement(window, texture, data, dataSize, visible) {}

	virtual void PreUpdate(const float delta) override;
};

#endif