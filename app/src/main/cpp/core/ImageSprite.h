#pragma once

#include <math.h>
#include "Vector2.h"
#include "Resource.h"
#include "Texture.h"
//#include "TextureManager.h"
//#include "ScriptLoader.h"



namespace core
{
	struct TextureSpriteCoords
	{
		union
		{
			float uvArray[8];
			Vector2 uvPoints[4];
		};

		float whRatio;
		bool inPixels;


		TextureSpriteCoords(float _left = 0.0f, float _right = 1.0f, float _top = 1.0f, float _bottom = 0.0f, bool _inPixels = false);
		TextureSpriteCoords(const Vector2 &_v0, const Vector2 &_v1, const Vector2 &_v2, const Vector2 &_v3, bool _inPixels = false);
		TextureSpriteCoords(const TextureSpriteCoords &_rhs);

		TextureSpriteCoords& operator=(const TextureSpriteCoords &_sc);

		bool toTextureSpace(unsigned int _w, unsigned int _h);
		void correctExpandedTexture(unsigned int _originalW, unsigned int _originalH, unsigned int _w, unsigned int _h);
	};


	class TextureManager;
	class ScriptLoader;

	class ImageSprite : public Resource
	{
	public:

		// !!!need replacing from resource system loader!!!
		constexpr static char *SpriteDefinitionFile = "Sprites.xml";

	protected:

		TexturePtr texture;

		TextureSpriteCoords coords;


	public:

		ImageSprite(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager = nullptr, TexturePtr _texture = nullptr, const TextureSpriteCoords &_coords = TextureSpriteCoords());

		void loadImp();
		void unloadImp();

		unsigned int sizeCalcImpl();

		inline const TextureSpriteCoords& getCoords() const
		{
			return coords;
		};

		inline TexturePtr getTexture() const
		{
			return texture;
		};

	};


	typedef std::shared_ptr<ImageSprite> ImageSpritePtr;

}
