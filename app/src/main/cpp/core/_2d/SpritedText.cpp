#include "SpritedText.h"


namespace core
{
	namespace _2d
	{

		void SpritedText::_updateTextCoords() const
		{
			if (visibleFrom == 0 && visibleCount == text.length())
			{
				textCoords = SpriteCoords(0.0f, textSize.x, 0.0f, textSize.y);
			}
			else if (visibleCount > 0)
			{
				Vector2 vMin(vertices[visibleFrom].vertex2), vMax(vertices[visibleFrom].vertex1);

				for (unsigned int i = visibleFrom + 1, iEnd = visibleFrom + visibleCount; i < iEnd; ++i)
				{
					if (vMin.x > vertices[i].vertex2.x) vMin.x = vertices[i].vertex2.x;
					if (vMin.y > vertices[i].vertex2.y) vMin.y = vertices[i].vertex2.y;

					if (vMin.x < vertices[i].vertex1.x) vMin.x = vertices[i].vertex1.x;
					if (vMin.y < vertices[i].vertex1.y) vMin.y = vertices[i].vertex1.y;
				}

				textCoords = SpriteCoords(vMin, vMax);
			}

			textCoordsNeedUpdate = false;
		};


		void SpritedText::updatePosition()
		{
			if (textCoordsNeedUpdate)
				_updateTextCoords();

			AxisAlignedBox aabb = textCoords.getBoundingAABB();
			Vector2 size = aabb.getMaximum() - aabb.getMinimum();
			Vector2 position = aabb.getMinimum();

			// horizontal
			if ((textAnchor & 0x0F) == AP_LEFT)
				;
			else if ((textAnchor & 0x0F) == AP_RIGHT)
				position.x = -aabb.getMaximum().x;
			else
				position.x += (aabb.getMaximum().x - aabb.getMinimum().x) * 0.5f;

			// vertical
			if ((textAnchor & 0xF0) == AP_TOP)
				;
			else if ((textAnchor & 0xF0) == AP_BOTTOM)
				position.y = -aabb.getMaximum().y;
			else
				position.y += (aabb.getMaximum().y - aabb.getMinimum().y) * 0.5f;

			setPosition(position);
		};


		void SpritedText::_invalidateWorldTransformImpl() const
		{
			cashedTextNeedUpdate = true;
		};


		AxisAlignedBox SpritedText::_boundingBoxImpl() const
		{
			if (textCoordsNeedUpdate)
				_updateTextCoords();

			SpriteCoords coords(textCoords);
			coords.transform(MovableObject::getWorldTransform());
			return coords.getBoundingAABB();
		};


		void SpritedText::_findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
		{
			_queue->addRenderable(this);
		};


		void SpritedText::updateCashedText() const
		{
			assert(cashedTextNeedUpdate && "cashe don't need to be updated");

			const Matrix3& mx = getTransform();
			cashedVertices.resize(vertices.size());

			for (unsigned int i = 0, iMax = vertices.size(); i < iMax; ++i)
			{
				cashedVertices[i].vertex0 = mx * vertices[i].vertex0;
				cashedVertices[i].texel0 = vertices[i].texel0;
				cashedVertices[i].vertex1 = mx * vertices[i].vertex1;
				cashedVertices[i].texel1 = vertices[i].texel1;
				cashedVertices[i].vertex2 = mx * vertices[i].vertex2;
				cashedVertices[i].texel2 = vertices[i].texel2;
				cashedVertices[i].vertex3 = mx * vertices[i].vertex3;
				cashedVertices[i].texel3 = vertices[i].texel3;
			}

			cashedTextNeedUpdate = false;
		};


		SpritedText::SpritedText(const std::string &_name, Priority _renderPriority, MaterialPtr _material) :
				MovableObject(_name),
				Renderable(_renderPriority, _material, true),
				textFont(nullptr),
				cashedTextNeedUpdate(true),
				visibleFrom(0),
				visibleCount(0),
				textAnchor(AP_CENTER),
				textCoordsNeedUpdate(true)
		{};



		void SpritedText::setMaterial(ShadingProgramPtr _program, SpritedFontPtr _font)
		{
			textFont = _font;
			Renderable::setMaterial(_program, textFont->getTexture());

			if (text.length() && textFont)
			{
				vertices = textFont->generateSpritedVector(text, &textSize);
				textCoordsNeedUpdate = true;
				cashedTextNeedUpdate = true;
			}
		};


		const Matrix3& SpritedText::getTransform() const
		{
			return MovableObject::getWorldTransform();
		};


		BuffWriteResult SpritedText::writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite) const
		{
			assert(_buffer.getElementType() == GL_FLOAT && "incorrect buffer type used");

			if (cashedTextNeedUpdate)
				updateCashedText();

			unsigned int availableSize = _buffer.getRemainingLength() / SpritedFont::LetterElementCount;

			if (_fromSprite < visibleFrom)
				_fromSprite = visibleFrom;

			unsigned int beginDist = _fromSprite - visibleFrom ;

			// write till the end
			if (availableSize >= (visibleCount - beginDist))
			{
				_buffer.write((const float*) &cashedVertices[_fromSprite], (visibleCount - beginDist) * SpritedFont::LetterElementCount);
				return BuffWriteResult({visibleCount - beginDist, true});
			}
				// write partial
			else
			{
				_buffer.write((const float*) &cashedVertices[_fromSprite], availableSize * SpritedFont::LetterElementCount);
				return BuffWriteResult({availableSize, false});
			}
		};


		void SpritedText::setAnchorPosition(ANCHOR_POSITION _anchorPosition)
		{
			if (textAnchor != _anchorPosition)
			{
				textAnchor = _anchorPosition;
				textCoordsNeedUpdate = true;
				updatePosition();
			}
		};


		unsigned int SpritedText::getVisibleCharsFrom() const
		{
			return visibleFrom;
		};


		unsigned int SpritedText::getVisibleCharsCount() const
		{
			return visibleCount;
		};


		void SpritedText::setVisibleChars(unsigned int _visibleFrom, unsigned int _visibleCount)
		{
			if (_visibleFrom > text.length())
				_visibleFrom = text.length();

			if (_visibleFrom + _visibleCount)
				_visibleCount = text.length() - _visibleFrom;

			if (visibleFrom != _visibleFrom && visibleCount != _visibleCount)
			{
				visibleFrom = _visibleFrom;
				visibleCount = _visibleCount;
				textCoordsNeedUpdate = true;
				updatePosition();
			}
		};


		void SpritedText::setText(std::string _text)
		{
			text = _text;
			visibleFrom = 0;
			visibleCount = text.length();
			cashedTextNeedUpdate = true;

			if (textFont)
			{
				cashedVertices.clear();
				vertices = textFont->generateSpritedVector(text, &textSize);
			}

		};

	}
}
