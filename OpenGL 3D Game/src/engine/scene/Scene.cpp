#include "Scene.h"
#include "engine/component/renderer/Renderer.h"
#include "engine/core/gameObject/GameObject.h"
#include "engine/engines/rendering/shaders/Shader.h"

VBO* Scene::vbo;
Shader* Scene::shader;

void Scene::CreateScene(GameObject* root)
{
	vbo = new VBO();
	Mesh mesh;

	Vertex v0(Vector3f(-1, -1, -1));
	Vertex v1(Vector3f(-1, 1, -1));
	Vertex v2(Vector3f(1, 1, -1));
	Vertex v3(Vector3f(1, -1, -1));

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

	vbo->store(mesh);

	shader = new Shader();
	shader->AddShader(ResourceLoader::LoadShader("basic_VS.glsl"), VERTEX_SHADER);
	shader->AddShader(ResourceLoader::LoadShader("basic_FS.glsl"), FRAGMENT_SHADER);
	shader->CompileShader();

	shader->AddUniform("m_transform");
	shader->AddUniform("m_view");
	shader->AddUniform("m_projection");

	GameObject* child = new GameObject();
	child->AddComponent("Renderer component", new Renderer(vbo, shader));

	root->AddChild(child);
}

void Scene::DeleteData()
{
	delete vbo;
	delete shader;
}