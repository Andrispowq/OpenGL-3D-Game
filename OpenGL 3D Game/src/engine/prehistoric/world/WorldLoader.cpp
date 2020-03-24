#include "engine/prehistoric/core/util/Includes.hpp"
#include "WorldLoader.h"

void WorldLoader::LoadWorld(const std::string& worldFile, GameObject* root)
{
	std::ifstream file;
	file.open(worldFile.c_str());

	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, line);
			std::vector<std::string> tokens = Util::Split(line, ' ');
			std::vector<std::string> nameTokens = Util::Split(tokens[0], '.');

			if (tokens.size() == 0 || nameTokens[0] == "#")
				continue;

			//Setting the directories which we'll be using to load models and textures from
			if (nameTokens[0] == "directories")
			{
				if (nameTokens[1] == "models")
				{
					directoryModels = tokens[1];
				}
				if (nameTokens[1] == "textures")
				{
					directoryTextures = tokens[1];
				}
			}

			//Loading models and texture
			if (nameTokens[0] == "models")
			{
				if (nameTokens[1] == "load")
				{
					MeshVBO* vbo = OBJLoader::LoadModel(directoryModels, tokens[2], "");
					models.emplace(tokens[1], vbo);
				}
			}
			if (nameTokens[0] == "textures")
			{
				if (nameTokens[1] == "load")
				{
					Texture* texture = TextureLoader::LoadTexture(directoryTextures + tokens[2]);
					textures.emplace(tokens[1], texture);
				}
			}

			//Creating materials from textures and other data
			if (nameTokens[0] == "materials")
			{
				if (nameTokens[1] == "add")
				{
					Material* material = new Material;
					materials.emplace(tokens[1], material);
				}
				else
				{
					auto index = materials.find(nameTokens[1]);

					if (index == materials.end())
					{
						PR_LOG_ERROR("Material (name: %s) does not exist, or haven't been created in the world file!\n", nameTokens[1]);
						continue;
					}

					Material* material = materials.at(nameTokens[1]);
					
					if (nameTokens[2] == "texture")
					{
						auto index = textures.find(tokens[2]);

						if (index == textures.end())
						{
							PR_LOG_ERROR("Texture (name: %s) does not exist, or haven't been created in the world file!\n", tokens[1]);
							continue;
						}

						material->AddTexture(tokens[1] + "Map", textures.at(tokens[2]));
					}

					if (nameTokens[2] == "vec4")
					{
						std::vector<std::string> compTokens = Util::Split(tokens[2], ',');

						Vector4f* value = new Vector4f((float) std::atof(compTokens[0].c_str()), (float) std::atof(compTokens[1].c_str()), (float) std::atof(compTokens[2].c_str()), (float) std::atof(compTokens[3].c_str()));
						material->AddVector4f(tokens[1], value);
					}
					if (nameTokens[2] == "vec3")
					{
						std::vector<std::string> compTokens = Util::Split(tokens[2], ',');

						Vector3f* value = new Vector3f((float) std::atof(compTokens[0].c_str()), (float) std::atof(compTokens[1].c_str()), (float) std::atof(compTokens[2].c_str()));
						material->AddVector3f(tokens[1], value);
					}
					if (nameTokens[2] == "vec2")
					{
						std::vector<std::string> compTokens = Util::Split(tokens[2], ',');

						Vector2f* value = new Vector2f((float) std::atof(compTokens[0].c_str()), (float) std::atof(compTokens[1].c_str()));
						material->AddVector2f(tokens[1], value);
					}

					if (nameTokens[2] == "float")
					{
						float* value = new float{ static_cast<float>(std::atof(tokens[2].c_str())) };
						material->AddFloat(tokens[1], value);
					}
					if (nameTokens[2] == "int")
					{
						int* value = new int{ std::atoi(tokens[2].c_str()) };
						material->AddInt(tokens[1], value);
					}
				}
			}

			//Creating GameObjects
			if (nameTokens[0] == "world")
			{
				if (nameTokens[1] == "object")
				{
					if (nameTokens.size() < 3)
					{
						GameObject* obj = new GameObject();

						if (tokens.size() > 2)
						{
							std::vector<std::string> compTokens = Util::Split(tokens[2], ',');
							obj->SetPosition({ (float) std::atof(compTokens[0].c_str()), (float) std::atof(compTokens[1].c_str()), (float) std::atof(compTokens[2].c_str()) });
						}
						if (tokens.size() > 3)
						{
							std::vector<std::string> compTokens = Util::Split(tokens[3], ',');
							obj->SetRotation({ (float)(float) std::atof(compTokens[0].c_str()), (float) std::atof(compTokens[1].c_str()), (float) std::atof(compTokens[2].c_str()) });
						}
						if (tokens.size() > 4)
						{
							std::vector<std::string> compTokens = Util::Split(tokens[4], ',');
							obj->SetScale({ (float) std::atof(compTokens[0].c_str()), (float) std::atof(compTokens[1].c_str()), (float) std::atof(compTokens[2].c_str()) });
						}

						root->AddChild(tokens[1], obj);
					}
					else
					{
						int i = 2;
						GameObject* objToAdd = root;
						while (true)
						{
							objToAdd = reinterpret_cast<GameObject*>(objToAdd->GetChildren()[nameTokens[i]]);
							i++;

							if(nameTokens.size() < (i + 1))
							{
								GameObject* obj = new GameObject();

								if (tokens.size() > 2)
								{
									std::vector<std::string> compTokens = Util::Split(tokens[2], ',');
									obj->SetPosition({ (float) std::atof(compTokens[0].c_str()), (float) std::atof(compTokens[1].c_str()), (float) std::atof(compTokens[2].c_str()) });
								}
								if (tokens.size() > 3)
								{
									std::vector<std::string> compTokens = Util::Split(tokens[3], ',');
									obj->SetRotation({ (float)std::atof(compTokens[0].c_str()), (float)std::atof(compTokens[1].c_str()), (float)std::atof(compTokens[2].c_str()) });
								}
								if (tokens.size() > 4)
								{
									std::vector<std::string> compTokens = Util::Split(tokens[4], ',');
									obj->SetScale({ (float)std::atof(compTokens[0].c_str()), (float)std::atof(compTokens[1].c_str()), (float)std::atof(compTokens[2].c_str()) });
								}

								objToAdd->AddChild(tokens[1], obj);

								break;
							}
						}
					}
				}
			}

			//Adding components to objects
			if (nameTokens[0] == "object")
			{
				auto list = root->GetChildren();

				auto index = list.find(nameTokens[1]);

				if (index == list.end())
				{
					PR_LOG_ERROR("GameObject (name: %s) does not exist, or haven't been created in the world file!\n", nameTokens[1]);
					continue;
				}

				GameObject* obj = reinterpret_cast<GameObject*>(list[nameTokens[1]]);

				if (nameTokens[2] == "component")
				{
					if (tokens[1] == "Renderer")
					{
						std::vector<std::string> compTokens = Util::Split(tokens[2], ',');

						MeshVBO* vbo = models[compTokens[0]];
						Material* material = materials[compTokens[2]];
						Shader* shader = nullptr;

						Pipeline* pipeline = nullptr;

						//Checking for shader types
						if (compTokens[1] == "basic")
						{
							if (FrameworkConfig::api == OpenGL)
							{
								shader = new GLBasicShader();
							}
							else
							{
								PR_LOG_RUNTIME_ERROR("Shader type 'basic' is currently only supported on the OpenGL API!\n");
							}
						}
						if (compTokens[1] == "pbr")
						{
							if (FrameworkConfig::api == OpenGL)
							{
								shader = new GLPBRShader();
							}
							else
							{
								PR_LOG_RUNTIME_ERROR("Shader type 'pbr' is currently only supported on the OpenGL API!\n");
							}
						}

						if (FrameworkConfig::api == OpenGL)
						{
							pipeline = new GLPipeline();
							pipeline->SetShader(shader);
						}

						Renderer* renderer = new Renderer(vbo, pipeline, material);

						obj->AddComponent(tokens[1], renderer);
					}
					if (tokens[1] == "Light")
					{
						std::vector<std::string> compTokens = Util::Split(tokens[2], ';');

						Light* light = new Light();

						std::vector<std::string> posTokens = Util::Split(compTokens[0], ',');
						light->SetColour({ (float) std::atof(posTokens[0].c_str()), (float)std::atof(posTokens[1].c_str()), (float)std::atof(posTokens[2].c_str()) });

						light->SetIntensity((float) std::atof(compTokens[1].c_str()));

						obj->AddComponent("Light", light);

						break;
					}
				}
				else
				{
					int i = 2;
					GameObject* objToAdd = obj;
					while (true)
					{
						auto list = objToAdd->GetChildren();

						auto index = list.find(nameTokens[i]);

						if (index == list.end())
						{
							PR_LOG_ERROR("GameObject (name: %s) does not exist, or haven't been created in the world file!\n", nameTokens[i]);
							continue;
						}

						objToAdd = reinterpret_cast<GameObject*>(list[nameTokens[1]]);
						i++;

						if (nameTokens[i] == "component")
						{
							if (tokens[1] == "Renderer")
							{
								std::vector<std::string> compTokens = Util::Split(tokens[2], ',');

								MeshVBO* vbo = models[compTokens[0]];
								Material* material = materials[compTokens[2]];
								Shader* shader = nullptr;

								Pipeline* pipeline = nullptr;

								//Checking for shader types
								if (compTokens[1] == "basic")
								{
									if (FrameworkConfig::api == OpenGL)
									{
										shader = new GLBasicShader();
									}
									else
									{
										PR_LOG_RUNTIME_ERROR("Shader type 'basic' is currently only supported on the OpenGL API!\n");
									}
								}
								if (compTokens[1] == "pbr")
								{
									if (FrameworkConfig::api == OpenGL)
									{
										shader = new GLPBRShader();
									}
									else
									{
										PR_LOG_RUNTIME_ERROR("Shader type 'pbr' is currently only supported on the OpenGL API!\n");
									}
								}

								if (FrameworkConfig::api == OpenGL)
								{
									pipeline = new GLPipeline();
									pipeline->SetShader(shader);
								}

								Renderer* renderer = new Renderer(vbo, pipeline, material);

								obj->AddComponent(tokens[1], renderer);

								break;
							}
							if (tokens[1] == "Light")
							{
								std::vector<std::string> compTokens = Util::Split(tokens[2], ';');

								Light* light = new Light();

								std::vector<std::string> posTokens = Util::Split(compTokens[0], ',');
								light->SetColour({ (float)std::atof(posTokens[0].c_str()), (float)std::atof(posTokens[1].c_str()), (float)std::atof(posTokens[2].c_str()) });

								light->SetIntensity((float)std::atof(compTokens[1].c_str()));

								objToAdd->AddComponent("Light", light);

								break;
							}
						}
						else
						{
							continue;
						}
					}
				}
			}
		}
	}
	else
	{
		PR_LOG_ERROR("Could not open world file %s!\n", worldFile.c_str());
	}
}