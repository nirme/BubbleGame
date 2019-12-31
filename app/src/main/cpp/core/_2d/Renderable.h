#pragma once

#include <memory>
#include "../Matrix3.h"
#include "MaterialManager.h"
#include "AxisAlignedBox.h"

#include "../GraphicBuffer.h"


namespace core
{
	//class GraphicBuffer;

	namespace _2d
	{

		typedef unsigned char Priority;

		struct BuffWriteResult
		{
			//index of sprite for next write operation in case last write was only partial
			unsigned int nextSpriteIndex;

			//true if all data was writen to buffer (nextSpriteIndex=0)
			bool operComplete;
		};

		class Renderable
		{
		protected:

			Priority renderPriority;
			MaterialPtr material;

			bool bufferable;


			void setMaterial(ShadingProgramPtr _program, TexturePtr _tex0, TexturePtr _tex1 = nullptr, TexturePtr _tex2 = nullptr, TexturePtr _tex3 = nullptr, TexturePtr _tex4 = nullptr, TexturePtr _tex5 = nullptr, TexturePtr _tex6 = nullptr, TexturePtr _tex7 = nullptr);


		public:

			Renderable(Priority _renderPriority, MaterialPtr _material, bool _bufferable = true);
			virtual ~Renderable();

			virtual const Matrix3& getTransform() const = 0;
			Priority getPriority() const;
			MaterialId getMaterialId() const;
			MaterialPtr getMaterial() const;

			void setPriority(Priority _renderPriority);

			inline bool isBufferable() const
			{
				return bufferable;
			};

			virtual BuffWriteResult writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite = 0) const;

		};


		typedef std::unique_ptr<Renderable> RenderableUPtr;
		typedef std::shared_ptr<Renderable> RenderablePtr;
	}
}
