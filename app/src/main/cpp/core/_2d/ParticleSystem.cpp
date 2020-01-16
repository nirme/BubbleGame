#include "ParticleSystem.h"


namespace core
{
	namespace _2d
	{
        AxisAlignedBox ParticleSystem::_boundingBoxImpl() const
        {
            AxisAlignedBox box;
            if (!aliveParticles.size())
                return box;

            Particle *prt = nullptr;
            float maxSize = 0;
            for (ParticleSystem::ParticleList::const_iterator it = aliveParticles.begin(), itEnd = aliveParticles.end();
                 it != itEnd;
                 ++it)
            {
                prt = (*it);
                if (maxSize < prt->size)
                    maxSize = prt->size;
                box.merge((*it)->position);
            }

            box.setMinimum(box.getMinimum() - maxSize);
            box.setMaximum(box.getMaximum() + maxSize);

            return box;
        };

        void ParticleSystem::_findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
        {
            _queue->addRenderable(this);
        };



        void ParticleSystem::convertParticleToWorldSpace(Particle *_prt)
        {
            const Matrix3 &transform = getWorldTransform();
            _prt->position = transform * _prt->position;
            _prt->direction = transformVector(transform, _prt->direction);
        };




        void ParticleSystem::initParticles(unsigned int _particleCount)
        {
            particlesPool.resize(_particleCount);
            aliveParticles.clear();
            deadParticles.clear();

            for (unsigned int i=0, iEnd = particlesPool.size(); i < iEnd; ++i)
            {
                //particlesPool[i].spriteIndex = i;
                deadParticles.push_back(&particlesPool[i]);
            }
        };


        void ParticleSystem::expireParticles(float _timeElapsed)
        {
            Particle *prt = nullptr;
            ParticleList::iterator itTmp;
            unsigned int particlesRemoved = 0;
            for (ParticleList::iterator it = aliveParticles.begin(), itEnd = aliveParticles.end(); it != itEnd; ++it)
            {
                prt = (*it);
                if (prt->ageLeft < _timeElapsed)
                {
                    ++particlesRemoved;
                    prt->alive = false;
                    if (prt->type == Particle::PT_NORMAL)
                    {
                        itTmp = it;
                        --itTmp;
                        deadParticles.splice(deadParticles.end(),aliveParticles,it);
                        it = itTmp;
                    }
                    else
                    {
                        throw std::logic_error("unimplemented");
                    }
                }
            }

            if (particlesRemoved)
                MovableObject::invalidateBoundingBox();
        };


        void ParticleSystem::updateParticles(float _timeElapsed)
        {
            Particle *prt = nullptr;
            for (ParticleList::iterator it = aliveParticles.begin(), itEnd = aliveParticles.end(); it != itEnd; ++it)
            {
                prt = (*it);
                prt->position += prt->direction * _timeElapsed;
                prt->size += prt->sizeChangeSpeed * _timeElapsed;
                prt->rotation += prt->rotationalSpeed * _timeElapsed;
                prt->ageLeft -= _timeElapsed;
            }
        };


        Particle *ParticleSystem::createParticle()
        {
            Particle *prt = deadParticles.front();
            prt->alive = true;
            aliveParticles.splice(aliveParticles.end(), deadParticles, deadParticles.begin());
            return prt;
        };


