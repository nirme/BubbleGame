#include "StringUtils.h"


namespace core
{

	std::vector<std::string> tokenize(const std::string &_text, const std::string &_tokens)
	{
		std::vector<std::string> output;

		const char *text = _text.c_str();
		const char *textEnd = text + output.size();

		const char *tokens = _tokens.c_str();
		const char *tokensEnd = tokens + _tokens.size();

		const char *textPtr = text;
		while (text < textEnd)
		{
			textPtr = std::find_first_of(text, textEnd, tokens, tokensEnd);
			output.push_back({text, textPtr});

			text = textPtr + 1;
		}

		return output;
	};


	Vector2 stov2(const std::string& str, size_t* idx)
	{
		const char *begin = str.c_str();
		char *end;

		float x = strtof(begin, &end);
		if (begin == end || (begin = strchr(end, ',')) == nullptr)
		{
			if (idx)
				*idx = end - str.c_str(); // length parsed
			return Vector2(x);
		}

		float y = strtof(++begin, &end);

		if (idx)
			*idx = end - str.c_str(); // length parsed

		if (begin == end)
			return Vector2(x);
		return Vector2(x, y);
	};


}
