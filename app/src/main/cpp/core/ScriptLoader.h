#pragma once

#include <string>
#include <cstdlib>

#include "Vector2.h"
#include "_2d/Quaternion.h"


#include "SingletonTemplate.h"
#include "./StringUtils.h"

#include "ScriptParser.h"
#include "Texture.h"
#include "Shader.h"
#include "ImageSprite.h"
#include "SpritedFont.h"

#include "ScriptNode.h"
#include "_2d/MovableObject.h"
#include "_2d/ObjectFactory.h"
#include "SpriteCoords.h"

#include "TouchControlFactory.h"

#include "DataStream.h"

#include "_2d/shapes/ShapeFactory.h"




namespace core
{


	class ScriptLoader : public Singleton<ScriptLoader>
	{
	private:
		ScriptParserPtr parser;

	public:

		// base functions
		ScriptLoader();

		const ScriptParser& getParser();

		void registerParser(ScriptParserPtr _parser);

		ScriptNodeListPtr parse(DataStreamPtr _stream);


		//  generic data

		ScriptNodePtr getChildNodeByName(ScriptNodePtr _parent, std::string _name);
		ScriptNodePtr getChildNodeByNameAndValue(ScriptNodePtr _parent, std::string _name, std::string _attrib="", std::string _value="");


		//  generic script value parsing

		std::string parseResourceName(ScriptNodePtr _node);
		std::string parseResourceGroup(ScriptNodePtr _node);


		//  texture specific script parsing

		TEXTURE_TYPE parseTextureType(ScriptNodePtr _node);
		TEXTURE_FILTER parseTextureFilter(ScriptNodePtr _node);


		//  shader specific script parsing

		SHADER_TYPE parseShaderType(ScriptNodePtr _node);
		std::string parseShaderName(ScriptNodePtr _node);

		ScriptNodePtr findProgramNode(ScriptNodeListPtr _nodeList, const std::string &_name);


		// sprite specific parsing

		ScriptNodePtr findSpriteNode(ScriptNodeListPtr _nodeList, const std::string &_name);
		std::string parseImgSpriteTexture(ScriptNodePtr _node);
		TextureSpriteCoords parseImgSpriteCoords(ScriptNodePtr _node);


		// sprited font

		std::string parseFontName(ScriptNodePtr _node);
		std::string parseCodePoint(ScriptNodePtr _node);
		float parseCharWidth(ScriptNodePtr _node);
		bool parseCharVisibility(ScriptNodePtr _node);
		KerningPair parseKerningPair(ScriptNodePtr _node);
		SpritedFont::FONT_SPACING parseFontSpacing(ScriptNodePtr _node);
		std::string parseFontTexture(ScriptNodePtr _node);
		float parseSizeMultiplier(ScriptNodePtr _node);
		float parseLineHeight(ScriptNodePtr _node);
		std::string parseDefaultChar(ScriptNodePtr _node);


		// scene node

		std::string parseNodeName(ScriptNodePtr _node);
		Vector2 parseNodeScale(ScriptNodePtr _node);
		_2d::Quaternion parseNodeRotation(ScriptNodePtr _node);
		Vector2 parseNodePosition(ScriptNodePtr _node);


		// object

		OBJECT_TYPE parseRenderableType(ScriptNodePtr _node);

		std::string parseObjectName(ScriptNodePtr _node);
		std::string parseObjectShader(ScriptNodePtr _node);
		std::string parseObjectImageSprite(ScriptNodePtr _node);
		std::string parseObjectFont(ScriptNodePtr _node);

		Vector2 parseObjectScale(ScriptNodePtr _node);
		_2d::Quaternion parseObjectRotation(ScriptNodePtr _node);
		Vector2 parseObjectPosition(ScriptNodePtr _node);

		_2d::SpritedText::ANCHOR_POSITION parseTextAnchorPosition(ScriptNodePtr _node);
		std::string parseObjectText(ScriptNodePtr _node);

		_2d::Priority parseRenderablePriority(ScriptNodePtr _node);
		SpriteCoords parseSingleSpriteCoords(ScriptNodePtr _node);

		std::list<std::string> parseParticlesImageSprites(ScriptNodePtr _node);

		ScriptNodeListPtr getAnimationNodesList(ScriptNodePtr _node);

		std::string parseAnimationName(ScriptNodePtr _node);
		ScriptNodeListPtr getKeyframeNodeList(ScriptNodePtr _node);
		float parseKeyframeTime(ScriptNodePtr _node);
		std::string parseKeyframeSpriteName(ScriptNodePtr _node);
		float parseAnimationLength(ScriptNodePtr _node);

		std::string parseDefaultAnimationName(ScriptNodePtr _node);
		Animator::ANIMATION_MODE parseDefaultAnimationMode(ScriptNodePtr _node);


		//shapes

		_2d::SHAPE_TYPE parseShapeType(ScriptNodePtr _node);
		Vector2 parseShapePosition(ScriptNodePtr _node);

		float parseShapeRadius(ScriptNodePtr _node);
		Vector2 parseRectangleSize(ScriptNodePtr _node);
		float parseShapeRotation(ScriptNodePtr _node);
		Vector3 parse2DLineParams(ScriptNodePtr _node);
		float parsePillLength(ScriptNodePtr _node);


		//controls

		TOUCH_CONTROL_TYPE parseTouchControlType(ScriptNodePtr _node);
		std::string parseTouchControlName(ScriptNodePtr _node);
		std::string parseTouchControlSet(ScriptNodePtr _node);
		std::string parseTouchControlAttachedToObject(ScriptNodePtr _node);
		ScriptNodePtr getTouchControlShapeNode(ScriptNodePtr _node);


	};

}
