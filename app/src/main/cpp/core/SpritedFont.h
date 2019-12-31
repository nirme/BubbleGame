#pragma once

#include <memory>
#include <limits>
#include <string>
#include <unordered_map>

#include "Vector2.h"
#include "Resource.h"
#include "Texture.h"
#include "ImageSprite.h"
//#include "ImageSpriteManager.h"





namespace core
{
	struct CharacterSprite
	{
		union
		{
			Vector2 s[8];
			struct
			{
				Vector2 vert1;
				Vector2 tex1;
				Vector2 vert2;
				Vector2 tex2;
				Vector2 vert3;
				Vector2 tex3;
				Vector2 vert4;
				Vector2 tex4;
			};
		};

		CharacterSprite() {};
	};

	struct KerningPair
	{
		char cp1[4];
		char cp2[4];
		float kerning;
	};

	enum FONT_SPACING
	{
		FS_MONOSPACE = 0x00,
		FS_PROPORTIONAL = 0x01,
	};


	typedef std::vector<CharacterSprite> SpritedText;


	class SpritedFont : public Resource
	{
	public:
		typedef unsigned int Character;

		static unsigned int ToUtf8(Character _char, char *_out);
		static Character Utf8ToCodepoint(const char *_out);

	protected:

		constexpr static char* config = "configuration";
		constexpr static char* charMap = "characters";
		constexpr static char* kerning = "kerning";


		struct CharacterPair
		{
			Character first, second;
			CharacterPair(Character _first, Character _second);
			bool operator==(const CharacterPair &_rhs) const;

			struct Hash
			{
				size_t operator()(CharacterPair const& _pair) const noexcept;
			};
		};



		typedef std::unordered_map<Character, ImageSpritePtr> CharacterSpritesMap;
		typedef std::unordered_map<CharacterPair, float, CharacterPair::Hash> KerningMap;

		FONT_SPACING spacing;

		TexturePtr spriteAtlas;

		CharacterSpritesMap characters;
		ImageSpritePtr defaultChar;
		KerningMap kerningTable;

		float sizeMultiplier;
		float lineHeight;


	public:

		SpritedFont(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager) :
				Resource(_name, _handle, _group, _manager),
				spacing(FS_MONOSPACE),
				spriteAtlas(nullptr),
				defaultChar(nullptr),
				sizeMultiplier(1.0f),
				lineHeight(1.0f)
		{};

		void loadImp();
		void unloadImp();

		unsigned int sizeCalcImpl();

		TexturePtr getTexture()
		{
			return spriteAtlas;
		}


		SpritedText generateSpritedVector(const std::string &_text, float *_vectorWidth = nullptr, float *_vectorHeight = nullptr, float _width = std::numeric_limits<double>::max());
	};


	typedef std::shared_ptr<SpritedFont> SpritedFontPtr;
}


