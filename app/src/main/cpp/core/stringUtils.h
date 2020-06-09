#pragma once

#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "Vector2.h"


namespace core
{
	std::vector<std::string> tokenize(const std::string &_text, const std::string &_tokens);

	Vector2 stov2(const std::string& str, size_t* idx = 0);

}
