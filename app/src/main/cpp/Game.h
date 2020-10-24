#pragma once

#include "core/Engine.h"
#include "core/TouchControls.h"
#include "MusicController.h"

#include "core/_2d/shapes/Circle.h"
#include "core/_2d/shapes/LineArea.h"

/*

class Game1 : public core::Engine
{

public:

	Game(android_app* _androidApp) : Engine(_androidApp)
			{};

	void onStep()
	{
		Engine::onStep();


	};
};

*/





class Game : public core::Engine
{
protected:

	std::unique_ptr<MusicController> musicController;
	core::_2d::SpritedText* fpsText;

	float fps;
	float fpsTime;
	long int cc;


public:

	Game(android_app* _androidApp) : Engine(_androidApp)
	{};

	void onStep()
	{

		Engine::onStep();




/*
fpsTime += frameTime.getLastUpdateTime<float>();
++cc;

if (fpsTime >= 1.0f)
{
	fps = static_cast<int>(100.0f * cc / fpsTime) * 0.01f;
	cc = 0;
	fpsTime = 0.0f;

	fpsText->setText(std::string("FPS: ") + std::to_string(fps).substr(0, 5));

	core::_2d::Quaternion q = fpsText->getRotation();
	//q *= core::_2d::Quaternion(0.2f);
	//q.normalize();
	//q.rotate(core::degreeToRad(10.0f));
	//fpsText->setRotation(q);
}
*/
	};




