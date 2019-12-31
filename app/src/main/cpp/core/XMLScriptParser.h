#pragma once


#include <memory>
#include <vector>

#include "ScriptParser.h"
#include "XmlScriptNode.h"
#include "Logger.h"
#include "../utils/rapidxml/rapidxml.hpp"


namespace core
{

	class XmlScriptParser : public ScriptParser
	{
	private:

		typedef rapidxml::xml_document<char>	XmlDoc;
		typedef rapidxml::xml_node<char>		XmlNode;


#ifdef NDEBUG
		static constexpr int parserSetting = rapidxml::parse_trim_whitespace;
#else // !NDEBUG
		static constexpr int parserSetting = rapidxml::parse_validate_closing_tags | rapidxml::parse_trim_whitespace;
#endif // NDEBUG

		typedef std::shared_ptr<XmlScriptNode> XmlScriptNodePtr;


		ScriptNodePtr populateNode(XmlDocContainerPtr _owner, XmlNode *_root) const
		{
			if (!_root) return nullptr;

			ScriptNodeListPtr childNodes(new ScriptNodeList());
			for (XmlNode *child = _root->first_node(); child != nullptr; child = child->next_sibling())
			{
				childNodes->push_back(populateNode(_owner, child));
			}

			return ScriptNodePtr(new XmlScriptNode(_owner, _root, childNodes));
		};


	public:
		ScriptNodeListPtr parseScript(DataStreamPtr _stream) const
		{
			XmlDocContainerPtr xmlDocContainer(new XmlDocContainer());

			xmlDocContainer->xmlText.resize(_stream->getSize() + 1);
			xmlDocContainer->xmlText.shrink_to_fit();
			_stream->readData(xmlDocContainer->xmlText.data());
            xmlDocContainer->xmlText.back() = 0; //make sure string is capped with '\0' at the end


			xmlDocContainer->xmlDoc.parse<parserSetting>(xmlDocContainer->xmlText.data());

			// xml should have only 1 root node
			XmlNode *root = xmlDocContainer->xmlDoc.first_node();

			ScriptNodePtr rootNode = populateNode(xmlDocContainer, root);

			// return list of root children to ignore the root
			return rootNode->getChildList();
		};
	};

}
