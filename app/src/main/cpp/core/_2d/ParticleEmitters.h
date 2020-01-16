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
            constexpr static unsigned short Mask = 0x3FF;
            constexpr static float Div = 1.0f / (Mask + 1);

        public:

            RandomParticleEmitter(ParticleSystem *_parent);
            void initParticle(Particle *_prt);
        };


        class PointRandomParticleEmitter : public ParticleEmitter
        {
        protected:
            constexpr static unsigned short Mask = 0x3FF;
            constexpr static float Div = 1.0f / (Mask + 1);

        public:

            PointRandomParticleEmitter(ParticleSystem *_parent);
            void initParticle(Particle *_prt);
        };

    }
}
