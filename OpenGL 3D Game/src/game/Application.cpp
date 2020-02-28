#pragma once

#include "engine/CoreEngine.h"

#define println(x) std::cout << x << std::endl;

int main()
{
	CoreEngine engine;
	engine.Start();

	/*
	Shader shader;

	if (!shader.AddShader(ResourceLoader::LoadShader("basic_VS.glsl"), VERTEX_SHADER))
		std::cerr << "Vertex shader cannot be added" << std::endl;
	if (!shader.AddShader(ResourceLoader::LoadShader("basic_FS.glsl"), FRAGMENT_SHADER))
		std::cerr << "Fragment shader cannot be added" << std::endl;

	if (!shader.CompileShader())
		std::cerr << "Shader couldn't be compiled" << std::endl;

	Transform transform(Vector3f(0), Vector3f(0), Vector3f(0.5f));

	if (!shader.AddUniform("m_transform"))
		exit(-1);

	while (!window.shouldClose())
	{
		in->Update();

		window.clearScreen();
		window.Input();

		if (in->IsKeyPushed(GLFW_KEY_ESCAPE))
		{
			window.SetClosed(true);
		}

		//Here are some test for the input system
		if (in->IsButtonPushed(0))
		{
			println("hello");
		}

		in->GetCursorPosition().print();

		transform.SetRotation(transform.GetRotation() + Vector3f(0, 0, 0.5f));

		shader.Bind();
		shader.SetUniform("m_transform", transform.getTransformationMatrix());
		vbo.draw();
		shader.Unbind();

		window.Render();
	}*/

	return 0;
}