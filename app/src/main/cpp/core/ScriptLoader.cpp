#include "ScriptLoader.h"


template<>
core::ScriptLoader* Singleton<core::ScriptLoader>::impl = nullptr;

namespace core
{
	ScriptLoader::ScriptLoader() :
		parser(nullptr)
	{};


	const ScriptParser& ScriptLoader::getParser()
	{
		assert(parser && "no parser registered in script loader");
		return *parser.get();
	};

	void ScriptLoader::registerParser(ScriptParserPtr _parser)
	{
		parser = _parser;
	};

	ScriptNodeListPtr ScriptLoader::parse(DataStreamPtr _stream)
	{
		assert(parser && "no parser registered in script loader");
		return parser->parseScript(_stream);
	};


	//  generic data

	ScriptNodePtr ScriptLoader::getChildNodeByName(ScriptNodePtr _parent, std::string _name)
	{
		ScriptNodeListPtr children = _parent->getChildList();
		auto childIt = std::find_if(children->begin(), children->end(), [_name] (ScriptNodePtr node) { return !node->getName().compare(_name); });
		return childIt != children->end() ? *childIt : nullptr;
	};


	ScriptNodePtr ScriptLoader::getChildNodeByNameAndValue(ScriptNodePtr _parent, std::string _name, std::string _attrib, std::string _value)
	{
		assert((!_name.empty() || (!_attrib.empty() && !_value.empty())) && "no value provided");

		ScriptNodeListPtr children = _parent->getChildList();

		//_name, std::string _attrib="", std::string _value
		ScriptNodeListIterator childIt;

		if (_attrib.empty())
			return getChildNodeByNameAndValue(_parent, _name);

		if (_name.empty())
			childIt = std::find_if(children->begin(),
								   children->end(),
								   [_attrib, _value] (ScriptNodePtr node)
								   {
									   return !node->getValue(_attrib).compare(_value);
								   });

		else
			childIt = std::find_if(children->begin(),
								   children->end(),
								   [_name, _attrib, _value] (ScriptNodePtr node)
								   {
									   return !node->getName().compare(_name) &&
											  !node->getValue(_attrib).compare(_name);
								   });

		return childIt != children->end() ? *childIt : nullptr;
	};

	//  generic script value parsing

	std::string ScriptLoader::parseResourceName(ScriptNodePtr _node)
	{
		return _node->getValue("name");
	};


	std::string ScriptLoader::parseResourceGroup(ScriptNodePtr _node)
	{
		return _node->getValue("group");
	};


	//  texture specific script parsing

	TEXTURE_TYPE ScriptLoader::parseTextureType(ScriptNodePtr _node) // stub func, only 1 tex type allowed
	{
		std::string type = _node->getValue("type");

		// default
		//if (type.compare("texture2d") == 0)
		return TT_TEXTURE_2D;
	};


	TEXTURE_FILTER ScriptLoader::parseTextureFilter(ScriptNodePtr _node)
	{
		std::string filter = _node->getValue("filter");

		if (filter.compare("linear") == 0)
			return TF_LINEAR;

		// default:
		//if (filter.compare("nearest") == 0)
		return TF_NEAREST;
	};


	//  shader specific script parsing

	SHADER_TYPE ScriptLoader::parseShaderType(ScriptNodePtr _node)
	{
		std::string filter = _node->getValue("type");

		if (filter.compare("vertex") == 0)
			return ST_VERTEX;

		if (filter.compare("fragment") == 0)
			return ST_FRAGMENT;

		return ST_UNKNOWN;
	};


	std::string ScriptLoader::parseShaderName(ScriptNodePtr _node)
	{
		return _node->getValue("resource_name");
	};


	ScriptNodePtr ScriptLoader::findProgramNode(ScriptNodeListPtr _nodeList, const std::string &_name)
	{
		for (auto it = _nodeList->begin(), end = _nodeList->end(); it != end; ++it)
		{
			if (_name.compare((*it)->getValue("name")) == 0)
				return *it;
		}
		return nullptr;
	};


	// sprite specific parsing

