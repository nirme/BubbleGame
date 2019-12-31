#include "Renderable.h"

namespace core
{
	namespace _2d
	{
		
		void Renderable::setMaterial(ShadingProgramPtr _program, TexturePtr _tex0, TexturePtr _tex1, TexturePtr _tex2, TexturePtr _tex3, TexturePtr _tex4, TexturePtr _tex5, TexturePtr _tex6, TexturePtr _tex7)
		{
			assert(_program && _tex0 && "program/texture cannot be null");

			material = MaterialManager::getSingleton().generateMaterial(_program, _tex0, _tex1, _tex2, _tex3, _tex4, _tex5, _tex6, _tex7);
		};


		Renderable::Renderable(Priority _renderPriority, MaterialPtr _material, bool _bufferable) :
			renderPriority(_renderPriority),
			material(_material),
			bufferable(_bufferable)
		{};


		Renderable::~Renderable()
		{};


		Priority Renderable::getPriority() const
		{
			return renderPriority;
		};

		MaterialId Renderable::getMaterialId() const
		{
			return material->id;
		};

		MaterialPtr Renderable::getMaterial() const
		{
			return material;
		};


		void Renderable::setPriority(Priority _renderPriority)
		{
			renderPriority = _renderPriority;
		};



		BuffWriteResult Renderable::writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite) const
		{
			return BuffWriteResult({ 0,true });
		};

	}
}
