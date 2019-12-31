#include "SingleSprite.h"
#include "SceneNode.h"


namespace core
{
	namespace _2d
	{
		
		AxisAlignedBox SingleSprite::_boundingBoxImpl() const
		{
			SpriteCoords transformedCoords = spriteCoords;
			transformedCoords.transform(MovableObject::getWorldTransform());
			return transformedCoords.getBoundingAABB();
		};


		void SingleSprite::_findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
		{
			_queue->addRenderable(this);
		};


		SingleSprite::SingleSprite(const std::string &_name, Priority _renderPriority, MaterialPtr _material, ImageSpritePtr _sprite) :
			MovableObject(_name),
			Renderable(_renderPriority, _material, true),
			spriteCoords(),
			sprite(nullptr),
			material(nullptr)
		{};


		void SingleSprite::setSpriteCoords(const SpriteCoords &_spriteCoords)
		{
			spriteCoords = _spriteCoords;
			invalidateBoundingBox();
		};


		void SingleSprite::setMaterial(ShadingProgramPtr _program, ImageSpritePtr _sprite)
		{
			sprite = _sprite;
			Renderable::setMaterial(_program, sprite->getTexture());
		};


		const Matrix3& SingleSprite::getTransform() const
		{
			return MovableObject::getWorldTransform();
		};


		BuffWriteResult SingleSprite::writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite) const
		{
			assert(!_fromSprite && "_fromSprite must be 0 for SingleSprite");

			const Matrix3 &mx = MovableObject::getWorldTransform();
			const TextureSpriteCoords &texCoords = sprite->getCoords();


			// 4 verts (x,y),(u,v)
			Vector2 vertices[8] = {
				mx * spriteCoords.uvPoints[0],
				texCoords.uvPoints[0],
				mx * spriteCoords.uvPoints[1],
				texCoords.uvPoints[1],
				mx * spriteCoords.uvPoints[2],
				texCoords.uvPoints[2],
				mx * spriteCoords.uvPoints[3],
				texCoords.uvPoints[3]
			};

			if (_buffer.write<>((float*)vertices, 16))
				return BuffWriteResult({ 1, true });
			return BuffWriteResult({ 0, false });
		};
	}
}
