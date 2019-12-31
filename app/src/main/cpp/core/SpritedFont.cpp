#include "SpritedFont.h"
#include "TextureManager.h"
#include "ImageSpriteManager.h"
#include "ScriptLoader.h"

namespace core
{

	unsigned int SpritedFont::ToUtf8(Character _char, char *_out)
	{
		if (_char < 0x80)
		{
			if (_out)
			{
				// 0xxxxxxx
				_out[0] = (char)_char;
			}
			return 1;
		}
		else if (_char < 0x800)
		{
			if (_out)
			{
				// 110xxxxx 10xxxxxx
				_out[0] = 0b11000000 | (char)(0b00011111 & (_char >> 6));
				_out[1] = (char)(0b00111111 & _char);
			}
			return 2;
		}
		else if (_char < 0x10000)
		{
			if (_out)
			{
				// 1110xxxx 10xxxxxx 10xxxxxx
				_out[0] = 0b11100000 | (char)(0b00001111 & (_char >> 12));
				_out[1] = 0b10000000 | (char)(0b00111111 & (_char >> 6));
				_out[2] = 0b10000000 | (char)(0b00111111 & _char);
			}
			return 3;
		}
		else //if (_char < 0x110000)
		{
			if (_out)
			{
				// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
				_out[0] = 0b11110000 | (char)(0b00000111 & (_char >> 18));
				_out[1] = 0b10000000 | (char)(0b00111111 & (_char >> 12));
				_out[2] = 0b10000000 | (char)(0b00111111 & (_char >> 6));
				_out[3] = 0b10000000 | (char)(0b00111111 & _char);
			}
			return 4;
		}
	};

	SpritedFont::Character SpritedFont::Utf8ToCodepoint(const char *_in)
	{
        const unsigned char *character = reinterpret_cast<const unsigned char *>(_in);

		if (character[0] < 0x80) // 0xxxxxxx
		{
			return (Character)character[0];
		}
		else if (character[0] < 0xE0) // 110xxxxx 10xxxxxx
		{
			return (((Character)(0b00011111 & character[0])) << 6) |
				(0b00111111 & character[1]);
		}
		else if (character[0] < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
		{
			return (((Character)(0b00001111 & character[0])) << 12) |
				(((Character)(0b00111111 & character[1])) << 6) |
				(0b00111111 & character[2]);
		}
		else // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		{
			return (((Character)(0b00000111 & character[0])) << 18) |
				(((Character)(0b00111111 & character[1])) << 12) |
				(((Character)(0b00111111 & character[2])) << 6) |
				(0b00111111 & character[3]);
		}
	};




	SpritedFont::CharacterPair::CharacterPair(Character _first, Character _second) : 
		first(_first), 
		second(_second)
	{};


	bool SpritedFont::CharacterPair::operator==(const CharacterPair &_rhs) const
	{
		return first == _rhs.first && second == _rhs.second;
	};


	size_t SpritedFont::CharacterPair::Hash::operator()(CharacterPair const& _pair) const noexcept
	{
		return (size_t)(_pair.first ^ _pair.second);
	};


	void SpritedFont::loadImp()
	{
		DataStreamPtr data = manager->openResource(getName());

		ScriptLoader &sloader = ScriptLoader::getSingleton();
		ImageSpriteManager &spriteManager = ImageSpriteManager::getSingleton();

		ScriptNodeListPtr fontData = sloader.parse(data);

		Character defaultCharacter;

		for (auto it = fontData->begin(); it != fontData->end(); ++it)
		{
			if ((*it)->getName().compare(config) == 0)
			{
				spacing = sloader.parseFontSpacing(*it);

				spriteAtlas = TextureManager::getSingleton().getByName(sloader.parseFontTexture(*it), getGroup());

				sizeMultiplier = sloader.parseSizeMultiplier(*it);
				lineHeight = sloader.parseLineHeight(*it);

				defaultCharacter = SpritedFont::Utf8ToCodepoint(sloader.parseDefaultChar(*it).c_str());
			}
			else if ((*it)->getName().compare(charMap) == 0)
			{
				std::string codePoint, spriteName;
				ScriptNodeListPtr chars = (*it)->getChildList();
				for (auto charIt = chars->begin(); charIt != chars->end(); ++charIt)
				{
					//get and parse codepoint value
					codePoint = sloader.parseCodePoint(*charIt);

                    ImageSpritePtr sprite = spriteManager.getByName(spriteAtlas->getName() + "#" + codePoint, getGroup());
                    sprite->load();

					// need to make sure to load spriteAtlas beforehand
					characters.emplace(Utf8ToCodepoint(codePoint.c_str()), sprite);
				}
			}
			else if ((*it)->getName().compare(kerning) == 0)
			{
				ScriptNodeListPtr chars = (*it)->getChildList();
				for (auto charIt = chars->begin(); charIt != chars->end(); ++charIt)
				{
					KerningPair kp = sloader.parseKerningPair(*charIt);
					kerningTable.emplace(CharacterPair(Utf8ToCodepoint(kp.cp1), Utf8ToCodepoint(kp.cp2)), kp.kerning);
				}
			}
		}



		CharacterSpritesMap::iterator it = characters.find(defaultCharacter);

		if (it == characters.end())
		{
			it = characters.find((Character)('?'));

			if (it == characters.end())
				it = characters.begin();
		}

		if (it != characters.end())
			defaultChar = (*it).second;
	};