        void ParticleSystem::emitParticles(float _timeElapsed)
        {
            unsigned int particlesToEmit = 0;
            unsigned int particlesEmited = 0;
            unsigned int i = 0;
            float partialTime;
            float partialParticleTime;
            ParticleEmitter *prtEmit = nullptr;
            Particle *prt = nullptr;
            ParticleAffectorList::iterator itAffectEnd = activeAffectors.end();

            for (ParticleEmitterList::iterator it = activeEmitters.begin(), itEnd = activeEmitters.end(); it != itEnd; ++it, ++i)
            {
                particlesToEmit = (*it)->getEmissionCount(_timeElapsed);
            }

            // correct timer for available particles
            float particleRatio = particlesToEmit <= deadParticles.size() ? 1.0f : ((float)(deadParticles.size()) / particlesToEmit);

            for (ParticleEmitterList::iterator it = activeEmitters.begin(), itEnd = activeEmitters.end(); it != itEnd; ++it)
            {
                prtEmit = (*it);
                particlesToEmit = particleRatio * (*it)->getEmissionCount(_timeElapsed);
                partialTime = _timeElapsed / particlesToEmit;

                if (prtEmit->emitParticles())
                {
                    particlesEmited += particlesToEmit;
                    for (unsigned int i = 0; i < particlesToEmit; ++i)
                    {
                        prt = this->createParticle();
                        prtEmit->initParticle(prt);
                        convertParticleToWorldSpace(prt);


                        for (ParticleAffectorList::iterator itAff = activeAffectors.begin(); itAff != itAffectEnd; ++itAff)
                        {
                            (*itAff)->initParticle(prt);
                        }

                        partialParticleTime = partialTime * i;

                        prt->position += prt->direction * partialParticleTime;
                        prt->rotation += prt->rotationalSpeed * partialParticleTime;
                        prt->size += prt->sizeChangeSpeed * partialParticleTime;
                    }
                }
                else
                {
                    throw std::logic_error("unimplemented");
                }

                (*it)->addTimeToRemains(_timeElapsed);
            }

            if (particlesEmited)
                MovableObject::invalidateBoundingBox();
        };


        ParticleIterator ParticleSystem::getActiveParticleIterator()
        {
            ParticleIterator iter;
            iter.pos = aliveParticles.begin();
            iter.end = aliveParticles.end();
            return iter;
        };


        ParticleSystem::ParticleSystem(const std::string &_name, Priority _renderPriority, MaterialPtr _material) :
                MovableObject(_name),
                Renderable(_renderPriority, _material, false),
                speedFactor(1.0f)
        {};


        short ParticleSystem::getSpriteIndex(const std::string &_spriteName)
        {
            for (short i =0, iEnd = sprites.size(); i < iEnd; ++i)
            {
                if (_spriteName.compare(sprites[i]->getName()) == 0)
                    return i;
            }

            return -1;
        };


        void ParticleSystem::setSpeedFactor(float _speedFactor)
        {
            speedFactor = _speedFactor;
        };


        void ParticleSystem::addSprite(ImageSpritePtr _sprite)
        {
            assert(_sprite && "Sprite cannot be null!");
            _sprite->load();
            assert(_sprite->getTexture()->getHandle() == material->textures[0]->getHandle() && "Sprite texture dont match with system material");
            sprites.push_back(_sprite);
        };


        void ParticleSystem::addEmiter(ParticleEmitter *_emiter)
        {
            activeEmitters.push_back(_emiter);
        };


        void ParticleSystem::addAffector(ParticleAffector *_affector)
        {
            activeAffectors.push_back(_affector);
        };


        void ParticleSystem::initSystem(unsigned int _particleCount)
        {
            // cashe sprites
            cashedSpriteCoords.resize(sprites.size());
            for (unsigned int i = 0, iEnd = sprites.size(); i < iEnd; ++i)
                cashedSpriteCoords[i] = sprites[i]->getCoords();

            initParticles(_particleCount);
        };


        void ParticleSystem::update(float _timeElapsed)
        {
            _timeElapsed *= speedFactor;

            expireParticles(_timeElapsed);

            //ParticleAffectorList activeAffectors
            for (ParticleAffectorList::iterator it = activeAffectors.begin(), itEnd = activeAffectors.end(); it != itEnd; ++it)
            {
                (*it)->affect(*this, _timeElapsed);
            }

            updateParticles(_timeElapsed);
            emitParticles(_timeElapsed);
        };