	void onInitWindow()
	{
		Engine::onInitWindow();


		renderer->setBackgroundColor(0.5f, 0.5f, 0.5f);

		resourceManager->parseConfiguration("GameResourceCfg.xml");


		float sceneScale = 0.5f * (float)(renderer->BaseScreenDensity) / renderer->getScreenDensity();

		core::_2d::SceneManager* mainScene = createScene("mainScene", sceneScale);


		{
			core::_2d::SceneNode *node = mainScene->createNode("TestNode");
			mainScene->getRootNode()->appendChild(node);


			node->setPosition({ 0.0f, 0.0f });
			node->setRotation(0.0f);
			node->setScale({ 1.0f });

			core::_2d::SingleSprite *spriteTest = nullptr;
			spriteTest = mainScene->createSingleSprite("TestSprite", nullptr);
			spriteTest->setEnabled(false);

			core::ShadingProgramManager::getSingleton().getByName("baseShader")->load();

			core::ImageSpriteManager::getSingleton().getByName("font1_tex.bmp#Kidpixies#F")->load();
			spriteTest->setMaterial(core::ShadingProgramManager::getSingleton().getByName("baseShader"), core::ImageSpriteManager::getSingleton().getByName("font1_tex.bmp#Kidpixies#F"));

			spriteTest->setPosition({0.0f});

			spriteTest->setPriority(250);
			spriteTest->setRotation(0.0f);
			spriteTest->setScale({2.0f});
			spriteTest->setSpriteCoords(core::SpriteCoords::SPRITE_SQUARE);

			node->appendObject(spriteTest);

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



			core::_2d::SceneNode *node4 = mainScene->createNode("Text");
			mainScene->getRootNode()->appendChild(node4);

			node4->setPosition({ 14.0f, 8.0f });
			node4->setRotation(0.0f);
			node4->setScale({ 1.0f });

			fpsText = mainScene->createSpritedText("TestText", nullptr);
			fpsText->setMaterial(
				core::ShadingProgramManager::getSingleton().getByName("baseShader"),
				//core::SpritedFontManager::getSingleton().getByName("Kidpixies.xml")
				core::SpritedFontManager::getSingleton().getByName("Connection_II.xml")
				//core::SpritedFontManager::getSingleton().getByName("Lucida.xml")
			);

			fpsText->setPriority(255);
			fpsText->setAnchorPosition(core::_2d::SpritedText::AP_TOP_RIGHT);
			fpsText->setText("qwertyuiopasdfghjklzxcvbnm\n\nQWERTYUIOPASDFGHJKLZXCVBNM\n\n1234567890`[]\\;',./~!@#$%^\n\n&*()_+{}|:\"<>?", 24.0f);
			fpsText->setScale(1.0f);

			fpsText->setEnabled(true);
			node4->appendObject(fpsText);





			// Particle system test

			core::_2d::SceneNode *node2 = mainScene->createNode("ParticleSystemTest");
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
				core::TouchArea *buttonLeft = new core::TouchArea("left_button", btn1Shape);
				buttonLeft->registerListener(mList1);
				inputManager->registerControl("base_control", buttonLeft);

				// btn2
				EmissionListener *mList2 = new EmissionListener();
				mList2->object = emiter;
				mList2->emisionChange = 10;
				mList2->speedChange = 0.1f;

				core::_2d::Shape *btn2Shape = new core::_2d::Rectangle(1.0f, 2.2f, {0.6f, 0.0f}, 0.0f);
				core::TouchArea *buttonRight = new core::TouchArea("right_button", btn2Shape);
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









		musicController = std::make_unique<MusicController>(soundSystem.get());



		musicController->registerSound("menu", core::SoundManager::getSingleton().getByName("Mr_Tea.ogg"));
		musicController->registerSound("game", core::SoundManager::getSingleton().getByName("Ant_Fire.ogg"));
		musicController->registerSound("game", core::SoundManager::getSingleton().getByName("Ladybirds_Mate.ogg"));

		class TestListener : public core::TouchArea::Listener
		{
		public:
			MusicController *musicController;
			std::string set;

			void onPointerOnArea(){};
			void onPointerOffArea()
			{
				if (set.compare("menu") == 0)
					set = "game";
				else
					set = "menu";

				musicController->playSet(set);
			};
		};

		// btn1
		TestListener *mListT = new TestListener();
		mListT->musicController = musicController.get();
		mListT->set = "menu";
		musicController->playSet("menu");

		core::_2d::Shape *btn1Shape = new core::_2d::Rectangle(1.1f, 2.2f, {0.0f}, 0.0f);
		core::TouchArea *buttonCenter = new core::TouchArea("center_button", btn1Shape);
		buttonCenter->registerListener(mListT);
		inputManager->registerControl("base_control", buttonCenter);



		/*
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




		*/









		// physics test
		{
			class BallCollision : public core::_2d::PhysicsSystem::CollisionListener
			{
			public:
				virtual void onCollisionDetected(core::_2d::RigidObject *_object1, core::_2d::RigidObject *_object2)
				{
					//core::Logger::getSingleton().write(std::string("two balls overlapped: ") + _object1->getName() + " and " + _object2->getName());
				};

				virtual void onCollisionEnded(core::_2d::RigidObject *_object1, core::_2d::RigidObject *_object2)
				{
					//core::Logger::getSingleton().write(std::string("two balls stopped overlapping: ") + _object1->getName() + " and " + _object2->getName());
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

					/*
					if (dynamic_cast<core::_2d::SpritedBall*>(_object1))
						core::Logger::getSingleton().write(std::string("ball hit the wall: ") + _object1->getName());
					else
						core::Logger::getSingleton().write(std::string("ball hit the wall: ") + _object2->getName());
					*/
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

			core::_2d::SceneNode *node3 = mainScene->createNode("PhysicsSystemTest");
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
			for (unsigned int i = 0; i < 15; ++i)
			{
				core::_2d::SingleSprite *spritedBall = new core::_2d::SingleSprite(std::string("ball") + std::to_string(i), 255, nullptr, nullptr);
				spritedBall->setScale({1.6f});
				spritedBall->setPosition({-10.0f + i*4, 5.0f - (2.0f * (i/5))});

				core::ImageSpriteManager::getSingleton().getByName("objects.bmp#ball")->load();
				spritedBall->setMaterial(core::ShadingProgramManager::getSingleton().getByName("baseShader"), core::ImageSpriteManager::getSingleton().getByName("objects.bmp#ball"));

				mainScene->addObject(spritedBall);
				node3->appendObject(spritedBall);
				spritedBall->setEnabled(true);


				core::_2d::RigidObject *rigidObject = new core::_2d::RigidObject("ball", physicsSystem.get());
				rigidObject->setEntity(spritedBall);

				core::_2d::ShapePtr bb(new core::_2d::Circle({0.0f}, 0.5f));
				rigidObject->addShape(bb);
				rigidObject->setBounceFactor(1.0f);

				float direction = ((std::rand() % 1000) / 1000.0f) * (std::rand() % 2 ? -1 : 1);
				rigidObject->setDirectionVector({5.0f * direction, 0.0f});
				rigidObject->addAffector("gravity");
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




			core::_2d::SingleSprite *spritedWall = new core::_2d::SingleSprite("wall1", 255, nullptr, nullptr);
			spritedWall->setSpriteCoords({-6.0f, 6.0f, 0.0f, -0.5f});
			spritedWall->setPosition({-12.0f, 0.0f});
			spritedWall->setRotation({(90.0f/180.0f) * M_PI});
			spritedWall->setScale(2.0f);


			core::ImageSpriteManager::getSingleton().getByName("objects.bmp#wall")->load();
			spritedWall->setMaterial(core::ShadingProgramManager::getSingleton().getByName("baseShader"), core::ImageSpriteManager::getSingleton().getByName("objects.bmp#wall"));

			mainScene->addObject(spritedWall);
			node3->appendObject(spritedWall);
			spritedWall->setEnabled(true);


			core::_2d::RigidObject *rigidObject = new core::_2d::RigidObject("wall", physicsSystem.get());
			rigidObject->setEntity(spritedWall);

			core::_2d::ShapePtr w1(new core::_2d::LineArea({-1.0f, 0.0f}, {1.0f, 0.0f}));
			rigidObject->addShape(w1);
			rigidObject->setBounceFactor(1.0f);
			rigidObject->setStatic(true);






			spritedWall = new core::_2d::SingleSprite("wall2", 255, nullptr, nullptr);
			spritedWall->setSpriteCoords({-6.0f, 6.0f, 0.0f, -0.5f});
			spritedWall->setPosition({0.0f, -6.0f});
			spritedWall->setScale(2.0f);


			core::ImageSpriteManager::getSingleton().getByName("objects.bmp#wall")->load();
			spritedWall->setMaterial(core::ShadingProgramManager::getSingleton().getByName("baseShader"), core::ImageSpriteManager::getSingleton().getByName("objects.bmp#wall"));

			mainScene->addObject(spritedWall);
			node3->appendObject(spritedWall);
			spritedWall->setEnabled(true);


			rigidObject = new core::_2d::RigidObject("wall", physicsSystem.get());
			rigidObject->setEntity(spritedWall);

			core::_2d::ShapePtr w2(new core::_2d::LineArea({-1.0f, 0.0f}, {1.0f, 0.0f}));
			rigidObject->addShape(w2);
			rigidObject->setBounceFactor(1.0f);
			rigidObject->setStatic(true);








			spritedWall = new core::_2d::SingleSprite("wall3", 255, nullptr, nullptr);
			spritedWall->setSpriteCoords({-6.0f, 6.0f, 0.0f, -0.5f});
			spritedWall->setPosition({12.0f, 0.0f});
			spritedWall->setScale(2.0f);
			spritedWall->setRotation({(-90.0f/180.0f) * M_PI});


			core::ImageSpriteManager::getSingleton().getByName("objects.bmp#wall")->load();
			spritedWall->setMaterial(core::ShadingProgramManager::getSingleton().getByName("baseShader"), core::ImageSpriteManager::getSingleton().getByName("objects.bmp#wall"));

			mainScene->addObject(spritedWall);
			node3->appendObject(spritedWall);
			spritedWall->setEnabled(true);


			rigidObject = new core::_2d::RigidObject("wall", physicsSystem.get());
			rigidObject->setEntity(spritedWall);

			core::_2d::ShapePtr w3(new core::_2d::LineArea({-1.0f, 0.0f}, {1.0f, 0.0f}));
			rigidObject->addShape(w3);
			rigidObject->setBounceFactor(1.0f);
			rigidObject->setStatic(true);

		}



		frameTime.reset();
		initialized = true;
	};


};
