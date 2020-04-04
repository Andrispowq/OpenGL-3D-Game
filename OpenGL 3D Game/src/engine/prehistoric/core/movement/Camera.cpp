#include "engine/prehistoric/core/util/Includes.hpp"
#include "Camera.h"
#include "engine/config/EngineConfig.h"

Camera::Camera(float moveAmt, float rotAmt, float mouseSensitivity, float fov, const Vector3f& position)
{
	this->movAmt = moveAmt;
	this->rotAmt = rotAmt;
	this->mouseSensitivity = mouseSensitivity;

	this->position = position;
	this->forward = Vector3f(0, 0, 1);
	this->up = yAxis;

	SetProjection(fov, WINDOW_WIDTH, WINDOW_HEIGHT);

	this->viewMatrix = Matrix4f::View(forward, up) * Matrix4f::Translation(position * -1);
	this->viewProjectionMatrix = Matrix4f::Zero();

	previousViewMatrix = Matrix4f::Zero();
	previousViewProjectionMatrix = Matrix4f::Zero();

	frustumPlanes = new Vector4f[6];
	frustumCorners = new Vector3f[8];
}

Camera::Camera(const Vector3f& position, float fov)
{
	this->movAmt = 0.1f;
	this->rotAmt = 0.8f;
	this->mouseSensitivity = 0.8f;

	this->position = position;
	this->forward = Vector3f(0, 0, 1);
	this->up = yAxis;

	SetProjection(fov, WINDOW_WIDTH, WINDOW_HEIGHT);

	this->viewMatrix = Matrix4f::View(forward, up) * Matrix4f::Translation(position * -1);
	this->viewProjectionMatrix = Matrix4f::Zero();

	previousViewMatrix = Matrix4f::Zero();
	previousViewProjectionMatrix = Matrix4f::Zero();

	frustumPlanes = new Vector4f[6];
	frustumCorners = new Vector3f[8];
}

Camera::~Camera()
{
	for (uint32_t i = 0; i < inputs.size(); i++)
	{
		delete inputs[i];
	}

	delete[] frustumPlanes;
	delete[] frustumCorners;
}

void Camera::LogStage() const
{
	PR_LOG_MESSAGE("Camera information:\n");

	PR_LOG_MESSAGE("\tPosition: ");
	position.print();
	PR_LOG_MESSAGE("\tForward: ");
	forward.print();
	PR_LOG_MESSAGE("\tUp direction: ");
	up.print();
	PR_LOG_MESSAGE("\tRight direction: ");
	GetRight().print();
	PR_LOG_MESSAGE("\tLeft direction: ");
	GetLeft().print();
}

