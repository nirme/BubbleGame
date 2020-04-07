#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES
#include "Particle.h"
//#include "ParticleSystem.h"
#include "../Texture.h"
#include "../ImageSprite.h"
#include "../Vector2.h"
#include "../Math2D.h"


namespace core
{
    namespace _2d
    {
        class ParticleSpeedAffector : public ParticleAffector
        {
        public:
            enum SPEED_MODIFIER_OPERATION
            {
                SMO_ADD = 0x00,
                SMO_AVERAGE = 0x01,
            };

        protected:
            SPEED_MODIFIER_OPERATION operation;
            Vector2 direction;


        public:

            ParticleSpeedAffector(ParticleSystem *_parent,
                                  SPEED_MODIFIER_OPERATION _operation = SMO_ADD,
                                  Vector2 _direction = 0.0f);
            void setOperation(SPEED_MODIFIER_OPERATION _operation);
            void setDirection(Vector2 _direction);
            Vector2 getDirection();
            virtual void initParticle(Particle *_prt);
            virtual void affect(ParticleSystem &_system, float _timeElapsed);
        };


        class ParticleSpriteAgeAffector : public ParticleAffector
        {
        public:

        protected:
            struct SpriteInfo{ short spriteIndex; std::string spriteName;};
            typedef std::map<float,SpriteInfo> SpriteTimeMap;
            SpriteTimeMap spriteChangeMap;

        public:

            ParticleSpriteAgeAffector(ParticleSystem *_parent);
            void setSprite(float _timeLeft, const std::string &_spriteName);
            virtual void initParticle(Particle *_prt);
            virtual void affect(ParticleSystem &_system, float _timeElapsed);

        };

    }
}
