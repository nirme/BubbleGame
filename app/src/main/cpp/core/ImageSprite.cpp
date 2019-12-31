#include "ImageSprite.h"
#include "TextureManager.h"
#include "ScriptLoader.h"

namespace core
{

	TextureSpriteCoords::TextureSpriteCoords(float _left, float _right, float _top, float _bottom, bool _inPixels) :
		uvPoints{ {_left, _top}, {_right, _top}, {_left, _bottom}, {_right, _bottom} },
		whRatio(fabsf((_top - _bottom) / (_right - _left))),
		inPixels(_inPixels)
	{};

	TextureSpriteCoords::TextureSpriteCoords(const Vector2 &_v0, const Vector2 &_v1, const Vector2 &_v2, const Vector2 &_v3, bool _inPixels) :
		uvPoints{ _v0, _v1, _v2, _v3 },
		whRatio(fabsf((_v3.y - _v0.y) / (_v3.x - _v0.x))),
		inPixels(_inPixels)
	{};

	TextureSpriteCoords::TextureSpriteCoords(const TextureSpriteCoords &_rhs) :
		uvPoints{_rhs.uvPoints[0], _rhs.uvPoints[1], _rhs.uvPoints[2], _rhs.uvPoints[3]},
		whRatio(_rhs.whRatio),
		inPixels(_rhs.inPixels)
	{};

	TextureSpriteCoords& TextureSpriteCoords::operator=(const TextureSpriteCoords &_sc)
	{
		uvPoints[0] = _sc.uvPoints[0];
		uvPoints[1] = _sc.uvPoints[1];
		uvPoints[2] = _sc.uvPoints[2];
		uvPoints[3] = _sc.uvPoints[3];
		return *this;
	};


	bool TextureSpriteCoords::toTextureSpace(unsigned int _w, unsigned int _h)
	{
		if (inPixels)
		{

			for (int i = 0; i < 4; ++i)
			{
				uvPoints[i].x /= _w;
				uvPoints[i].y = 1.0f - (uvPoints[i].y / _h);
			}

            inPixels = false;
			return true;
		}

		return false;
	};


	void TextureSpriteCoords::correctExpandedTexture(unsigned int _originalW, unsigned int _originalH, unsigned int _w, unsigned int _h)
	{
		float wScale = (float)_originalW / _w;
		float hScale = (float)_originalH / _h;
		uvPoints[0].x *= wScale;
		uvPoints[0].y = hScale * (uvPoints[0].y - 1.0f) + 1.0f;
		uvPoints[1].x *= wScale;
		uvPoints[1].y = hScale * (uvPoints[1].y - 1.0f) + 1.0f;
		uvPoints[2].x *= wScale;
		uvPoints[2].y = hScale * (uvPoints[2].y - 1.0f) + 1.0f;
		uvPoints[3].x *= wScale;
		uvPoints[3].y = hScale * (uvPoints[3].y - 1.0f) + 1.0f;
	}



	ImageSprite::ImageSprite(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager, TexturePtr _texture, const TextureSpriteCoords &_coords) :
		Resource(_name, _handle, _group, _manager),
		texture(_texture),
		coords(_coords)
	{};

	void ImageSprite::loadImp()
	{
		DataStreamPtr data = manager->openResource(ImageSprite::SpriteDefinitionFile);
		ScriptLoader &sloader = ScriptLoader::getSingleton();
		ScriptNodeListPtr spriteDataList = sloader.parse(data);

		ScriptNodePtr spriteData = sloader.findSpriteNode(spriteDataList, getName());

		std::string texName = sloader.parseImgSpriteTexture(spriteData);
		TextureSpriteCoords spriteCoords = sloader.parseImgSpriteCoords(spriteData);

		texture = TextureManager::getSingleton().getByName(texName, getGroup());
        texture->load();

		if (spriteCoords.inPixels)
			spriteCoords.toTextureSpace(texture->getWidth(), texture->getHeight());

		if (texture->getOriginalWidth()!= texture->getWidth() || texture->getOriginalHeight() != texture->getHeight())
			spriteCoords.correctExpandedTexture(texture->getOriginalWidth(), texture->getOriginalHeight(), texture->getWidth(), texture->getHeight());

		coords = spriteCoords;
	};

	void ImageSprite::unloadImp()
	{
		texture.reset();
	};

	unsigned int ImageSprite::sizeCalcImpl()
	{
		unsigned int s = sizeof(TextureSpriteCoords);
		return s;
	};

}
