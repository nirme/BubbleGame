#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cstring>

#include "../SpriteCoords.h"
#include "AxisAlignedBox.h"

#include "Renderable.h"
#include "MovableObject.h"
#include "../GraphicBuffer.h"

#include "../Texture.h"
#include "../ImageSprite.h"
#include "../Vector2.h"


namespace core
{
	namespace _2d
	{
		class SingleSprite : public MovableObject, public Renderable
		{
		protected:

			SpriteCoords spriteCoords;

			ImageSpritePtr sprite;
			MaterialPtr material; //this contains shader and texture already


			virtual AxisAlignedBox _boundingBoxImpl() const;
			virtual void _findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const;

		public:

			SingleSprite(const std::string &_name, Priority _renderPriority = 0, MaterialPtr _material = nullptr, ImageSpritePtr _sprite = nullptr);
			~SingleSprite() {};

			void setSpriteCoords(const SpriteCoords &_spriteCoords);

			void setMaterial(ShadingProgramPtr _program, ImageSpritePtr _sprite);

			const Matrix3& getTransform() const;

			BuffWriteResult writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite = 0) const;
		};


		typedef std::shared_ptr<SingleSprite> SingleSpritePtr;
		typedef std::unique_ptr<SingleSprite> SingleSpriteUPtr;
	}
}
