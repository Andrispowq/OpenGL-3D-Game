#ifndef SCENE_H
#define SCENE_H

class GameObject;
class VBO;
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
	static VBO* vbo;
	static Shader* shader;
};

#endif