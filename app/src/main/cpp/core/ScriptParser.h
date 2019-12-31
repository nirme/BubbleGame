#pragma once

#include <memory>

#include "DataStream.h"
#include "ScriptNode.h"



namespace core
{

	class ScriptParser
	{
	public:
		virtual ScriptNodeListPtr parseScript(DataStreamPtr _stream) const = 0;
		virtual ~ScriptParser(){};
	};

	typedef std::shared_ptr<ScriptParser> ScriptParserPtr;
}