	void SpritedFont::unloadImp()
	{
		spriteAtlas.reset();
		defaultChar.reset();

		characters.clear();
		kerningTable.clear();
	};


	unsigned int SpritedFont::sizeCalcImpl()
	{
		unsigned int s = sizeof(spacing);

		s += sizeof(spriteAtlas);
		s += sizeof(defaultChar);

		s += characters.size() * (sizeof(CharacterSpritesMap::key_type) + sizeof(CharacterSpritesMap::value_type));
		s += kerningTable.size() * (sizeof(KerningMap::key_type) + sizeof(KerningMap::value_type));

		s += sizeof(sizeMultiplier);
		s += sizeof(lineHeight);

		return s;
	};


	SpritedText SpritedFont::generateSpritedVector(const std::string &_text, float *_vectorWidth, float *_vectorHeight, float _width)
	{
		unsigned int len = _text.length();
		if (!len)
			return SpritedText();

		const char* textArr = _text.c_str();

		Vector2 nextLetterPosition;
		Vector2 spriteSize;
		Vector2 textSize;

		CharacterSpritesMap::iterator charSprite;
		KerningMap::iterator kern;
		ImageSpritePtr sprite;

		SpritedText out(len);

		for (unsigned int i = 0; i < len; ++i)
		{
			charSprite = characters.find((Character)textArr[i]);
			sprite = charSprite != characters.end() ? (*charSprite).second : defaultChar;

			const TextureSpriteCoords &texCoords = sprite->getCoords();
			//fill in texels
			out[i].tex1 = texCoords.uvPoints[0];
			out[i].tex2 = texCoords.uvPoints[1];
			out[i].tex3 = texCoords.uvPoints[2];
			out[i].tex4 = texCoords.uvPoints[3];

           spriteSize = Vector2(
				sizeMultiplier * (texCoords.uvPoints[1].x - texCoords.uvPoints[0].x),
				sizeMultiplier * (texCoords.uvPoints[2].y - texCoords.uvPoints[0].y)
			);

			if (nextLetterPosition.x + spriteSize.x > _width)
			{
				nextLetterPosition.x = 0.0f;
				nextLetterPosition.y -= lineHeight;
            }


            //fill in vertices
			out[i].vert1 = nextLetterPosition;

			out[i].vert2.x = nextLetterPosition.x + spriteSize.x;
            out[i].vert2.y = nextLetterPosition.y;

			out[i].vert3.x = nextLetterPosition.x;
			out[i].vert3.y = nextLetterPosition.y + spriteSize.y;

			out[i].vert4 = nextLetterPosition + spriteSize;


			nextLetterPosition.x += spriteSize.x;
			if (spacing == FONT_SPACING::FS_PROPORTIONAL && i + 1 < len)
			{
				kern = kerningTable.find(CharacterPair((Character)textArr[i], (Character)textArr[i + 1]));
				if (kern != kerningTable.end())
					nextLetterPosition.x += (*kern).second;
			}
		}


		if (out.back().vert4.x > textSize.x)
			textSize.x = out.back().vert4.x;

		textSize.y = out.back().vert4.y;

		if (_vectorWidth)
			*_vectorWidth = textSize.x;

		if (_vectorHeight)
			*_vectorHeight = textSize.y;

		return out;
	};



}
