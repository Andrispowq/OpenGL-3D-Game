#pragma once

#include "engine/prehistoric/core/util/Includes.hpp"
#include "engine/CoreEngine.h"

int main(int argc, char** args)
{
	/*_CrtMemState state, state_after, state2;
	_CrtMemCheckpoint(&state);*/

	{
		CoreEngine engine;
		engine.Start();
	}

	/*_CrtMemCheckpoint(&state_after);
	
	if(_CrtMemDifference(&state2, &state, &state_after))
		_CrtMemDumpStatistics(&state2);*/

  	return 0;
}