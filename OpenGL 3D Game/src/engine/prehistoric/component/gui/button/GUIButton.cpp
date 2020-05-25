#include "engine/prehistoric/core/util/Includes.hpp"
#include "GUIButton.h"

#include "engine/prehistoric/core/Engine.h"

void GUIButton::PreUpdate(Engine* engine)
{
	bool* val = (bool*)data;

	if (InputInstance.IsButtonHeld(PR_MOUSE_LEFT_BUTTON))
	{
		//Converting mouse position from [0 -> window width; 0 -> window height (from top corner)] 
		//	to [-1 -> 1; -1 -> 1 (from center)], also known as NDC or Normalized Device Coordinates, so that the comparison work on every resolution
		Vector2f mousePos = InputInstance.GetCursorPosition();
		mousePos.y = window->GetHeight() - mousePos.y;
		mousePos /= { (float)window->GetWidth(), (float)window->GetHeight() };
		mousePos *= 2;
		mousePos -= 1;

		Vector2f start = parent->getWorldTransform()->GetPosition().xy() - parent->getWorldTransform()->GetScaling().xy();
		Vector2f size = parent->getWorldTransform()->GetScaling().xy() * 2;

		if (start <= mousePos && (start + size) >= mousePos)
		{
			(*val) = true;
		}
		else
		{
			(*val) = false;
		}
	}
	else
	{
		(*val) = false;
	}
}