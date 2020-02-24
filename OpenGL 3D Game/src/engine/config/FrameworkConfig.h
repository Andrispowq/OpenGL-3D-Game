#ifndef FRAMEWORKCONFIG_H
#define FRAMEWORKCONFIG_H

#include <fstream>

#include "engine/core/Util.h"
#include "engine/core/math/Math.h"

namespace FrameworkConfig
{
	void LoadConfig(const std::string& path);
	void CleanUp();

	//Config variables
	const char* windowName;
	int windowWidth;
	int windowHeight;
	bool windowFullScreen;
	bool windowResizable;
	int windowNumSamples;

	Vector2i openglVersion;
};

#endif