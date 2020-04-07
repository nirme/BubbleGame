#pragma once

#include "core/Engine.h"


class Game : public core::Engine
{
public:

	Game(android_app* _androidApp) : Engine(_androidApp)
	{};



	void onInitWindow()
	{
		Engine::onInitWindow();


		renderer->setBackgroundColor(0.5f, 0.5f, 0.5f);

		resourceManager->parseConfiguration("GameResourceCfg.xml");


		float sceneScale = 0.5f * (float)(renderer->BaseScreenDensity) / renderer->getScreenDensity();

		core::_2d::SceneManager* mainScene = createScene("mainScene", sceneScale);


		{
			core::_2d::SceneNode *node = mainScene->createNode("TestNode", nullptr);
			mainScene->getRootNode()->appendChild(node);

			/*
			node->setPosition({ 0.0f, 0.0f });
			node->setRotation(0.0f);
			node->setScale({ 1.0f });

			_2d::SingleSprite *spriteTest = nullptr;
			spriteTest = mainScene->createSingleSprite("TestSprite", nullptr);
			spriteTest->setEnabled(true);
			*/
			core::ShadingProgramManager::getSingleton().getByName("baseShader")->load();
			/*
			ImageSpriteManager::getSingleton().getByName("anim_1.bmp")->load();
			spriteTest->setMaterial(ShadingProgramManager::getSingleton().getByName("baseShader"), ImageSpriteManager::getSingleton().getByName("anim_1.bmp"));

			spriteTest->setPosition({0.0f});

			spriteTest->setPriority(127);
			spriteTest->setRotation(0.0f);
			spriteTest->setScale({1.0f});
			spriteTest->setSpriteCoords(SpriteCoords::SPRITE_SQUARE);

			node->appendObject(spriteTest);
			*/
			/*
			class MoveListener : public TouchArea::Listener
			{
			public:
				_2d::MovableObject *object;
				Vector2 pos;
				void onPointerOnArea()
				{
					object->setPosition(object->getPosition() + pos);
				};
				void onPointerOffArea()
				{
					object->setPosition(object->getPosition() + pos);
				};
			};

			// btn1
			MoveListener *mList1 = new MoveListener();
			mList1->object = spriteTest;
			mList1->pos.x = -0.3f;

			_2d::Shape *btn1Shape = new _2d::Rectangle(Vector2(-10.0f, 10.0f), Vector2(-0.6f, 10.0f), Vector2(-0.6f, -10.0f), Vector2(-10.0f, -10.0f));
			TouchArea *buttonLeft = new TouchArea(btn1Shape);
			buttonLeft->registerListener(mList1);

			inputManager->registerControl("base_control", buttonLeft);


			// btn2
			MoveListener *mList2 = new MoveListener();
			mList2->object = spriteTest;
			mList2->pos.x = 0.3f;

			_2d::Shape *btn2Shape = new _2d::Rectangle(Vector2(10.0f, 10.0f), Vector2(0.6f, 10.0f), Vector2(0.6f, -10.0f), Vector2(10.0f, -10.0f));
			TouchArea *buttonRight = new TouchArea(btn2Shape);
			buttonRight->registerListener(mList2);

			inputManager->registerControl("base_control", buttonRight);

			inputManager->activateControlSet("base_control");

			*/




			// Particle system test

			core::_2d::SceneNode *node2 = mainScene->createNode("ParticleSystemTest", nullptr);
			mainScene->getRootNode()->appendChild(node2);

			node2->setPosition({ -5.0f, 0.0f });
			node2->setRotation(0.0f);
			node2->setScale({ 1.0f });


			core::_2d::ParticleSystem* particleTest = mainScene->createParticleSystem("TestParticleSystem", nullptr);
			particleTest->setEnabled(true);
			node2->appendObject(particleTest);
			particleTest->setEnabled(true);

			particleTest->setMaterial(
					core::ShadingProgramManager::getSingleton().getByName("particleShader"),
					core::TextureManager::getSingleton().getByName("explosion.bmp")
			);


			{
				core::_2d::PointRandomParticleEmitter *emiter = new core::_2d::PointRandomParticleEmitter(particleTest);
				//_2d::RandomParticleEmitter *emiter = new _2d::RandomParticleEmitter(particleTest);
				//emiter->setEmissionPosition({-1.0f, -1.0f}, {1.0f, 1.0f});
				emiter->setEmissionPosition({-0.1f, -0.1f}, {0.1f, 0.1f});
				emiter->setEmissionRotation(0.0f, 2.0f * M_PI);
				emiter->setEmissionSize(0.75f, 1.5f);

				//emiter->setEmissionDirection(0.0f, M_PI * 2, 1.25f, 1.75f);
				emiter->setEmissionDirection(M_PI * 0.5f - 0.1f, M_PI * 0.5f + 0.1f);
				emiter->setEmissionLinearSpeed(1.5f, 2.5f);
				emiter->setEmissionRotationalSpeed(0.0f, 0.3f);
				emiter->setEmissionSizeChangeSpeed(0.0f, 0.3f);

				emiter->setEmissionTimeAlive(2.0f, 3.5f);
				emiter->setEmissionRate(60.0f);

				particleTest->addEmiter(emiter);



				class EmissionListener : public core::TouchArea::Listener
				{
				public:
					core::_2d::ParticleEmitter *object;
					int emisionChange;
					float speedChange;
					void onPointerOnArea()
					{
						object->setEmissionRate(object->getEmissionRate() + emisionChange);
						object->setEmissionLinearSpeed(object->getEmissionMinLinearSpeed() + speedChange,
													   object->getEmissionMaxLinearSpeed() + speedChange
						);
						core::Logger::getSingleton().write(std::to_string(object->getEmissionRate()) );
					};
					void onPointerOffArea()
					{};
				};

				// btn1
				EmissionListener *mList1 = new EmissionListener();
				mList1->object = emiter;
				mList1->emisionChange = -10;
				mList1->speedChange = -0.1f;

				core::_2d::Shape *btn1Shape = new core::_2d::Rectangle(1.0f, 2.2f, {-0.6f, 0.0f}, 0.0f);
				core::TouchArea *buttonLeft = new core::TouchArea(btn1Shape);
				buttonLeft->registerListener(mList1);
				inputManager->registerControl("base_control", buttonLeft);

				// btn2
				EmissionListener *mList2 = new EmissionListener();
				mList2->object = emiter;
				mList2->emisionChange = 10;
				mList2->speedChange = 0.1f;

				core::_2d::Shape *btn2Shape = new core::_2d::Rectangle(1.0f, 2.2f, {0.6f, 0.0f}, 0.0f);
				core::TouchArea *buttonRight = new core::TouchArea(btn2Shape);
				buttonRight->registerListener(mList2);

				inputManager->registerControl("base_control", buttonRight);

				inputManager->activateControlSet("base_control");





			}

			core::_2d::ParticleSpriteAgeAffector *imgAffector = new core::_2d::ParticleSpriteAgeAffector(particleTest);
			particleTest->addAffector(imgAffector);

			core::_2d::ParticleSpeedAffector *speedAffector = new core::_2d::ParticleSpeedAffector(particleTest);
			speedAffector->setDirection({0.0f, -10.0f});
			speedAffector->setOperation(core::_2d::ParticleSpeedAffector::SMO_ADD);
			particleTest->addAffector(speedAffector);

			{
				std::string texName = "explosion.bmp#";
				for (unsigned int i = 0; i <= 40; ++i)
				{
					std::string spriteName = texName + std::to_string(i);
					particleTest->addSprite(core::ImageSpriteManager::getSingleton().getByName(spriteName));
					float time = 1.0f - (i / 41.0f);
					time = std::pow(time, 1.3f);
					imgAffector->setSprite(time,spriteName);
				}
			}

			particleTest->setPriority(0xFF);
			particleTest->setSpeedFactor(1.0f);
			particleTest->initSystem(10000);

			particleTest->setPosition({0.0f, 0.0f});
			particleTest->setRotation(0.0f);
			particleTest->setScale(1.0f);


		}

		class SoundRepeater : public core::SoundPlayer::Listener
		{
			core::SoundSystem *system;
			core::SoundPtr sound;

		public:

			SoundRepeater(core::SoundSystem *_system, core::SoundPtr _sound) :
					system(_system),
					sound(_sound)
			{};

			virtual void onSoundEnd()
			{
				system->playSound(255, sound, this);
			};
		};



		core::SoundPtr testSound = core::SoundManager::getSingleton().getByName("Mr_Tea.ogg");
		testSound->load();
		SoundRepeater *rep = new SoundRepeater(soundSystem.get(), testSound);
		core::SoundSystem::getSingleton().playSound(0, testSound, rep);

		//SoundPtr testSound = SoundManager::getSingleton().getByName("test.ogg");
		//testSound->load();
		//SoundRepeater *rep = new SoundRepeater(soundSystem.get(), testSound);
		//SoundSystem::getSingleton().playSound(0, testSound, rep);






		// physics test
		{
			class BallCollision : public core::_2d::PhysicsSystem::CollisionListener
			{
			public:
				virtual void onCollisionDetected(core::_2d::RigidObject *_object1, core::_2d::RigidObject *_object2)
				{
					core::Logger::getSingleton().write(std::string("two balls overlapped: ") + _object1->getName() + " and " + _object2->getName());
				};

				virtual void onCollisionEnded(core::_2d::RigidObject *_object1, core::_2d::RigidObject *_object2)
				{
					core::Logger::getSingleton().write(std::string("two balls stopped overlapping: ") + _object1->getName() + " and " + _object2->getName());
				};

				virtual ~BallCollision()
				{};
			};

			class WallCollision : public core::_2d::PhysicsSystem::CollisionListener
			{
			public:
				core::SoundPtr sound;

				virtual void onCollisionDetected(core::_2d::RigidObject *_object1, core::_2d::RigidObject *_object2)
				{
					core::SoundSystem::getSingleton().playSound(0, sound);

					if (dynamic_cast<core::_2d::SpritedBall*>(_object1))
						core::Logger::getSingleton().write(std::string("ball hit the wall: ") + _object1->getName());
					else
						core::Logger::getSingleton().write(std::string("ball hit the wall: ") + _object2->getName());
				};

				virtual void onCollisionEnded(core::_2d::RigidObject *_object1, core::_2d::RigidObject *_object2)
				{};

				virtual ~WallCollision()
				{};
			};

			WallCollision *cl = new WallCollision();
			cl->sound = core::SoundManager::getSingleton().getByName("bounce_3.ogg");
			cl->sound->load();
			physicsSystem->registerCollidablePair("ball", "wall", true, cl);

			//	physicsSystem->registerCollidablePair("ball", "wall", true, new WallCollision());
			physicsSystem->registerCollidablePair("ball", "ball", true, new BallCollision());

			core::_2d::PhysicsAffectorPtr gravity(new core::_2d::PhysicsLinearAffector());
			reinterpret_cast<core::_2d::PhysicsLinearAffector*>(gravity.get())->setDirection({0.0f, -10.0f});
			physicsSystem->registerAffector("gravity", gravity);

			core::_2d::SceneNode *node3 = mainScene->createNode("PhysicsSystemTest", nullptr);
			mainScene->getRootNode()->appendChild(node3);

			node3->setPosition({ 0.0f, 0.0f });
			//node3->setRotation({(20.0f/180.0f) * M_PI});
			node3->setScale({ 1.0f });





			//float rotation = 0;
			//Matrix3 mx = affine2DMatrix({1.0f}, {(float) (rotation/180.0f * M_PI)}, {0.0f});
			/*
			_2d::SpritedBall *spritedBall1 = new _2d::SpritedBall("ball", "ball", physicsSystem.get());
			spritedBall1->setScale({2.0f});
			spritedBall1->setPosition({0.0f, 6.0f});
			spritedBall1->setDirectionVector({0.0f, -0.4f});
			mainScene->addObject(spritedBall1);
			node3->appendObject(spritedBall1);
			spritedBall1->setEnabled(true);

			_2d::SpritedBall *spritedBall2 = new _2d::SpritedBall("ball", "ball", physicsSystem.get());
			spritedBall2->setScale({2.0f});
			spritedBall2->setPosition({0.0f, 3.5f});
			spritedBall2->setDirectionVector({0.0f, 0.0f});
			mainScene->addObject(spritedBall2);
			node3->appendObject(spritedBall2);
			spritedBall2->setEnabled(true);
			*/


			std::srand(std::time(nullptr));
			for (unsigned int i = 0; i < 5; ++i)
			{

				core::_2d::SpritedBall *spritedBall = new core::_2d::SpritedBall(std::string("ball") + std::to_string(i), "ball", physicsSystem.get());
				spritedBall->setScale({1.6f});
				spritedBall->setPosition({-10.0f + i*4, 6.0f-(i/2.0f)});
				float direction = ((std::rand() % 1000) / 1000.0f) * (std::rand() % 2 ? -1 : 1);
				spritedBall->setDirectionVector({5.0f * direction, 0.0f});
				spritedBall->addAffector("gravity");
				mainScene->addObject(spritedBall);
				node3->appendObject(spritedBall);
				spritedBall->setEnabled(true);
			}


			/*
			class TestListener : public TouchArea::Listener
			{
			public:
			float rotation;
			_2d::SpritedBall *b1, *b2;

			void onPointerOnArea()
			{};
			void onPointerOffArea()
			{
				rotation += 5.0f;
				Matrix3 mx = affine2DMatrix({1.0f}, {(float) (rotation/180.0f * M_PI)}, {0.0f});

				b1->setPosition(mx * Vector2(-3.0f, 0.0f));
				b1->setDirectionVector(transformVector(mx, Vector2(4.0f, 0.0f)));

				b2->setPosition(mx * Vector2(3.0f, 0.0f));
				b2->setDirectionVector(transformVector(mx, Vector2(-4.0f, 0.0f)));
			};
			};

			// btn1
			TestListener *mListT = new TestListener();
			mListT->rotation = 0.0f;
			mListT->b1 = spritedBall1;
			mListT->b2 = spritedBall2;

			_2d::Shape *btn1Shape = new _2d::Rectangle(1.1f, 2.2f, {0.0f}, 0.0f);
			TouchArea *buttonCenter = new TouchArea(btn1Shape);
			buttonCenter->registerListener(mListT);
			inputManager->registerControl("base_control", buttonCenter);

			*/



			core::_2d::SpritedWall* spritedWall1 = new core::_2d::SpritedWall("wall", "wall", physicsSystem.get());
			//spritedWall->setPosition({0.0f, -0.4f});
			spritedWall1->setPosition({-12.0f, 0.0f});
			spritedWall1->setRotation({(90.0f/180.0f) * M_PI});
			mainScene->addObject(spritedWall1);
			node3->appendObject(spritedWall1);
			spritedWall1->setEnabled(true);

			core::_2d::SpritedWall* spritedWall2 = new core::_2d::SpritedWall("wall", "wall", physicsSystem.get());
			//spritedWall->setPosition({0.0f, -0.4f});
			spritedWall2->setPosition({0.0f, -6.0f});
			//spritedWall2->setRotation({(-45.0f/180.0f) * M_PI});
			mainScene->addObject(spritedWall2);
			node3->appendObject(spritedWall2);
			spritedWall2->setEnabled(true);

			core::_2d::SpritedWall* spritedWall3 = new core::_2d::SpritedWall("wall", "wall", physicsSystem.get());
			//spritedWall->setPosition({0.0f, -0.4f});
			spritedWall3->setPosition({12.0f, 0.0f});
			spritedWall3->setRotation({(-90.0f/180.0f) * M_PI});
			mainScene->addObject(spritedWall3);
			node3->appendObject(spritedWall3);
			spritedWall3->setEnabled(true);
		}



		frameTime.reset();
		initialized = true;
	};


};