void Camera::Input(Window* window, float delta)
{
	PR_LOG_MESSAGE("Speed: %f\n", movAmt);

	this->previousPosition = position;
	this->previousForward = forward;
	cameraMoved = false;
	cameraRotated = false;

	movAmt += CameraInput::GetKey(speedControl) * delta * 35.0f;
	movAmt = static_cast<float>(std::fmax(0.02, movAmt));

	//Move by input systems
	bool movedForward = false, movedBackward = false, movedRight = false, movedLeft = false,
		rotUp = false, rotDown = false, rotRight = false, rotLeft = false;

	for (CameraInput* in : inputs)
	{
		if (in->IsForward() != 0 && !movedForward)
		{
			Move(forward, movAmt * in->IsForward() * delta);
			movedForward = true;
		}
		if (in->IsBackward() != 0 && !movedBackward)
		{
			Move(forward, -movAmt * in->IsBackward() * delta);
			movedBackward = true;
		}
		if (in->IsLeft() != 0 && !movedLeft)
		{
			Move(GetLeft(), movAmt * in->IsLeft() * delta);
			movedLeft = true;
		}
		if (in->IsRight() != 0 && !movedRight)
		{
			Move(GetLeft(), -movAmt * in->IsRight() * delta);
			movedRight = true;
		}

		if (in->IsUp() != 0 && !rotUp)
		{
			RotateX(static_cast<float>(rotAmt * 2.0 * in->IsUp() * delta));
			rotUp = true;
		}
		if (in->IsDown() != 0 && !rotDown)
		{
			RotateX(static_cast<float>(-rotAmt * 2.0 * in->IsDown() * delta));
			rotDown = true;
		}
		if (in->IsRightRot() != 0 && !rotRight)
		{
			RotateY(static_cast<float>(-rotAmt * 2.0 * in->IsRightRot() * delta));
			rotRight = true;
		}
		if (in->IsLeftRot() != 0 && !rotLeft)
		{
			RotateY(static_cast<float>(rotAmt * 2.0 * in->IsLeftRot() * delta));
			rotLeft = true;
		}
	}

	/*if (InputInstance.IsKeyHeld(GLFW_KEY_W))
		Move(forward, movAmt);
	if (InputInstance.IsKeyHeld(GLFW_KEY_S))
		Move(forward, -movAmt);
	if (InputInstance.IsKeyHeld(GLFW_KEY_A))
		Move(GetLeft(), movAmt);
	if (InputInstance.IsKeyHeld(GLFW_KEY_D))
		Move(GetLeft(), -movAmt);

	if (InputInstance.IsKeyHeld(GLFW_KEY_UP))
		RotateX(-rotAmt / 8.0);
	if (InputInstance.IsKeyHeld(GLFW_KEY_DOWN))
		RotateX(rotAmt / 8.0);
	if (InputInstance.IsKeyHeld(GLFW_KEY_LEFT))
		RotateY(-rotAmt / 8.0);
	if (InputInstance.IsKeyHeld(GLFW_KEY_RIGHT))
		RotateY(rotAmt / 8.0);*/

		//Free mouse rotation
	if (InputInstance.IsButtonHeld(2))
	{
		float dy = InputInstance.GetLockedCursorPosition().y - InputInstance.GetCursorPosition().y;
		float dx = InputInstance.GetLockedCursorPosition().x - InputInstance.GetCursorPosition().x;

		//y-axis rotation
		if (dy != 0)
		{
			rotYstride = std::fabs(dy * 0.01f);
			rotYamt = -dy;
			rotYcounter = 0;
			rotYInitiated = true;
		}

		if (rotYInitiated)
		{
			//up-rotation
			if (rotYamt < 0)
			{
				if (rotYcounter > rotYamt)
				{
					if(FrameworkConfig::api == Vulkan)
						RotateX(rotYstride * mouseSensitivity);
					else if (FrameworkConfig::api == OpenGL)
						RotateX(-rotYstride * mouseSensitivity);
					rotYcounter -= rotYstride;
					rotYstride *= 0.98f;
				}
				else
				{
					rotYInitiated = false;
				}
			}
			//down-rotation
			else if (rotYamt > 0)
			{
				if (rotYcounter < rotYamt)
				{
					if (FrameworkConfig::api == Vulkan)
						RotateX(-rotYstride * mouseSensitivity);
					else if (FrameworkConfig::api == OpenGL)
						RotateX(rotYstride * mouseSensitivity);
					rotYcounter += rotYstride;
					rotYstride *= 0.98f;
				}
				else
				{
					rotYInitiated = false;
				}
			}
		}

		//x-axis rotation
		if (dx != 0)
		{
			rotXstride = std::fabs(dx * 0.01f);
			rotXamt = -dx;
			rotXcounter = 0;
			rotXInitiated = true;
		}

		if (rotXInitiated)
		{
			//up-rotation
			if (rotXamt < 0)
			{
				if (rotXcounter > rotXamt)
				{
					RotateY(-rotXstride * mouseSensitivity);
					rotXcounter -= rotXstride;
					rotXstride *= 0.98f;
				}
				else
				{
					rotXInitiated = false;
				}
			}
			//down-rotation
			else if (rotXamt > 0)
			{
				if (rotXcounter < rotXamt)
				{
					RotateY(rotXstride * mouseSensitivity);
					rotXcounter += rotXstride;
					rotXstride *= 0.98f;
				}
				else
				{
					rotXInitiated = false;
				}
			}
		}

		InputInstance.SetCursorPositionOnScreen(window, InputInstance.GetLockedCursorPosition());
	}

	if (position != previousPosition)
	{
		cameraMoved = true;
	}

	if (forward != previousForward)
	{
		cameraRotated = true;
	}

	this->previousViewMatrix = viewMatrix;

	viewMatrix = Matrix4f::View(forward, up) * Matrix4f::Translation(position * -1);

	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::Move(const Vector3f& dir, const float& amount)
{
	this->position += dir * amount;
}

void Camera::RotateX(const float& angle)
{
	Vector3f hAxis = yAxis.cross(forward).normalize();
	this->forward = forward.rotate(hAxis, angle).normalize();
	this->up = forward.cross(hAxis).normalize();
}

void Camera::RotateY(const float& angle)
{
	Vector3f hAxis = yAxis.cross(forward).normalize();
	this->forward = forward.rotate(yAxis, angle).normalize();
	this->up = forward.cross(hAxis).normalize();
}

void Camera::SetProjection(const float& fov, const float& width, const float& height)
{
	this->fovY = fov;
	this->width = width;
	this->height = height;

	this->projectionMatrix = Matrix4f::PerspectiveProjection(fovY, width / height, EngineConfig::rendererNearPlane, EngineConfig::rendererFarPlane);
}