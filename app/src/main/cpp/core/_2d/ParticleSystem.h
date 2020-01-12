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

#include "../Texture.h"
#include "../ImageSprite.h"
#include "../Vector2.h"
#include "../Matrix3.h"
#include "../Math2D.h"

#include "../ControllerManager.h"

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

			ControllerPtr controller;

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


			void convertParticleToWorldSpace(Particle *_prt)
			{
				const Matrix3 &transform = this->getWorldTransform();
				_prt->position = transform * _prt->position;
				_prt->direction = transformVector(transform, _prt->direction);
			};

			void initParticles(unsigned int _particleCount);
			void expireParticles(float _timeElapsed);
			void updateParticles(float _timeElapsed);
			Particle *createParticle();
			void emitParticles(float _timeElapsed);


		public:


			short getSpriteIndex(const std::string &_spriteName)
			{
				for (short i =0, iEnd = sprites.size(); i < iEnd; ++i)
				{
					if (_spriteName.compare(sprites[i]->getName()) == 0)
						return i;
				}

				return -1;
			};


			ParticleIterator getActiveParticleIterator();

			ParticleSystem(const std::string &_name, Priority _renderPriority = 0, MaterialPtr _material = nullptr);

			virtual void notifyAttached(SceneNode* _parent)
			{
				MovableObject::notifyAttached(_parent);

				if (parent && !controller)
				{
					controller = ControllerManager::getSingleton().createFrameTimeController(new TTimeframeControllerValue<ParticleSystem>(this));
				}
			};

            void setMaterial(ShadingProgramPtr _program, TexturePtr _tex);

            void setSpeedFactor(float _speedFactor);

            void addSprite(ImageSpritePtr _sprite);

            void addEmiter(ParticleEmitter *_emiter);

            void addAffector(ParticleAffector *_affector);

            void initSystem(unsigned int _particleCount = 500);

			void update(float _timeElapsed);

			BuffWriteResult writeVertexData(GraphicBufferPtr _buffer, unsigned int _fromSprite = 0);
		};

        typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;
        typedef std::unique_ptr<ParticleSystem> ParticleSystemUPtr;
	}
}
