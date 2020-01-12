#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cstring>
#include <cstdlib>
#include "Particle.h"
//#include "ParticleSystem.h"
#include "../Texture.h"
#include "../ImageSprite.h"
#include "../Vector2.h"
#include "../math.h"


namespace core
{
    namespace _2d
    {

        class RandomParticleEmitter : public ParticleEmitter
        {
        protected:
            constexpr static float Div = 1.0f / 256.0f;

        public:

            RandomParticleEmitter(ParticleSystem *_parent);

            void initParticle(Particle *_prt);

        };
    }
}
