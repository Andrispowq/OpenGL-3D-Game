#include "Scene.h"
#include "engine/prehistoric/component/renderer/Renderer.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/platform/opengl/rendering/shaders/basic/GLBasicShader.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/platform/opengl/buffer/GLMeshVBO.h"

MeshVBO* Scene::vbo;
Shader* Scene::shader;

void Scene::CreateScene(GameObject* root)
{
	if (FrameworkConfig::api == OpenGL)
	{
		vbo = new GLMeshVBO();
	}

	Mesh mesh;

	Vertex v0(Vector3f(-1, -1, 1), Vector2f(0, 0), Vector3f(0, 0, -1));
	Vertex v1(Vector3f(-1, 1, 1), Vector2f(0, 1), Vector3f(0, 0, -1));
	Vertex v2(Vector3f(1, 1, 1), Vector2f(1, 1), Vector3f(0, 0, -1));
	Vertex v3(Vector3f(1, -1, 1), Vector2f(1, 0), Vector3f(0, 0, -1));

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

	vbo->Store(mesh);

	if (FrameworkConfig::api == OpenGL)
	{
		shader = new GLBasicShader();
	}

	GameObject* child = new GameObject();

	Material* mat = new Material;
	mat->AddTexture("diffuse", TextureLoader::LoadTexture("res/textures/brick/brick_DIF.jpg"));

	child->AddComponent("renderer", new Renderer(vbo, shader, mat));
	child->SetScale(0.5f);

	root->AddChild(child);
}

void Scene::DeleteData()
{
	Renderable::CleanUp();
}