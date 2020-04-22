#pragma once

#include <iostream>
#include <fstream>

#include <algorithm>

#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include <string>

#include "engine/prehistoric/core/log/Log.h"
#include "engine/prehistoric/core/util/time/Time.h"

#include "codes/InputCode.h"

template<typename T>
size_t FindElement(T* element, std::vector<T*> list)
{
	for (T* _elem : list)
	{
		if ((*_elem) == (*element))
		{
			return std::distance(list.begin(), std::find(_elem, list.begin(), list.end()));
		}
	}

	return 0xFFFFFFFF;
}