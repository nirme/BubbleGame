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
	//used in loading
	struct KerningPair
	{
		char cp1[4];
		char cp2[4];
		float kerning;
	};


#pragma pack(push, 1)
	struct __attribute__ ((packed)) CharacterSprite
	{
		union
		{
			Vector2 _values[8];
			struct
			{
				Vector2 vertex0;
				Vector2 texel0;
				Vector2 vertex1;
				Vector2 texel1;
				Vector2 vertex2;
				Vector2 texel2;
				Vector2 vertex3;
				Vector2 texel3;
			};
		};

		CharacterSprite(){};
		CharacterSprite(const CharacterSprite& _rhs) :
			_values{_rhs._values[0], _rhs._values[1], _rhs._values[2], _rhs._values[3],
					 _rhs._values[4], _rhs._values[5], _rhs._values[6], _rhs._values[7]}
		{};

	};
#pragma pack(pop)

	typedef std::vector<CharacterSprite> SpritedTextVertices;


	class SpritedFont : public Resource
	{
	public:
		enum FONT_SPACING
		{
			FS_MONOSPACE = 0x00,
			FS_PROPORTIONAL = 0x01,
		};

		static constexpr unsigned int LetterElementCount = sizeof(CharacterSprite) / sizeof(float);

		typedef wchar_t Character;

		static unsigned int ToUtf8(Character _char, char *_out);
		static bool Utf8ToCodepoint(const char *_in, Character &_cp);
		static bool Utf8ToCodepoint(const std::string &_in, std::wstring &_out);

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

		struct CharSprite
		{
			Character cp;
			ImageSpritePtr sprite;
			float width, height;
			bool visible;

			CharSprite(Character _cp = 0, ImageSpritePtr _sprite = nullptr, float _width = 0, float _height = 0, bool _visible = false) :
				cp(_cp), sprite(_sprite), width(_width), height(_height), visible(_visible)
			{};

			void clear()
			{
				cp = 0;
				sprite.reset();
				width = 0.0f;
				height = 0.0f;
				visible = false;
			};
		};


		typedef std::unordered_map<Character, CharSprite> CharacterSpritesMap;
		typedef std::unordered_map<CharacterPair, float, CharacterPair::Hash> KerningMap;

		FONT_SPACING spacing;

		TexturePtr spriteAtlas;

		CharacterSpritesMap characters;
		CharSprite defaultChar;
		KerningMap kerningTable;

		float sizeMultiplier;
		float lineHeight;


	public:

		SpritedFont(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager) :
				Resource(_name, _handle, _group, _manager),
				spacing(FS_MONOSPACE),
				spriteAtlas(nullptr),
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


		SpritedTextVertices generateSpritedVector(const std::string &_text, Vector2 *_textSize = nullptr, float _maxWidth = std::numeric_limits<float>::max());
		SpritedTextVertices generateSpritedVector(const std::wstring &_text, Vector2 *_textSize = nullptr, float _maxWidth = std::numeric_limits<float>::max());
	};


	typedef std::shared_ptr<SpritedFont> SpritedFontPtr;
}


