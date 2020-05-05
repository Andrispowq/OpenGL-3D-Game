#pragma once

#include <iostream>
#include <fstream>

#include <algorithm>

#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>

#include "engine/prehistoric/core/log/Log.h"
#include "engine/prehistoric/core/util/time/Time.h"

#include "codes/InputCode.h"

template<typename T>
size_t FindElement(T* element, std::vector<T*> list)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		if ((*list[i]) == (*element))
		{
			return i;
		}
	}

	return 0xFFFFFFFF;
}