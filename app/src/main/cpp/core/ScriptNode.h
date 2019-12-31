#pragma once

#include <string>
#include <list>
#include <memory>

namespace core
{

	class ScriptNode;

	typedef std::shared_ptr<ScriptNode>		ScriptNodePtr;
	typedef std::list< ScriptNodePtr>		ScriptNodeList;
	typedef std::shared_ptr<ScriptNodeList>	ScriptNodeListPtr;

	class ScriptNode
	{
	public:
		virtual std::string getName() = 0;
		virtual std::string getValue(const std::string& _attribute = "") = 0;
		virtual ScriptNodeListPtr getChildList() = 0;
	};


}
