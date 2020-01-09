#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cstring>
#include "../Exceptions.h"

#include "../SpriteCoords.h"
#include "AxisAlignedBox.h"

#include "Renderable.h"
#include "MovableObject.h"
#include "../GraphicBuffer.h"
#include "SpriteCoords.h"

#include "../Texture.h"
#include "../ImageSprite.h"
#include "../Vector2.h"
#include "../Matrix3.h"
#include "../Math2D.h"

#include "Particle.h"


namespace core
{
	namespace _2d
	{
		class ParticleSystem;

		class ParticleIterator
		{
			friend class ParticleSystem;
		private:
			std::list<Particle*>::iterator pos, end;

		public:
			bool isEnd()
			{
				return pos != end;
			};

			Particle* next()
			{
				return isEnd() ? nullptr : (*pos++);
			};
		};


		class ParticleSystem : public MovableObject, public Renderable
		{
		protected:

			struct ParticleData
			{
				struct
				{
					Vector2 position;
					Vector2 texel;
					float age, alpha;
				} vertex[4];
			};
			constexpr static unsigned int particleSize = sizeof(ParticleData) / sizeof(float); // pos{2}, tex{2}, age, alpha?

			std::vector<ImageSpritePtr> sprites;
			std::vector<TextureSpriteCoords> cashedSpriteCoords;


			typedef std::list<Particle*> ParticleList;
			ParticleList aliveParticles;
			ParticleList deadParticles;
			std::vector<Particle> particlesPool;

			typedef std::list<ParticleEmitter*> ParticleEmitterList;
			ParticleEmitterList activeEmitters;

			typedef std::list<ParticleAffector*> ParticleAffectorList;
			ParticleAffectorList activeAffectors;


			float speedFactor;


			void initParticles(unsigned int _particleCount);
			void expireParticles(float _timeElapsed);
			void updateParticles(float _timeElapsed);
			Particle *createParticle();
			void emitParticles(float _timeElapsed);


		public:


			ParticleIterator getActiveParticleIterator();

			ParticleSystem(const std::string &_name, Priority _renderPriority, MaterialPtr _material, std::vector<ImageSpritePtr> &_sprites);

			void initSystem(unsigned int _particleCount = 500);

			void update(float _timeElapsed);

			BuffWriteResult writeVertexData(GraphicBufferPtr _buffer, unsigned int _fromSprite = 0);
		};
	}
}
