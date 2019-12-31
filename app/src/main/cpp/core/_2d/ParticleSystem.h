#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cstring>
#include "Renderable.h"
#include "Particle.h"
#include "../Texture.h"
#include "../ImageSprite.h"
#include "../Vector2.h"
#include "../math.h"


namespace core
{
	namespace _2d
	{

		class ParticleSystem : public Renderable
		{
		protected:


			std::vector<ImageSpritePtr> sprites;
			std::vector<TextureSpriteCoords> cashedSpriteCoords;


			std::list<Particle*> aliveParticles;
			std::list<Particle*> deadParticles;
			std::vector<Particle> particlesPool;


			void _initParticlePool;

		public:

			SingleSprite(Priority _renderPriority, MaterialPtr _material, ImageSpritePtr _sprite = nullptr, SceneNode *_parent = nullptr);

			void setResources(ShadingProgramPtr _program, ImageSpritePtr _sprite);

			virtual BuffWriteResult writeVertexData(GraphicBufferPtr _buffer, unsigned int _fromSprite = 0);
		};
	}
}
