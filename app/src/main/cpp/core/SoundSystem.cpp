#include "SoundSystem.h"


template<>
core::SoundSystem* Singleton<core::SoundSystem>::impl = nullptr;

namespace core
{

	std::array<SLInterfaceID,1> SoundSystem::EngineInterfacesArray = {
			SL_IID_ENGINE
	};
/*
	std::array<SLInterfaceID,2> SoundSystem::OutputInterfacesArray = {
			SL_IID_VOLUME,
			SL_IID_OUTPUTMIX
	};
*/
	std::array<SLInterfaceID,1> SoundSystem::OutputInterfacesArray = {
			SL_IID_OUTPUTMIX
	};



	SoundPlayer* SoundSystem::getPlayer(SoundPriority _priority)
	{
		SoundPlayer *player = nullptr;

		if (freePlayers.size())
		{
			player = freePlayers.front();
			freePlayers.pop_front();
		}
		else if (usedPlayers.size())
		{
			auto pair = usedPlayers.front();
			if (_priority > pair.first)
			{
				player = pair.second;
				usedPlayers.pop_front();

			}
		}

		if (player)
		{
			bool insertFlag = false;
			for (auto it = usedPlayers.begin(), itEnd = usedPlayers.end(); it != itEnd; ++it)
			{
				if ((*it).first > _priority)
				{
					usedPlayers.insert(it, {_priority, player});
					insertFlag = true;
					break;
				}
			}

			if (!insertFlag)
				usedPlayers.push_back({_priority, player});
		}

		return player;
	};


	void SoundSystem::createSoundPlayerObject(SLObjectItf *_slPlayerObject, SLDataSource *_sInputSourceStructure, SLuint32 _interfacesNumber, const SLInterfaceID *_interfaces)
	{
		std::vector<SLboolean> interfaceRequired(SL_BOOLEAN_TRUE, _interfacesNumber);

		SL_ERROR_CHECK((*slEngine)->CreateAudioPlayer(slEngine,
													  _slPlayerObject,
													  _sInputSourceStructure,
													  &outputSinkStructure,
													  _interfacesNumber,
													  _interfaces,
													  interfaceRequired.data()
		));
	};


	SoundSystem::SoundSystem() :
			initialized(false),
			slEngineObject(nullptr),
			slEngine(nullptr),
			slOutputMixObject(nullptr),
			slOutputVolume(nullptr),
			slOutputMix(nullptr),
			outputLocatorStructure({0, nullptr}),
			outputSinkStructure({nullptr, nullptr}),
			activePlayersCount(0)
	{};


	void SoundSystem::initialize(unsigned int _activePlayersCount)
	{
		try
		{
			std::vector<SLboolean> interfaceRequired(SL_BOOLEAN_TRUE, EngineInterfacesArray.size());

			// create engine and get its interfaces
			SL_ERROR_CHECK(slCreateEngine(&slEngineObject,
										  EngineOptionsArr.size() / 2,
										  EngineOptionsArr.data(),
										  EngineInterfacesArray.size(),
										  EngineInterfacesArray.data(),
										  interfaceRequired.data()
			));


			SL_ERROR_CHECK((*slEngineObject)->Realize(slEngineObject,
													  SL_BOOLEAN_FALSE
			));


			SL_ERROR_CHECK((*slEngineObject)->GetInterface(slEngineObject,
														   SL_IID_ENGINE,
														   (void *) &slEngine
			));


			interfaceRequired.resize(OutputInterfacesArray.size(), SL_BOOLEAN_TRUE);

			// create output mix and get its interfaces
			SL_ERROR_CHECK((*slEngine)->CreateOutputMix(slEngine,
														&slOutputMixObject,
														OutputInterfacesArray.size(),
														OutputInterfacesArray.data(),
														interfaceRequired.data()
			));

			SL_ERROR_CHECK((*slOutputMixObject)->Realize(slOutputMixObject,
														 SL_BOOLEAN_FALSE
			));

			SL_ERROR_CHECK((*slOutputMixObject)->GetInterface(slOutputMixObject,
														   SL_IID_OUTPUTMIX,
														   (void *) &slOutputMix
			));



			outputLocatorStructure.locatorType = SL_DATALOCATOR_OUTPUTMIX;
			outputLocatorStructure.outputMix = slOutputMixObject;

			outputSinkStructure.pLocator = (void *) &outputLocatorStructure;
			outputSinkStructure.pFormat = NULL;


			// create players

			activePlayersCount = _activePlayersCount < MaxActivePlayers ? _activePlayersCount : MaxActivePlayers;
			playerList.resize(activePlayersCount);

			for (unsigned int i = 0; i < activePlayersCount; ++i)
			{
				playerList[i].initialize(this);
				freePlayers.push_back(&playerList[i]);
			}

			initialized = true;
		}
		catch (const std::exception &e)
		{
			// init failed
			Logger::getSingleton().write(e.what(), LL_ERROR);

			if (playerList.size())
				playerList.clear();

			// clear all vars and release everything
			if (slOutputMixObject)
			{
				(*slOutputMixObject)->Destroy(slOutputMixObject);
				slOutputMixObject = nullptr;
				slOutputVolume = nullptr;
				slOutputMix = nullptr;
			}

			if (slEngineObject)
			{
				(*slEngineObject)->Destroy(slEngineObject);
				slEngineObject = nullptr;
				slEngine = nullptr;
			}

			memset(&outputLocatorStructure, 0, sizeof(outputLocatorStructure));
			memset(&outputSinkStructure, 0, sizeof(outputSinkStructure));

			activePlayersCount = 0;

			initialized = false;
		}

	};


	void SoundSystem::uninitialize()
	{
		initialized = false;

		activePlayersCount = 0;
		if (playerList.size())
			playerList.clear();

		(*slOutputMixObject)->Destroy(slOutputMixObject);
		slOutputMixObject = nullptr;
		slOutputVolume = nullptr;
		slOutputMix = nullptr;

		(*slEngineObject)->Destroy(slEngineObject);
		slEngineObject = nullptr;
		slEngine = nullptr;

		memset(&outputLocatorStructure, 0, sizeof(outputLocatorStructure));
		memset(&outputSinkStructure, 0, sizeof(outputSinkStructure));
	};


	void SoundSystem::playSound(SoundPriority _priority, SoundPtr _sound)
	{
		SoundPlayer *player = getPlayer(_priority);
		if (player)
		{
			player->playSound(_sound);
		}
	};


	void SoundSystem::freePlayer(SoundPlayer *_player)
	{
		for (auto it = usedPlayers.begin(), itEnd = usedPlayers.end(); it != itEnd; ++it)
		{
			if ((*it).second == _player)
			{
				usedPlayers.erase(it);
				freePlayers.push_back(_player);
				return;
			}
		}
	};

}
