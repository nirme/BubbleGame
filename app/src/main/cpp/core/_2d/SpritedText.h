#pragma once

#include <memory>
#include <string>
#include "Renderable.h"
#include "MovableObject.h"
#include "../GraphicBuffer.h"
#include "../SpritedFont.h"
#include "../SpriteCoords.h"


namespace core
{
	namespace _2d
	{
		class SpritedText : public MovableObject, public Renderable
		{
		public:
			enum ANCHOR_POSITION : unsigned char
			{
				AP_CENTER = 0x00,
				AP_LEFT = 0x01,
				AP_RIGHT = 0x02,
				AP_TOP = 0x10,
				AP_BOTTOM = 0x20,
				AP_TOP_LEFT = 0x11,
				AP_TOP_RIGHT = 0x12,
				AP_BOTTOM_LEFT = 0x21,
				AP_BOTTOM_RIGHT = 0x22,
			};

		protected:
			SpritedFontPtr textFont;

			std::string text;
			float maxTextWidth;

			SpritedTextVertices vertices;
			unsigned int visibleFrom, visibleCount;

			mutable SpritedTextVertices cashedVertices;
			mutable bool cashedTextNeedUpdate;

			ANCHOR_POSITION textAnchor;
			Vector2 textSize;
			mutable Vector2 textAnchorCorrection;

			mutable bool textCoordsNeedUpdate;
			mutable SpriteCoords textCoords;


			void _updateTextCoords() const;
			void updatePosition() const;

			virtual void _invalidateWorldTransformImpl() const;

			virtual AxisAlignedBox _boundingBoxImpl() const;
			virtual void _findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const;

			void updateCashedText() const;

		public:

			SpritedText(const std::string &_name, Priority _renderPriority = 0, MaterialPtr _material = nullptr);
			virtual ~SpritedText();
			MovableObject* clone(const std::string &_name) const;

			void setMaterial(ShadingProgramPtr _program, SpritedFontPtr _font);

			const Matrix3& getTransform() const;

			BuffWriteResult writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite = 0) const;

			void setAnchorPosition(ANCHOR_POSITION _anchorPosition);
			void setVisibleChars(unsigned int _visibleFrom, unsigned int _visibleCount);
			void setText(std::string _text, float _maxWidth = std::numeric_limits<float>::max());

		};


		typedef std::shared_ptr<SpritedText> SpritedTextPtr;
		typedef std::unique_ptr<SpritedText> SpritedTextUPtr;
	}
}
