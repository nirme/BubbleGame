#pragma once

#include <string>
#include <cstdlib>

#include "Vector2.h"
#include "_2d/Quaternion.h"


#include "SingletonTemplate.h"

#include "ScriptParser.h"
#include "Texture.h"
#include "Shader.h"
#include "ImageSprite.h"
#include "SpritedFont.h"

#include "ScriptNode.h"
#include "_2d/MovableObject.h"
#include "_2d/ObjectFactory.h"
#include "SpriteCoords.h"

#include "DataStream.h"



namespace core
{


	class ScriptLoader : public Singleton<ScriptLoader>
	{
	private:
		ScriptParserPtr parser;

	public:

		ScriptLoader();


		const ScriptParser& getParser();

		void registerParser(ScriptParserPtr _parser);

		ScriptNodeListPtr parse(DataStreamPtr _stream);




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

		std::string parseCodePoint(ScriptNodePtr _node);
		KerningPair parseKerningPair(ScriptNodePtr _node);
		FONT_SPACING parseFontSpacing(ScriptNodePtr _node);
		std::string parseFontTexture(ScriptNodePtr _node);
		float parseSizeMultiplier(ScriptNodePtr _node);
		float parseLineHeight(ScriptNodePtr _node);
		std::string parseDefaultChar(ScriptNodePtr _node);



		// scene node

		Vector2 parseNodeScale(ScriptNodePtr _node);
		_2d::Quaternion parseNodeRotation(ScriptNodePtr _node);
		Vector2 parseNodePosition(ScriptNodePtr _node);



		// object

		OBJECT_TYPE parseRenderableType(ScriptNodePtr _node);

		std::string parseObjectName(ScriptNodePtr _node);
		std::string parseObjectShader(ScriptNodePtr _node);
		std::string parseObjectImageSprite(ScriptNodePtr _node);
		Vector2 parseObjectScale(ScriptNodePtr _node);
		_2d::Quaternion parseObjectRotation(ScriptNodePtr _node);
		Vector2 parseObjectPosition(ScriptNodePtr _node);

		_2d::Priority parseRenderablePriority(ScriptNodePtr _node);
		SpriteCoords parseSingleSpriteCoords(ScriptNodePtr _node);


	};

}
