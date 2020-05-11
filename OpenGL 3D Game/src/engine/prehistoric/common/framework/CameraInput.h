#ifndef CAMERA_INPUT_H
#define CAMERA_INPUT_H

#include "Input.h"

enum InputType
{
	NO_TYPE,
	KEY_PRESSED, KEY_HELD, KEY_RELEASED,
	MOUSE_PRESSED, MOUSE_HELD, MOUSE_RELEASED, MOUSE_SCROLL,
	JOYSTICK_BUTTON_PRESSED, JOYSTICK_AXIS_MOVED_NEGATIVE, JOYSTICK_AXIS_MOVED_POSITIVE
};

struct InputData
{
	InputType type;
	InputCode code;
	JoystickID joystickNumber;
};

struct MoveData
{
	bool moved;
	float amount;
};

static InputData DefInData = { NO_TYPE, PR_KEY_UNKNOWN, PR_JOYSTICK_1 };

template<typename T>
static T clamp(T val, T min, T max)
{
	if (val > max)
		return val = max;
	else if (val < min)
		return val = min;
	else
		return val;
}

class CameraInput
{
public:
	CameraInput(InputData forward, InputData backward, InputData right, InputData left,
		InputData up, InputData down, InputData rightRot, InputData leftRot)
		: forward(forward), backward(backward), right(right), left(left),
		up(up), down(down), rightRot(rightRot), leftRot(leftRot) {}

	CameraInput() : forward(DefInData), backward(DefInData), right(DefInData), left(DefInData),
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
	void SetForward(const InputData& forward) { this->forward = forward; }
	void SetBackward(const InputData& backward) { this->backward = backward; }
	void SetRight(const InputData& right) { this->right = right; }
	void SetLeft(const InputData& left) { this->left = left; }

	void SetUp(const InputData& up) { this->up = up; }
	void SetDowny(const InputData& down) { this->down = down; }
	void SetRightRot(const InputData& rightRot) { this->rightRot = rightRot; }
	void SetLeftRot(const InputData& leftRot) { this->leftRot = leftRot; }

	static float GetKey(const InputData& data)
	{
		switch (data.type)
		{
		case KEY_PRESSED:
			return (float)InputInstance.IsKeyPushed(data.code);
		case KEY_HELD:
			return (float)InputInstance.IsKeyHeld(data.code);
		case KEY_RELEASED:
			return (float)InputInstance.IsKeyReleased(data.code);

		case MOUSE_PRESSED:
			return (float)InputInstance.IsButtonPushed(data.code);
		case MOUSE_HELD:
			return (float)InputInstance.IsButtonHeld(data.code);
		case MOUSE_RELEASED:
			return (float)InputInstance.IsButtonReleased(data.code);
		case MOUSE_SCROLL:
			return InputInstance.GetScrollOffset();

		case JOYSTICK_BUTTON_PRESSED:
			return (float)InputInstance.IsJoystickButtonPushed(data.code, data.joystickNumber);
		case JOYSTICK_AXIS_MOVED_NEGATIVE:
			return clamp(-InputInstance.GetJoystickAxisOffset(data.code, data.joystickNumber), -1.0f, 0.0f);
		case JOYSTICK_AXIS_MOVED_POSITIVE:
			return clamp(-InputInstance.GetJoystickAxisOffset(data.code, data.joystickNumber), 0.0f, 1.0f);

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