	ScriptNodePtr ScriptLoader::findSpriteNode(ScriptNodeListPtr _nodeList, const std::string &_name)
	{
		for (auto it = _nodeList->begin(), end = _nodeList->end(); it != end; ++it)
		{
			if (_name.compare((*it)->getValue("name")) == 0)
				return *it;
		}
		return nullptr;
	};


	std::string ScriptLoader::parseImgSpriteTexture(ScriptNodePtr _node)
	{
		std::string texture = _node->getValue("name");

		std::size_t pos = texture.find_first_of('#');
		return pos != std::string::npos ? texture.substr(0, pos) : texture;
	};


	TextureSpriteCoords ScriptLoader::parseImgSpriteCoords(ScriptNodePtr _node)
	{
		bool inPixel = _node->getValue("in_pixel").empty() ? false : true;

		std::string tmp = _node->getValue("top");
		float top = tmp.empty() ? 1.0f : std::stof(tmp);

		tmp = _node->getValue("left");
		float left = tmp.empty() ? 0.0f : std::stof(tmp);

		tmp = _node->getValue("bottom");
		float bottom = tmp.empty() ? 0.0f : std::stof(tmp);

		tmp = _node->getValue("right");
		float right = tmp.empty() ? 1.0f : std::stof(tmp);

		return TextureSpriteCoords(left, right, top, bottom, inPixel);
	};


	// sprited font

	std::string ScriptLoader::parseFontName(ScriptNodePtr _node)
	{
		return _node->getValue("name");
	};


	std::string ScriptLoader::parseCodePoint(ScriptNodePtr _node)
	{
		return _node->getValue("cp");
	};


	float ScriptLoader::parseCharWidth(ScriptNodePtr _node)
	{
		std::string text = _node->getValue("width");
		if (text.length())
			return std::stof(text);
		return 1.0f;
	};


	bool ScriptLoader::parseCharVisibility(ScriptNodePtr _node)
	{
		std::string text = _node->getValue("visible");
		return (text.compare("false") != 0) ? true : false;
	};


	KerningPair ScriptLoader::parseKerningPair(ScriptNodePtr _node)
	{
		KerningPair value;

		std::string cp = _node->getValue("cp1").c_str();
		memcpy(value.cp1, cp.c_str(), cp.length() < 4 ? cp.length() : 4);

		cp = _node->getValue("cp2").c_str();
		memcpy(value.cp2, cp.c_str(), cp.length() < 4 ? cp.length() : 4);

		value.kerning = std::stof(_node->getValue("kern"));

		return value;
	};


	SpritedFont::FONT_SPACING ScriptLoader::parseFontSpacing(ScriptNodePtr _node)
	{
		std::string spacing = _node->getValue("spacing");

		if (spacing.compare("proportional") == 0)
			return SpritedFont::FS_PROPORTIONAL;

		//if (spacing.compare("mono") == 0)
		return SpritedFont::FS_MONOSPACE;
	};


	std::string ScriptLoader::parseFontTexture(ScriptNodePtr _node)
	{
		return _node->getValue("texture");
	};


	float ScriptLoader::parseSizeMultiplier(ScriptNodePtr _node)
	{
		return std::stof(_node->getValue("font_size"));
	};


	float ScriptLoader::parseLineHeight(ScriptNodePtr _node)
	{
		return std::stof(_node->getValue("line_height"));
	};


	std::string ScriptLoader::parseDefaultChar(ScriptNodePtr _node)
	{
		return _node->getValue("defaultChar");
	};


	// nodes

	std::string ScriptLoader::parseNodeName(ScriptNodePtr _node)
	{
		return _node->getValue("node_name");
	};


	Vector2 ScriptLoader::parseNodeScale(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("scale");

		if (value.size())
		{
			std::string::size_type pos = value.find_first_of(',');
			if (pos == value.npos)
				return Vector2(std::stof(value));
			return Vector2(std::stof(value), std::stof(value.substr(pos)));
		}
		return Vector2(1.0f);
	};


	_2d::Quaternion ScriptLoader::parseNodeRotation(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("scale");

		if (value.size())
			return _2d::Quaternion(std::stof(value));
		return _2d::Quaternion(0.0f);
	};


	Vector2 ScriptLoader::parseNodePosition(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("scale");

		std::string::size_type pos = value.find_first_of(',');
		if (value.size() && pos != value.npos)
			return Vector2(std::stof(value), std::stof(value.substr(pos)));
		return Vector2(0.0f);
	};


