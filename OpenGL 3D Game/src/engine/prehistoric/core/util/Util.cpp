#include "engine/prehistoric/core/util/Includes.hpp"
#include "Util.h"

namespace Util
{
	std::vector<std::string> Util::Split(const std::string& s, char delim)
	{
		std::vector<std::string> elems;

		const char* cstr = s.c_str();
		unsigned int strLength = static_cast<unsigned int>(s.size());
		unsigned int start = 0;
		unsigned int end = 0;

		while (end <= strLength)
		{
			while (end <= strLength)
			{
				if (cstr[end] == delim)
					break;

				end++;
			}

			elems.push_back(s.substr(start, end - start));
			start = end + 1;
			end = start;
		}

		return elems;
	}

	std::vector<std::string> Util::RemoveEmptyStrings(const std::vector<std::string>& vec)
	{
		std::vector<std::string> res;

		for (const auto& str : vec)
		{
			if (str == "")
			{
				continue;
			}
			else
			{
				res.push_back(str);
			}
		}

		return res;
	}
}