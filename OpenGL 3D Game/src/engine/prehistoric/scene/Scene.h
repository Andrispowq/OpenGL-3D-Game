#ifndef SCENE_H
#define SCENE_H

class GameObject;
class MeshVBO;
class Shader;

class Scene
{
public:
	static void CreateScene(GameObject* root);
	static void DeleteData();
private:
	Scene() {}
	virtual ~Scene() {}
private:
	static MeshVBO* vbo;
	static Shader* shader;
};

#endif