	// object

	OBJECT_TYPE ScriptLoader::parseRenderableType(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("type");

		if (value.compare("single_sprite") == 0)
			return OBJECT_TYPE::OT_SINGLE_SPRITE;

		if (value.compare("animated_sprite") == 0)
			return OBJECT_TYPE::OT_ANIMATED_SPRITE;

		if (value.compare("particle_system") == 0)
			return OBJECT_TYPE::OT_PARTICLE_SYSTEM;

		if (value.compare("sprited_text") == 0)
			return OBJECT_TYPE::OT_SPRITED_FONT;

		return OBJECT_TYPE::OT_UNDEFINED;
	};



	std::string ScriptLoader::parseObjectName(ScriptNodePtr _node)
	{
		return _node->getValue("name");
	};


	std::string ScriptLoader::parseObjectShader(ScriptNodePtr _node)
	{
		return _node->getValue("shader_name");
	};


	std::string ScriptLoader::parseObjectImageSprite(ScriptNodePtr _node)
	{
		std::string firstSprite = _node->getValue("sprite_names");
		std::string::size_type pos = firstSprite.find_first_of(';');
		if (pos != firstSprite.npos)
			return firstSprite.substr(0, pos);
		return firstSprite;
	};


	std::string ScriptLoader::parseObjectFont(ScriptNodePtr _node)
	{
		std::string firstSprite = _node->getValue("sprite_font");
		std::string::size_type pos = firstSprite.find_first_of(';');
		if (pos != firstSprite.npos)
			return firstSprite.substr(0, pos);
		return firstSprite;
	};


	Vector2 ScriptLoader::parseObjectScale(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("scale");

		if (!value.size())
			return Vector2(1.0f);

		std::string::size_type pos = value.find_first_of(',');
		if (pos == value.npos)
			return Vector2(std::stof(value));

		return Vector2(std::stof(value), std::stof(value.substr(pos + 1)));
	};


	_2d::Quaternion ScriptLoader::parseObjectRotation(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("rotation");
		return _2d::Quaternion(std::stof(value));
	};


	Vector2 ScriptLoader::parseObjectPosition(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("position");
		std::string::size_type pos = value.find_first_of(',');
		if (!value.size() || pos == value.npos)
			return Vector2(0.0f);
		return Vector2(std::stof(value), std::stof(value.substr(pos + 1)));
	};


	_2d::SpritedText::ANCHOR_POSITION ScriptLoader::parseTextAnchorPosition(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("anchor_position");
		unsigned char out = _2d::SpritedText::AP_CENTER;

		if (value.find("left") != std::string::npos)
			out |= _2d::SpritedText::AP_LEFT;
		else if (value.find("right") != std::string::npos)
			out |= _2d::SpritedText::AP_RIGHT;

		if (value.find("top") != std::string::npos)
			out |= _2d::SpritedText::AP_TOP;
		else if (value.find("bottom") != std::string::npos)
			out |= _2d::SpritedText::AP_BOTTOM;

		return (_2d::SpritedText::ANCHOR_POSITION) out;
	};


	std::string ScriptLoader::parseObjectText(ScriptNodePtr _node)
	{
		return _node->getValue("text");
	};


	_2d::Priority ScriptLoader::parseRenderablePriority(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("render_priority");
		int iValue = std::stoi(value);

		if (iValue >= 0 && iValue <= std::numeric_limits<_2d::Priority>::max())
			return (_2d::Priority) iValue;

		if (iValue < 0)
			return 0;

		return std::numeric_limits<_2d::Priority>::max();
	};


