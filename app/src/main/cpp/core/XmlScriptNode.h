#pragma once


#include <exception>
#include <memory>
#include <vector>

#undef RAPIDXML_NO_EXCEPTIONS

#include "..\utils\rapidxml\rapidxml.hpp"

#include "ScriptNode.h"
#include "Logger.h"



namespace core
{

	class XmlDocContainer
	{
		typedef rapidxml::xml_document<char>	XmlDoc;

	public:

		XmlDoc xmlDoc;
		std::vector<char> xmlText;
	};
	typedef std::shared_ptr<XmlDocContainer>	XmlDocContainerPtr;



	class XmlScriptNode : public ScriptNode
	{
	private:

		typedef rapidxml::xml_node<char>		XmlNode;
		typedef rapidxml::xml_attribute<char>	XmlAttrib;

		XmlDocContainerPtr owner;
		XmlNode *node;
		ScriptNodeListPtr childNodes;

	public:

		XmlScriptNode(XmlDocContainerPtr _owner, XmlNode *_node, ScriptNodeListPtr _childNodes) :
			ScriptNode(),
			owner(_owner),
			node(_node),
			childNodes(_childNodes)
		{
			assert(_owner && "temporary XML node require pointer to owner object");
			assert(_node && "XML node cannot be null");
		};


		std::string getName()
		{
			return std::string(node->name(), node->name_size());
		};

		std::string getValue(const std::string& _attribute)
		{
			if (_attribute.empty())
				return std::string(node->value(), node->value_size());

			XmlAttrib *attrib = node->first_attribute(_attribute.c_str());
			return attrib ? std::string(attrib->value(), attrib->value_size()) : std::string();
		};

		ScriptNodeListPtr getChildList()
		{
			return childNodes;
		};
	};

}
