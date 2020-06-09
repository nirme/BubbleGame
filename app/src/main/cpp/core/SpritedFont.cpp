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

	bool SpritedFont::Utf8ToCodepoint(const char *_in, Character &_cp)
	{
		assert(_in && "char string cannot be null");
        //const unsigned char *character = reinterpret_cast<const unsigned char *>(_in);

		if ((_in[0] & 0b10000000) == 0) // 0xxxxxxx
		{
			_cp = _in[0];
			return true;
		}

		if ((_in[0] & 0b11100000) == 0b11000000) // 110xxxxx 10xxxxxx
		{
			if ((_in[1] & 0b11000000) == 0b10000000)
			{
				_cp = (((Character)(_in[0] & 0b00011111)) << 6) | (_in[1] & 0b00111111);
				return true;
			}
			return false;
		}

		if ((_in[0] & 0b11110000) == 0b11100000) // 1110xxxx 10xxxxxx 10xxxxxx
		{
			if ((_in[1] & 0b11000000) == 0b10000000 && (_in[2] & 0b11000000) == 0b10000000)
			{
				_cp = (((Character)(_in[0] & 0b00001111)) << 12) |
					(((Character)(_in[1] & 0b00111111)) << 6) |
					(_in[2] & 0b00111111);
				return true;
			}
			return false;
		}

		if ((_in[0] & 0b11111000) == 0b11110000) // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		{
			if ((_in[1] & 0b11000000) == 0b10000000 && (_in[2] & 0b11000000) == 0b10000000 && (_in[3] & 0b11000000) == 0b10000000)
			{
				_cp = (((Character)(_in[0] & 0b00000111)) << 18) |
					(((Character)(_in[1] & 0b00111111)) << 12) |
					(((Character)(_in[2] & 0b00111111)) << 6) |
					(_in[3] & 0b00111111);
				return true;
			}
			return false;
		}

		return false;
	};

	bool SpritedFont::Utf8ToCodepoint(const std::string &_in, std::wstring &_out)
	{
		assert(sizeof(wchar_t) == 4 && "wchar_t must be 4 bytes");
		const char *ptr ,*text = _in.c_str();
		unsigned int c(0), len(_in.length());
		std::wstring out;

		while (c < len)
		{
			ptr = text + c;

			if ((ptr[0] & 0b10000000) == 0) // 0b0#######
			{
				out.push_back(ptr[0]);
				c += 1;
				continue;
			}
			else if ((ptr[0] & 0b11100000) == 0b11000000) // 0b110#####
			{
				if (len - c >= 2 && ((ptr[1] & 0b11000000) == 0b10000000)) // 0b10######
				{
					out.push_back(
						( ( (wchar_t) ( ptr[0] | 0b00011111 ) ) << 6 ) |
						( ptr[1] | 0b00111111 ) );
					c += 2;
					continue;
				}
			}
			else if ((ptr[0] & 0b11110000) == 0b11100000) // 0b1110####
			{
				if (len - c >= 3 && ((ptr[1] & 0b11000000) == 0b10000000) && ((ptr[2] & 0b11000000) == 0b10000000)) // 0b10###### 0b10######
				{
					out.push_back(
							( ( (wchar_t) ( ptr[0] | 0b00001111 ) ) << 12 ) |
							( ( (wchar_t) ( ptr[1] | 0b00111111 ) ) << 6  ) |
							( ptr[2] | 0b00111111 ) );
					c += 3;
					continue;
				}
			}
			else if ((ptr[0] & 0b11111000) == 0b11110000) // 0b11110###
			{
				if (len - c >= 4 && ((ptr[1] & 0b11000000) == 0b10000000) && ((ptr[2] & 0b11000000) == 0b10000000) && ((ptr[3] & 0b11000000) == 0b10000000)) // 0b10###### 0b10###### 0b10######
				{
					out.push_back(
							( ( (wchar_t) ( ptr[0] | 0b00000111 ) ) << 18 ) |
							( ( (wchar_t) ( ptr[1] | 0b00111111 ) ) << 12 ) |
							( ( (wchar_t) ( ptr[2] | 0b00111111 ) ) << 6  ) |
							( ptr[3] | 0b00111111 ) );
					c += 3;
					continue;
				}
			}
			return false;
		}

		out.swap(_out);
		return true;
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

		std::wstring defaultCharacter;
		std::string spriteNamePrefix;

		ScriptNodePtr configNode, charMapNode, kerningNode;

		for (auto it = fontData->begin(); it != fontData->end(); ++it)
		{
			if ((*it)->getName().compare(config) == 0)
				configNode = (*it);
			else if ((*it)->getName().compare(charMap) == 0)
				charMapNode = (*it);
			else if ((*it)->getName().compare(kerning) == 0)
				kerningNode = (*it);
		}


		assert(configNode && "config node not present");
		assert(charMapNode && "character map node not present");

		{
			std::string textureName = sloader.parseFontTexture(configNode);
			spriteAtlas = TextureManager::getSingleton().getByName(sloader.parseFontTexture(configNode), getGroup());
			spriteAtlas->load();
			std::string fontName = sloader.parseFontName(configNode);
			spriteNamePrefix = spriteAtlas->getName() + "#" + fontName + "#";

			spacing = sloader.parseFontSpacing(configNode);
			sizeMultiplier = sloader.parseSizeMultiplier(configNode);
			lineHeight = sloader.parseLineHeight(configNode);

			//get default char
			Utf8ToCodepoint(sloader.parseDefaultChar(configNode), defaultCharacter);
			defaultChar.clear();
		}

		{
			std::string codePointUTF8;
			std::wstring codePoint;
			float charWidth, charHeight;
			bool visibility;
			ScriptNodeListPtr chars = charMapNode->getChildList();
			for (auto charIt = chars->begin(); charIt != chars->end(); ++charIt)
			{
				//get and parse codepoint value
				codePointUTF8 = sloader.parseCodePoint(*charIt);
				Utf8ToCodepoint(codePointUTF8, codePoint);

				charWidth = sloader.parseCharWidth(*charIt);
				visibility = sloader.parseCharVisibility(*charIt);

				ImageSpritePtr sprite(nullptr);
				if (visibility)
				{
					sprite = spriteManager.getByName(spriteNamePrefix + codePointUTF8, getGroup());
					sprite->load();
					Vector2 size = sprite->getCoords().uvPoints[3] - sprite->getCoords().uvPoints[0];
					charHeight = (charWidth / size.x) * size.y;
				}

				if (codePoint.compare(defaultCharacter) == 0)
					defaultChar = CharSprite(-1, sprite, charWidth, charHeight, visibility);

				if (codePoint.length() == 1)
					characters.emplace(codePoint[0], CharSprite(codePoint[0], sprite, charWidth, charHeight, visibility) );
			}
		}

		{
			ScriptNodeListPtr chars = kerningNode->getChildList();
			Character cp1, cp2;

			for (auto charIt = chars->begin(); charIt != chars->end(); ++charIt)
			{
				KerningPair kp = sloader.parseKerningPair(*charIt);
				Utf8ToCodepoint(kp.cp1, cp1);
				Utf8ToCodepoint(kp.cp2, cp2);
				kerningTable.emplace(CharacterPair(cp1, cp2), kp.kerning);
			}
		}


		if (defaultChar.cp == 0)
		{
			auto it = characters.find(L'?');
			if (it != characters.end())
			{
				defaultChar = (*it).second;
				defaultChar.cp = -1;
			}
			else
			{
				it = characters.begin();
				if (it != characters.end())
				{
					defaultChar = (*it).second;
					defaultChar.cp = -1;
				}
			}
		}
	};


	void SpritedFont::unloadImp()
	{
		spriteAtlas.reset();
		characters.clear();
		defaultChar.clear();
		kerningTable.clear();
		sizeMultiplier = 0.0f;
		lineHeight = 0.0f;
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


	SpritedTextVertices SpritedFont::generateSpritedVector(const std::string &_text, Vector2 *_textSize, float _maxWidth)
	{
		std::wstring wText;
		bool test = Utf8ToCodepoint(_text, wText);
		assert(test && "incorrect UTF8 encoding, cannot convert text");
		return generateSpritedVector(wText, _textSize, _maxWidth);
	};


	SpritedTextVertices SpritedFont::generateSpritedVector(const std::wstring &_text, Vector2 *_textSize, float _maxWidth)
	{
		std::wstring preparedText;
		preparedText.reserve(_text.size());

		for (unsigned int i = 0, iEnd = _text.size(); i < iEnd; ++i)
		{
			if (_text[i] < 0x20 && _text[i] != L'\n')
			{
				// tab to 4 spaces
				if (_text[i] == 0x09)
					preparedText += L"    ";
			}
			else
				preparedText += _text[i];
		}


		unsigned int len = preparedText.length();
		if (!len)
			return SpritedTextVertices();

		const Character *textArr = preparedText.c_str();

		Vector2 nextLetterPosition;
		Vector2 spriteSize;
		Vector2 textSize;
		float kerning(0.0f);

		CharacterSpritesMap::iterator charSpriteIt;
		KerningMap::iterator kern;
		//CharSprite sprite;

		SpritedTextVertices out(len);

		unsigned int outIndex = 0;

		for (unsigned int i = 0; i < len; ++i)
		{
			if (textArr[i] < 0x20)
			{
				switch (textArr[i])
				{
					case L'\n':
					{
						nextLetterPosition.y -= lineHeight;

						if (textSize.x < nextLetterPosition.x)
							textSize.x = nextLetterPosition.x;
					}
					case L'\r':
						nextLetterPosition.x = 0.0f;
						break;
				}

				continue;
			}

			charSpriteIt = characters.find((Character)textArr[i]);
			CharSprite &charSprite = (charSpriteIt != characters.end()) ? (*charSpriteIt).second : defaultChar;


			// move due to kerning
			if (spacing == FS_PROPORTIONAL && i > 0)
			{
				auto kernIt = kerningTable.find({preparedText[i-1], preparedText[i]});
				if (kernIt != kerningTable.end())
					nextLetterPosition.x += (*kernIt).second;
			}

			//spriteSize = Vector2(sizeMultiplier * charSprite.width, sizeMultiplier * charSprite.height);
			spriteSize = Vector2(sizeMultiplier * charSprite.width, -sizeMultiplier * charSprite.height);

			// check char visibility
			if (!charSprite.visible)
			{
				nextLetterPosition.x += spriteSize.x;
				continue;
			}


			// move to next line if needed
			if (nextLetterPosition.x + spriteSize.x > _maxWidth)
			{
				if (textSize.x < nextLetterPosition.x)
					textSize.x = nextLetterPosition.x;

				nextLetterPosition.x = 0.0f;
				nextLetterPosition.y -= lineHeight;
            }


            //fill in vertices
			out[outIndex].vertex0 = nextLetterPosition;

			out[outIndex].vertex1.x = nextLetterPosition.x + spriteSize.x;
            out[outIndex].vertex1.y = nextLetterPosition.y;

			out[outIndex].vertex2.x = nextLetterPosition.x;
			out[outIndex].vertex2.y = nextLetterPosition.y - spriteSize.y;

			out[outIndex].vertex3.x = nextLetterPosition.x + spriteSize.x;
			out[outIndex].vertex3.y = nextLetterPosition.y - spriteSize.y;


			//fill in texels
			const TextureSpriteCoords &texCoords = charSprite.sprite->getCoords();
			out[outIndex].texel0 = texCoords.uvPoints[0];
			out[outIndex].texel1 = texCoords.uvPoints[1];
			out[outIndex].texel2 = texCoords.uvPoints[2];
			out[outIndex].texel3 = texCoords.uvPoints[3];


			nextLetterPosition.x += spriteSize.x;
			++outIndex;
		}

		// drop length in case we got nonrendered chars
		out.resize(outIndex);

		if (out.back().vertex3.x > textSize.x)
			textSize.x = out.back().vertex3.x;
		textSize.y = out.back().vertex3.y;


		if (_textSize)
			*_textSize = textSize;

		return out;
	};



}
