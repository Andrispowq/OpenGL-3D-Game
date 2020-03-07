#ifndef GL_INPUT_H
#define GL_INPUT_H

#include "engine/prehistoric/common/framework/Input.h"
#include "engine/prehistoric/core/math/Math.h"

class GLInput : public Input
{
public:
	bool Init(Window* window) const override;
	bool Update() override;

	void SetCursorPositionOnScreen(Window* window, const Vector2f& cursorPosition) override;
};

#endif