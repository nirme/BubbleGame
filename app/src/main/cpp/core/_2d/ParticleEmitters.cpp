#include "ParticleEmitters.h"
#include "ParticleSystem.h"


namespace core
{
    namespace _2d
    {

        RandomParticleEmitter::RandomParticleEmitter(ParticleSystem *_parent) :
            ParticleEmitter(_parent, true, 0.0f)
        {};

        void RandomParticleEmitter::initParticle(Particle *_prt)
        {
            assert(_prt->type == PT_NORMAL && "Particle emitter must emit particles");

            // 0x7FFF guaranteed by rand, xor 2 to generate 30bit random value
            //unsigned int seed = std::rand() << 15 ^ std::rand();
            unsigned int seed = std::rand();
            seed ^= seed << 15;

            _prt->position.x = position.x + minPosition.x + (maxPosition.x - minPosition.x) * ((seed & 0xFF ) * Div);
            seed >>= 1;
            _prt->position.y = position.y + minPosition.y + (maxPosition.y - minPosition.y) * ((seed & 0xFF ) * Div);
            seed >>= 1;

            _prt->rotation = minRotation + (maxRotation - minRotation) * ((seed & 0xFF ) * Div);
            seed >>= 1;

            _prt->size = minSize+ (maxSize - minSize) * ((seed & 0xFF ) * Div);
            seed >>= 1;

            _prt->alpha = 1.0f;

            _prt->direction.x = minDirection.x + (maxDirection.x - minDirection.x) * ((seed & 0xFF ) * Div);
            seed >>= 1;

            _prt->direction.y = minDirection.y + (maxDirection.y - minDirection.y) * ((seed & 0xFF ) * Div);
            seed >>= 1;

            _prt->rotationalSpeed = minRotationalSpeed + (maxRotationalSpeed - minRotationalSpeed) * ((seed & 0xFF ) * Div);
            seed >>= 1;

            _prt->sizeChangeSpeed = minSizeChangeSpeed + (maxSizeChangeSpeed - minSizeChangeSpeed) * ((seed & 0xFF ) * Div);
            seed >>= 1;

            _prt->totalAge = minTimeAlive + (maxTimeAlive - minTimeAlive) * ((seed & 0xFF ) * Div);
            _prt->ageLeft = _prt->totalAge;

            _prt->alive = true;
        };

    }
}
