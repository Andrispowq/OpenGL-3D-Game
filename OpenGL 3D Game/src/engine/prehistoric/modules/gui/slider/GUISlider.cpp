#include "engine/prehistoric/core/util/Includes.hpp"
#include "GUISlider.h"

GUISlider::GUISlider(Window* window, Texture* texture, void* data, size_t dataSize, bool visible)
	: GUIElement(window, texture, data, dataSize, visible)
{
	type = GUIType::Slider;
}

void GUISlider::PreUpdate(Engine* engine)
{

}
