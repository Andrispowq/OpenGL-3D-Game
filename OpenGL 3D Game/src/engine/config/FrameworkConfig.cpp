#include "FrameworkConfig.h"

namespace FrameworkConfig
{
	std::string FrameworkConfig::windowName;
	int FrameworkConfig::windowWidth;
	int FrameworkConfig::windowHeight;
	bool FrameworkConfig::windowFullScreen;
	bool FrameworkConfig::windowResizable;
	int FrameworkConfig::windowNumSamples;
	int FrameworkConfig::windowMaxFPS;

	API FrameworkConfig::api;
	Vector2i FrameworkConfig::apiVersion;

	void FrameworkConfig::LoadConfig(const std::string& path)
	{
		std::ifstream file;
		file.open(path.c_str());

		std::string line;

		if (file.is_open())
		{
			while (file.good())
			{
				std::getline(file, line);
				std::vector<std::string> tokens = Util::Split(line, ' ');
				std::vector<std::string> nameTokens = Util::Split(tokens[0], '.');

				if (nameTokens[0] == "window")
				{
					if (nameTokens[1] == "name")
					{
						std::string name;

						for (unsigned int i = 1; i < tokens.size(); i++)
						{
							name += tokens[i];
						}

						windowName = name;
					}
					if (nameTokens[1] == "width")
					{
						windowWidth = std::atoi(tokens[1].c_str());
					}
					if (nameTokens[1] == "height")
					{
						windowHeight = std::atoi(tokens[1].c_str());
					}
					if (nameTokens[1] == "fullScreen")
					{
						windowFullScreen = std::atoi(tokens[1].c_str()) != 0;
					}
					if (nameTokens[1] == "resizable")
					{
						windowResizable = std::atoi(tokens[1].c_str()) != 0;
					}
					if (nameTokens[1] == "numSamples")
					{
						windowNumSamples = std::atoi(tokens[1].c_str());
					}
					if (nameTokens[1] == "maxFPS")
					{
						windowMaxFPS = std::atoi(tokens[1].c_str());
					}
				}
				if (nameTokens[0] == "api")
				{
					if (nameTokens[1] == "name")
					{
						if (tokens[1] == "OpenGL")
						{
							api = OpenGL;
						}
						else if (tokens[1] == "Vulkan")
						{
							api = Vulkan;
						}
						else
						{
							printf("Specify a valid API (OpenGL or Vulkan) under res/config/framework.cfg!");
							api = NO_API;
						}
					}
					if (nameTokens[1] == "version")
					{
						apiVersion = { std::atoi(tokens[1].c_str()), std::atoi(tokens[2].c_str()) };
					}
				}
			}

			file.close();
		}
		else
		{
			fprintf(stderr, "File %s cannot be opened!", path.c_str());
		}
	}
}