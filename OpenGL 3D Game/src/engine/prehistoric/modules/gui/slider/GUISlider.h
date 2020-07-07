#ifndef GUI_SLIDER_H
#define GUI_SLIDER_H

#include "engine/prehistoric/modules/gui/GUIElement.h"

class GUISlider : public GUIElement
{
public:
	GUISlider(Window* window, Texture* texture = nullptr, void* data = nullptr, size_t dataSize = 0, bool visible = true);

	virtual void PreUpdate(Engine* engine) override;
};

#endif