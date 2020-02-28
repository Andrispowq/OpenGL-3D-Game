#include "Camera.h"

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
	delete[] frustumPlanes;
	delete[] frustumCorners;
}

void Camera::LogStage() const
{
	std::cout << "Camera position: ";
	position.print();
	std::cout << "Camera forward: ";
	forward.print();
	std::cout << "Camera up-vector: ";
	up.print();
	std::cout << "Camera right-vector: ";
	GetRight().print();
	std::cout << "Camera left-vector: ";
	GetLeft().print();
}

void Camera::Input(const Window& window)
{
	this->previousPosition = position;
	this->previousForward = forward;
	cameraMoved = false;
	cameraRotated = false;

	movAmt += (0.04f * Input::GetInstance().GetScrollOffset());
	movAmt = static_cast<float>(std::fmax(0.02, movAmt));
	
	//Move by keyboard
	if (InputInstance.IsKeyHeld(GLFW_KEY_W))
		Move(forward, movAmt);
	if (InputInstance.IsKeyHeld(GLFW_KEY_S))
		Move(forward, -movAmt);
	if (InputInstance.IsKeyHeld(GLFW_KEY_A))
		Move(GetLeft(), movAmt);
	if (InputInstance.IsKeyHeld(GLFW_KEY_D))
		Move(GetLeft(), -movAmt);

	if (InputInstance.IsKeyHeld(GLFW_KEY_UP))
		RotateX(-rotAmt / 8);
	if (InputInstance.IsKeyHeld(GLFW_KEY_DOWN))
		RotateX(rotAmt / 8);
	if (InputInstance.IsKeyHeld(GLFW_KEY_LEFT))
		RotateY(-rotAmt / 8);
	if (InputInstance.IsKeyHeld(GLFW_KEY_RIGHT))
		RotateY(rotAmt / 8);

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

		glfwSetCursorPos(window.getWindow(), InputInstance.GetLockedCursorPosition().x, InputInstance.GetLockedCursorPosition().y);
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
	this->previousViewProjectionMatrix = viewProjectionMatrix;

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

	this->projectionMatrix = Matrix4f::PerspectiveProjection(fovY, width / height, ZNEAR, ZFAR);
}