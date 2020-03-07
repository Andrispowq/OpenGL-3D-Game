#ifndef CAMERA_INPUT_H
#define CAMERA_INPUT_H

#include "Input.h"

enum InputType
{
	KEY_PRESSED, KEY_HELD, KEY_RELEASED,
	MOUSE_PRESSED, MOUSE_HELD, MOUSE_RELEASED,
	JOYSTICK_BUTTON_PRESSED, JOYSTICK_AXIS_MOVED
};

struct InputData
{
	unsigned int code;
	InputType type;
	int joystickNumber;
};

struct MoveData
{
	bool moved;
	float amount;
};

static InputData DefInData = { 0, KEY_HELD, 0 };

class CameraInput
{
public:
	CameraInput(InputData forward, InputData backward, InputData right, InputData left,
		InputData up, InputData down, InputData rightRot, InputData leftRot)
		: forward(forward), backward(backward), right(right), left(left),
		up(up), down(down), rightRot(rightRot), leftRot(leftRot) {}

	CameraInput()
		: forward(DefInData), backward(DefInData), right(DefInData), left(DefInData),
		up(DefInData), down(DefInData), rightRot(DefInData), leftRot(DefInData) {}

	virtual ~CameraInput() {}

	//Getters
	float IsForward() const { return GetKey(forward); }
	float IsBackward() const { return GetKey(backward); }
	float IsRight() const { return GetKey(right); }
	float IsLeft() const { return GetKey(left); }

	float IsUp() const { return GetKey(up); }
	float IsDown() const { return GetKey(down); }
	float IsRightRot() const { return GetKey(rightRot); }
	float IsLeftRot() const { return GetKey(leftRot); }

	//Setters
	void SetForward(InputData forward) { this->forward = forward; }
	void SetBackward(InputData backward) { this->backward = backward; }
	void SetRight(InputData right) { this->right = right; }
	void SetLeft(InputData left) { this->left = left; }

	void SetUp(InputData up) { this->up = up; }
	void SetDowny(InputData down) { this->down = down; }
	void SetRightRot(InputData rightRot) { this->rightRot = rightRot; }
	void SetLeftRot(InputData leftRot) { this->leftRot = leftRot; }
private:
	float GetKey(InputData data) const
	{
		switch (data.type)
		{
		case KEY_PRESSED:
			return InputInstance.IsKeyPushed(data.code);
		case KEY_HELD:
			return InputInstance.IsKeyHeld(data.code);
		case KEY_RELEASED:
			return InputInstance.IsKeyReleased(data.code);

		case MOUSE_PRESSED:
			return InputInstance.IsButtonPushed(data.code);
		case MOUSE_HELD:
			return InputInstance.IsButtonHeld(data.code);
		case MOUSE_RELEASED:
			return InputInstance.IsButtonReleased(data.code);

		case JOYSTICK_BUTTON_PRESSED:
			return InputInstance.IsJoystickButtonPushed(data.code, data.joystickNumber);
		case JOYSTICK_AXIS_MOVED:
			return InputInstance.GetJoystickAxisOffset(data.code, data.joystickNumber);

		default:
			return 0;
		}
	}
private:
	InputData forward;
	InputData backward;
	InputData right;
	InputData left;
	
	InputData up;
	InputData down;
	InputData rightRot;
	InputData leftRot;
};

#endif