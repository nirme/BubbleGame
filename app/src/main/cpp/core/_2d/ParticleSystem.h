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
			inline bool isEnd()
			{
				return pos == end;
			};

			inline Particle* next()
			{
                Particle *ptr = isEnd() ? nullptr : (*pos);
				++pos;
				return ptr;
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
					float alpha, age;
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


			AxisAlignedBox _boundingBoxImpl() const;
			void _findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const;

			void convertParticleToWorldSpace(Particle *_prt);

			void initParticles(unsigned int _particleCount);
			void expireParticles(float _timeElapsed);
			void updateParticles(float _timeElapsed);
			Particle *createParticle();
			void emitParticles(float _timeElapsed);


		public:

			ParticleIterator getActiveParticleIterator();
			ParticleSystem(const std::string &_name, Priority _renderPriority = 0, MaterialPtr _material = nullptr);

			short getSpriteIndex(const std::string &_spriteName);

            void setSpeedFactor(float _speedFactor);
            void addSprite(ImageSpritePtr _sprite);
            void addEmiter(ParticleEmitter *_emiter);
            void addAffector(ParticleAffector *_affector);
            void initSystem(unsigned int _particleCount = 500);

			void update(float _timeElapsed);


			void setMaterial(ShadingProgramPtr _program, TexturePtr _tex);
			const Matrix3& getTransform() const;
			virtual void notifyAttached(SceneNode* _parent);
			virtual BuffWriteResult writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite = 0) const;

        };

        typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;
        typedef std::unique_ptr<ParticleSystem> ParticleSystemUPtr;
	}
}
