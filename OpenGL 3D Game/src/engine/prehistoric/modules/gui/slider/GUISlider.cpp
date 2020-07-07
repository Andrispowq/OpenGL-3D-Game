#include "engine/prehistoric/core/util/Includes.hpp"
#include "GUISlider.h"

GUISlider::GUISlider(Window* window, float minValue, float maxValue, Texture* texture, void* data, size_t dataSize, bool visible)
	: GUIElement(window, texture, data, dataSize, visible), minValue(minValue), maxValue(maxValue), progress(0.5f)
{
	type = GUIType::Slider;
}

void GUISlider::PreUpdate(Engine* engine)
{
	float* _data = (float*)data;

	if (InputInstance.IsButtonHeld(PR_MOUSE_BUTTON_LEFT) && inside(InputInstance.GetCursorPosition()))
	{
		if (cursorOldPositionX == -1)
		{
			cursorOldPositionX = InputInstance.GetCursorPosition().x;
		}
		else
		{
			float newPosition = InputInstance.GetCursorPosition().x;

			float diff = newPosition - cursorOldPositionX;
			
			float dX = std::min(diff / (worldTransform->GetScaling().x * window->GetWidth() * 2), 1.0f);
			progress += dX;
			progress = std::max(std::min(progress, 1.0f), 0.0f);
			cursorOldPositionX = newPosition;
		}
	}
	else
	{
		cursorOldPositionX = -1;
	}

	*_data = minValue + (maxValue - minValue) * progress;
}
