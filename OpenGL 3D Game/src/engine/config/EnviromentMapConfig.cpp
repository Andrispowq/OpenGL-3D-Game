#include "engine/prehistoric/core/util/Includes.hpp"
#include "EnviromentMapConfig.h"

namespace EnviromentMapConfig
{
	std::string enviromentMapLocation;
	uint32_t enviromentMapResolution;
	uint32_t irradianceMapResolution;
	uint32_t prefilterMapResolution;
	uint32_t prefilterLevels;

	void EnviromentMapConfig::LoadConfig(const std::string & path)
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

				if (line.substr(0, 1).c_str()[0] == '#')
				{
					continue; //This line is a comment then
				}

				if (nameTokens[0] == "enviroment")
				{
					if (nameTokens[1] == "map")
					{
						if (nameTokens[2] == "location")
						{
							enviromentMapLocation = tokens[1];
						}
						if (nameTokens[2] == "resolution")
						{
							enviromentMapResolution = std::atoi(tokens[1].c_str());
						}
					}
				}
				if (nameTokens[0] == "irradiance")
				{
					if (nameTokens[1] == "map")
					{
						if (nameTokens[2] == "resolution")
						{
							irradianceMapResolution = std::atoi(tokens[1].c_str());
						}
					}
				}
				if (nameTokens[0] == "prefilter")
				{
					if (nameTokens[1] == "map")
					{
						if (nameTokens[2] == "resolution")
						{
							irradianceMapResolution = std::atoi(tokens[1].c_str());
						}
						if (nameTokens[2] == "levels")
						{
							irradianceMapResolution = std::atoi(tokens[1].c_str());
						}
					}
				}
			}

			file.close();
		}
		else
		{
			PR_LOG_ERROR("File %s cannot be opened!", path.c_str());
		}
	}
};
