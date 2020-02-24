#pragma once

#include "engine/core/model/Transform.h"
#include "engine/core/buffer/VBO.h"
#include "engine/rendering/shaders/Shader.h"

#define println(x) std::cout << x << std::endl;

int main()
{
	FrameworkConfig::LoadConfig("res/config/framework.cfg");

	Window window = Window(FrameworkConfig::windowWidth, FrameworkConfig::windowHeight, (const char*) FrameworkConfig::windowName, FrameworkConfig::windowFullScreen);

	if (!window.create())
	{
		return -1;
	}

	window.clearColor(0, 0, 0, 1);

	Input* in = &(Input::GetInstance());
	in->Init(window);

	Camera camera(Vector3f(0, 0, 0), 90);

	VBO vbo;
	Mesh mesh;

	Vertex v0(Vector3f(-1, -1, -1));
	Vertex v1(Vector3f(-1,  1, -1));
	Vertex v2(Vector3f( 1,  1, -1));
	Vertex v3(Vector3f( 1, -1, -1));

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);	
	vertices.push_back(v3);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	mesh.setVertices(vertices);
	mesh.setIndices(indices);

	vbo.store(mesh);

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
		window.input();

		if (in->IsKeyPushed(GLFW_KEY_ESCAPE))
		{
			window.SetClosed(true);
		}

		//Here are some test for the input system
		/*if (in->IsButtonPushed(0))
		{
			println("hello");
		}

		in->GetCursorPosition().print();*/

		transform.SetRotation(transform.GetRotation() + Vector3f(0, 0, 0.5f));

		shader.Bind();
		shader.SetUniform("m_transform", transform.getTransformationMatrix());
		vbo.draw();
		shader.Unbind();

		window.render();
	}

	FrameworkConfig::CleanUp();

	return 0;
}