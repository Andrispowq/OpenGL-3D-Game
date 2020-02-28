#ifndef FRAMEWORKCONFIG_H
#define FRAMEWORKCONFIG_H

#include <fstream>

#include "engine/core/util/Util.h"
#include "engine/core/math/Math.h"

namespace FrameworkConfig
{
	void LoadConfig(const std::string& path);

	extern std::string windowName;
	extern int windowWidth;
	extern int windowHeight;
	extern bool windowFullScreen;
	extern bool windowResizable;
	extern int windowNumSamples;
	extern int windowMaxFPS;

	extern Vector2i openglVersion;
};

#endif