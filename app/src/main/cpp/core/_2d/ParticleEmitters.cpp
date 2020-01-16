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

            _prt->position.x = position.x + minPosition.x + (maxPosition.x - minPosition.x) * (((seed >> 1) & Mask ) * Div);
            _prt->position.y = position.y + minPosition.y + (maxPosition.y - minPosition.y) * (((seed >> 2) & Mask ) * Div);

            _prt->rotation = minRotation + (maxRotation - minRotation) * (((seed >> 3) & Mask ) * Div);
            _prt->size = minSize+ (maxSize - minSize) * (((seed >> 4) & Mask ) * Div);

            _prt->alpha = 1.0f;

            _prt->direction = rotateVector(
                    Vector2(
                            minDirectionSpeed + (maxDirectionSpeed - minDirectionSpeed) * (((seed >> 5) & Mask ) * Div),
                            0.0f
                    ),
                    minDirection + (maxDirection - minDirection) * (((seed >> 6) & Mask ) * Div)
            );

            _prt->rotationalSpeed = minRotationalSpeed + (maxRotationalSpeed - minRotationalSpeed) * (((seed >> 7) & Mask ) * Div);
            _prt->sizeChangeSpeed = minSizeChangeSpeed + (maxSizeChangeSpeed - minSizeChangeSpeed) * (((seed >> 8) & Mask ) * Div);

            _prt->totalAge = minTimeAlive + (maxTimeAlive - minTimeAlive) * (((seed >> 9) & Mask) * Div);
            _prt->ageLeft = _prt->totalAge;

            _prt->alive = true;
        };



        PointRandomParticleEmitter::PointRandomParticleEmitter(ParticleSystem *_parent) :
            ParticleEmitter(_parent, true, 0.0f)
        {};


        void PointRandomParticleEmitter::initParticle(Particle *_prt)
        {
            assert(_prt->type == PT_NORMAL && "Particle emitter must emit particles");

            // 0x7FFF guaranteed by rand, xor 2 to generate 30bit random value
            //unsigned int seed = std::rand() << 15 ^ std::rand();
            unsigned int seed = std::rand();
            seed ^= seed << 15;

            _prt->position = position;
            _prt->rotation = minRotation + (maxRotation - minRotation) * (((seed) & Mask ) * Div);
            _prt->size = minSize+ (maxSize - minSize) * (((seed >> 1) & Mask ) * Div);

            _prt->alpha = 1.0f;

            float directionT = minDirection + (maxDirection - minDirection) * (((seed >> 2) & Mask ) * Div);
            float linearSpeed = minDirectionSpeed + (maxDirectionSpeed - minDirectionSpeed) * (((seed >> 3) & Mask ) * Div);
            _prt->direction.x = std::sinf(directionT) * linearSpeed;
            _prt->direction.y = std::cosf(directionT) * linearSpeed;

            _prt->rotationalSpeed = minRotationalSpeed + (maxRotationalSpeed - minRotationalSpeed) * (((seed >> 4) & Mask ) * Div);
            _prt->sizeChangeSpeed = minSizeChangeSpeed + (maxSizeChangeSpeed - minSizeChangeSpeed) * (((seed >> 5) & Mask ) * Div);

            _prt->totalAge = minTimeAlive + (maxTimeAlive - minTimeAlive) * (((seed >> 6) & Mask) * Div);
            _prt->ageLeft = _prt->totalAge;

            _prt->alive = true;
        };

    }
}