	SpriteCoords ScriptLoader::parseSingleSpriteCoords(ScriptNodePtr _node)
	{
		std::string value = _node->getValue("sprite_coords");

		std::string::size_type posBegin = 0, posEnd = 0;
		std::vector<std::string> values;
		// tokenize

		do
		{
			posBegin = posEnd + 1;
			posEnd = value.find_first_of(',', posBegin);

			values.push_back(value.substr(posBegin, posEnd));
		} while(posEnd != value.npos);


		// left, right, top, bottom
		if (values.size() == 4)
			return SpriteCoords(
					std::stof(values[0]),
					std::stof(values[1]),
					std::stof(values[2]),
					std::stof(values[3])
			);

		// v0, v1, v2, v3
		if (values.size() == 8)
			return SpriteCoords(
					Vector2(std::stof(values[0]), std::stof(values[1])),
					Vector2(std::stof(values[2]), std::stof(values[3])),
					Vector2(std::stof(values[4]), std::stof(values[5])),
					Vector2(std::stof(values[6]), std::stof(values[7]))
			);

		// width, height
		if (values.size() == 2)
		{
			float width = std::stof(values[0]);
			float height = std::stof(values[1]);
			return SpriteCoords(width * -0.5f, width * 0.5f, height * 0.5f, height * -0.5f);
		}

		if (values.size() == 1)
		{
			float size = std::stof(values[0]);
			return SpriteCoords(size * -0.5f, size * 0.5f, size * 0.5f, size * -0.5f);
		}

		return SpriteCoords();
	};


	std::list<std::string> ScriptLoader::parseParticlesImageSprites(ScriptNodePtr _node)
	{
		ScriptNodeListPtr children = _node->getChildList();
		ScriptNodePtr spritesNode;
		for (auto it = children->begin(), itEnd = children->end(); it != itEnd; ++it) {
			if ((*it)->getName().compare("Sprites") == 0) {
				spritesNode = (*it);
				break;
			}
		}

		std::list<std::string> spriteNamesList;

		if (spritesNode) {
			children = spritesNode->getChildList();

			for (auto it = children->begin(), itEnd = children->end(); it != itEnd; ++it) {
				if ((*it)->getName().compare("Sprite") == 0)
					spriteNamesList.push_back((*it)->getValue("name"));
			}
		}

		return spriteNamesList;
	};


	ScriptNodeListPtr ScriptLoader::getAnimationNodesList(ScriptNodePtr _node)
	{
		ScriptNodeListPtr nodeList = _node->getChildList();
		for (ScriptNodeListIterator it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			if ((*it)->getName().compare("animations") == 0)
				return (*it)->getChildList();
		}

		return nullptr;
	};


	std::string ScriptLoader::parseAnimationName(ScriptNodePtr _node)
	{
		return _node->getValue("name");
	};


	ScriptNodeListPtr ScriptLoader::getKeyframeNodeList(ScriptNodePtr _node)
	{
		ScriptNodeListPtr nodeList = _node->getChildList();
		for (ScriptNodeListIterator it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			if ((*it)->getName().compare("animation") == 0)
				return (*it)->getChildList();
		}

		assert(0 && "animation frames not found");
	};


	float ScriptLoader::parseKeyframeTime(ScriptNodePtr _node)
	{
		assert(_node->getName().compare("keyframe") == 0 && "not a keyframe node");
		return std::stof(_node->getValue("time"));
	};


	std::string ScriptLoader::parseKeyframeSpriteName(ScriptNodePtr _node)
	{
		assert(_node->getName().compare("keyframe") == 0 && "not a keyframe node");
		return _node->getValue("sprite_name");
	};



	float ScriptLoader::parseAnimationLength(ScriptNodePtr _node)
	{
		return std::stof(_node->getValue("length"));
	};


	std::string ScriptLoader::parseDefaultAnimationName(ScriptNodePtr _node)
	{
		ScriptNodeListPtr childList = _node->getChildList();
		auto childIt = std::find_if(childList->begin(), childList->end(), [] (ScriptNodeListIterator _cnode) { return !(*_cnode)->getName().compare("defaultAnimation"); });
		return childIt != childList->end() ? (*childIt)->getValue("name") : "";
	};


	Animator::ANIMATION_MODE ScriptLoader::parseDefaultAnimationMode(ScriptNodePtr _node)
	{
		ScriptNodeListPtr childList = _node->getChildList();
		auto childIt = std::find_if(childList->begin(), childList->end(), [] (ScriptNodeListIterator _cnode) { return !(*_cnode)->getName().compare("defaultAnimation"); });
		std::string mode = childIt != childList->end() ? (*childIt)->getValue("mode") : "";

		if (mode.compare("repeat") == 0)
			return Animator::ANIMATION_MODE::AM_REPEAT;

		if (mode.compare("loop") == 0)
			return Animator::ANIMATION_MODE::AM_LOOP;

		//default
		//if (mode.compare("once") == 0)
		return Animator::ANIMATION_MODE::AM_ONCE;
	};


