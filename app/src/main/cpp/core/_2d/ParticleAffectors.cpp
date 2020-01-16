#include "ParticleAffectors.h"
#include "ParticleSystem.h"


namespace core
{
    namespace _2d
    {

        ParticleSpeedAffector::ParticleSpeedAffector(ParticleSystem *_parent,
                              SPEED_MODIFIER_OPERATION _operation,
                              Vector2 _direction) :
                ParticleAffector(_parent),
                operation(_operation),
                direction(_direction)

        {};

        void ParticleSpeedAffector::setOperation(SPEED_MODIFIER_OPERATION _operation)
        {
            operation = _operation;
        };

        void ParticleSpeedAffector::setDirection(Vector2 _direction)
        {
            direction = _direction;
        };

        Vector2 ParticleSpeedAffector::getDirection()
        {
            return direction;
        };


        void ParticleSpeedAffector::initParticle(Particle *_prt)
        {};

        void ParticleSpeedAffector::affect(ParticleSystem &_system, float _timeElapsed)
        {
            ParticleIterator iter = _system.getActiveParticleIterator();
            Particle *_prt = nullptr;

            if (operation == SMO_ADD)
            {
                Vector2 tmpDirection = direction * _timeElapsed;
                while (!iter.isEnd())
                {
                    //_prt = iter.next();
                    //_prt->direction += tmpDirection;
                    iter.next()->direction += tmpDirection;
                }
            }
            else if (operation == SMO_AVERAGE)
            {
                float currentWeight = (1.0f - _timeElapsed) * 0.5f;
                Vector2 tmpDirection = (direction * _timeElapsed) * 0.5f;
                while (!iter.isEnd())
                {
                    _prt = iter.next();
                    _prt->direction *= currentWeight;
                    _prt->direction += tmpDirection;
                }
            }
        };


        ParticleSpriteAgeAffector::ParticleSpriteAgeAffector(ParticleSystem *_parent) :
                ParticleAffector(_parent)
        {};

        void ParticleSpriteAgeAffector::setSprite(float _timeLeft, const std::string &_spriteName)
        {
            short index = parent->getSpriteIndex(_spriteName);
            assert(index != -1 && "Sprite index in affector not found in particle system");
            spriteChangeMap.insert({_timeLeft, {index, _spriteName}});
        };

        void ParticleSpriteAgeAffector::initParticle(Particle *_prt)
        {
            _prt->spriteIndex = (*spriteChangeMap.rbegin()).second.spriteIndex;
        };

        void ParticleSpriteAgeAffector::affect(ParticleSystem &_system, float _timeElapsed)
        {
            ParticleIterator iter = _system.getActiveParticleIterator();
            Particle *_prt = nullptr;

            while (!iter.isEnd())
            {
                _prt = iter.next();
                _prt->spriteIndex = (*spriteChangeMap.upper_bound((_prt->ageLeft - _timeElapsed)/_prt->totalAge)).second.spriteIndex;
            }
        };

    }
}
