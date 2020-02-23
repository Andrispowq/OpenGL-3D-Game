#pragma once

#include "engine/core/movement/Camera.h"
#include "engine/core/buffer/VBO.h"
#include "engine/rendering/shaders/Shader.h"

#define println(x) std::cout << x << std::endl;

int main()
{ 
	Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, (const char*) WINDOW_TITLE, WINDOW_FULLSCREEN);

	if (!window.create())
	{
		return -1;
	}

	window.clearColor(0, 0, 0, 1);

	Input* in = &(Input::GetInstance());
	in->Init(window);

	/*Vector3f a;
	Vector3f b;
	a.lerp(b, 0.5f).print();
	a.cross(b).print();
	a.reflect(b).print();
	a.refract(b, 0.2f).print();*/

	Camera camera(Vector3f(0, 0, 0), 90);

	VBO vbo;
	Mesh mesh;

	Vertex v0(Vector3f(-1, -1, 0));
	Vertex v1(Vector3f(-1,  1, 0));
	Vertex v2(Vector3f( 1,  1, 0));
	Vertex v3(Vector3f( 1, -1, 0));

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);	vertices.push_back(v3);

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

	if (!shader.AddUniform("scale"))
	{
		exit(-1);
	}
	
	shader.Bind();
	shader.SetUniformf("scale", 0.5f);
	shader.Unbind();
	
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
		if (in->IsButtonPushed(0))
		{
			println("hello");
		}

		in->GetCursorPosition().print();

		shader.Bind();
		vbo.draw();
		shader.Unbind();

		window.render();
	}

	return 0;
}