        void ParticleSystem::setMaterial(ShadingProgramPtr _program, TexturePtr _tex)
        {
            _program->load();
            _tex->load();
            Renderable::setMaterial(_program, _tex);
        };


        const Matrix3& ParticleSystem::getTransform() const
        {
            return MovableObject::getWorldTransform();
        };


        void ParticleSystem::notifyAttached(SceneNode* _parent)
        {
            MovableObject::notifyAttached(_parent);

            if (parent && !controller)
            {
                std::shared_ptr<TTimeframeControllerValue<ParticleSystem>> controllerValue( new TTimeframeControllerValue<ParticleSystem>(this));
                controller = ControllerManager::getSingleton().createFrameTimeController(std::static_pointer_cast<ControllerValue>(controllerValue));
            }
        };


        BuffWriteResult ParticleSystem::writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite) const
        {
            unsigned int fromSprite = _fromSprite;
            ParticleList::const_iterator it = aliveParticles.begin();
            for (;fromSprite; ++it, --fromSprite);

            assert(_buffer.elementSize() == 4 && "Buffer element size must be 4 for float data");
            // temporary array to write particles

            ParticleData particleData[100];

            fromSprite = _fromSprite;
            unsigned int particlesWritten = 0;
            unsigned int maxParticles;
            Particle *prt = nullptr;
            Matrix3 particleTransform;
            float particleAge;
            while ( it != aliveParticles.end())
            {
                maxParticles = _buffer.getRemainingLength() / particleSize;
                if (maxParticles > 100)
                    maxParticles = 100;
                if (maxParticles > (aliveParticles.size() - fromSprite - particlesWritten))
                    maxParticles = (aliveParticles.size() - fromSprite - particlesWritten);

                unsigned int i = 0;
                for (; i < maxParticles; ++i, ++it)
                {
                    prt = (*it);

                    particleTransform = affine2DMatrix(prt->size, prt->rotation, prt->position );
                    particleAge = prt->ageLeft / prt->totalAge;

                    particleData[i].vertex[0].position = particleTransform * SpriteCoords::SPRITE_SQUARE.uvPoints[0];
                    particleData[i].vertex[0].texel = cashedSpriteCoords[prt->spriteIndex].uvPoints[0];
                    particleData[i].vertex[0].alpha = prt->alpha;
                    particleData[i].vertex[0].age = particleAge;

                    particleData[i].vertex[1].position = particleTransform * SpriteCoords::SPRITE_SQUARE.uvPoints[1];
                    particleData[i].vertex[1].texel = cashedSpriteCoords[prt->spriteIndex].uvPoints[1];
                    particleData[i].vertex[1].alpha = prt->alpha;
                    particleData[i].vertex[1].age = particleAge;

                    particleData[i].vertex[2].position = particleTransform * SpriteCoords::SPRITE_SQUARE.uvPoints[2];
                    particleData[i].vertex[2].texel = cashedSpriteCoords[prt->spriteIndex].uvPoints[2];
                    particleData[i].vertex[2].alpha = prt->alpha;
                    particleData[i].vertex[2].age = particleAge;

                    particleData[i].vertex[3].position = particleTransform * SpriteCoords::SPRITE_SQUARE.uvPoints[3];
                    particleData[i].vertex[3].texel = cashedSpriteCoords[prt->spriteIndex].uvPoints[3];
                    particleData[i].vertex[3].alpha = prt->alpha;
                    particleData[i].vertex[3].age = particleAge;

                }

                particlesWritten += i;

                _buffer.write(reinterpret_cast<float*>(particleData), maxParticles * particleSize);

                if (it == aliveParticles.end())
                    return BuffWriteResult({ particlesWritten + _fromSprite, true });

                if (_buffer.getRemainingLength() < particleSize)
                    return BuffWriteResult({ particlesWritten + _fromSprite, false });
            }

            return BuffWriteResult({ particlesWritten + _fromSprite, true });
        };

	}
}