	//shapes

	_2d::SHAPE_TYPE ScriptLoader::parseShapeType(ScriptNodePtr _node)
	{
		std::string shape = _node->getValue("shape_type");

		if (shape.compare("circle") == 0)
			return _2d::ST_CIRCLE;

		if (shape.compare("rectangle") == 0)
			return _2d::ST_RECTANGLE;

		if (shape.compare("line area") == 0)
			return _2d::ST_LINE_AREA;

		return _2d::ST_UNDEFINED;
	};


	Vector2 ScriptLoader::parseShapePosition(ScriptNodePtr _node)
	{
		std::string data = _node->getValue("position");
		std::string::size_type pos = data.find_first_of(',');

		if (pos == std::string::npos)
			return Vector2(std::stof(data));

		return Vector2(std::stof(data), std::stof(data.substr(pos+1)));
	};


	float ScriptLoader::parseShapeRadius(ScriptNodePtr _node)
	{
		return std::stof(_node->getValue("radius"));
	};


	Vector2 ScriptLoader::parseRectangleSize(ScriptNodePtr _node)
	{
		std::string data = _node->getValue("size");
		std::string::size_type numPos = data.find_first_of("-+0123456789.");
		std::string::size_type pos = data.find_first_of(',');

		if (numPos == std::string::npos)
			return Vector2(1.0f);

		if (pos == std::string::npos)
			return Vector2(std::stof(data.substr(numPos)));

		return Vector2(std::stof(data.substr(numPos)), std::stof(data.substr(pos+1)));
	};


	float ScriptLoader::parseShapeRotation(ScriptNodePtr _node)
	{
		std::string data = _node->getValue("rotation");
		float rotation = std::stof(data);

		std::string::size_type afterNumPos = data.find_first_not_of("-+0123456789.");

		if (afterNumPos != std::string::npos && (data[afterNumPos] == 'r' || data[afterNumPos] == 'R'))
			return rotation;

		// deg to rad
		return rotation * (M_PI / 180.f);
	};


	Vector3 ScriptLoader::parse2DLineParams(ScriptNodePtr _node)
	{
		return Vector3(std::stof(_node->getValue("param_a")),
					   std::stof(_node->getValue("param_b")),
					   std::stof(_node->getValue("param_c")));

	};


	float ScriptLoader::parsePillLength(ScriptNodePtr _node)
	{
		std::string data = _node->getValue("length");
		return std::stof(data);
	};


	TOUCH_CONTROL_TYPE parseTouchControlType(ScriptNodePtr _node)
	{
		assert(_node->getName().compare("control") == 0 || "Not a touch control node");
		std::string type = _node->getValue("type");

		if (!type.compare("touch_button"))
			return TCT_BUTTON;
		if (!type.compare("touch_area"))
			return TCT_AREA;
		if (!type.compare("object_touch_button"))
			return TCT_OBJECT_BUTTON;
		return TCT_UNDEFINED;
	};


	std::string parseTouchControlName(ScriptNodePtr _node)
	{
		assert(_node->getName().compare("control") == 0 || "Not a touch control node");
		return _node->getValue("name");
	};


	std::string parseTouchControlSet(ScriptNodePtr _node)
	{
		assert(_node->getName().compare("control") == 0 || "Not a touch control node");
		return _node->getValue("set");
	};


	std::string parseTouchControlAttachedToObject(ScriptNodePtr _node)
	{
		assert(_node->getName().compare("control") == 0 || "Not a touch control node");
		return _node->getValue("attached_to");
	};


	ScriptNodePtr getTouchControlShapeNode(ScriptNodePtr _node)
	{
		assert(_node->getName().compare("control") == 0 || "Not a touch control node");

		ScriptNodeListPtr childList = _node->getChildList();
		auto childIt = std::find_if(childList->begin(), childList->end(), [] (ScriptNodeListIterator _cnode) { return !(*_cnode)->getName().compare("shape"); });
		return childIt != childList->end() ? *childIt : nullptr;
	};

}
