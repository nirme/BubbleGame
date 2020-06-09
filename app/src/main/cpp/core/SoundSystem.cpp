#include "SoundSystem.h"


template<>
core::SoundSystem* Singleton<core::SoundSystem>::impl = nullptr;

namespace core
{

	SLDataFormat_PCM SoundSystem::InputFormatStruct = {
			SL_DATAFORMAT_PCM, // format, PCM needed fo buffer queue
			0, // number of channels
			0, // 44.1 kHz sampling
			0, // 16bit sample depth
			16,  // packing of samples, best same as BPS
			SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, // channels used
			SL_BYTEORDER_LITTLEENDIAN // little endian by default
	};


	std::array<SLInterfaceID,1> SoundSystem::EngineInterfacesArray = {
			SL_IID_ENGINE
	};

	std::array<SLInterfaceID,1> SoundSystem::OutputInterfacesArray = {
			SL_IID_OUTPUTMIX
	};



	SoundPlayer* SoundSystem::getPlayer(SoundPriority _priority, bool _freePlayerOnSoundEnd)
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

			player->setReserved(!_freePlayerOnSoundEnd);
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


	void SoundSystem::initialize(SLuint32 _samplingRate, SLuint16 _samplingDepth, SLuint32 _channels, unsigned int _activePlayersCount)
	{
		assert(!initialized && "sound system is already initialized");

		InputFormatStruct.numChannels = _channels;
		InputFormatStruct.samplesPerSec = _samplingRate * 1000;
		InputFormatStruct.bitsPerSample = _samplingDepth;

		SoundPlayer::setInputFormatStruct(&InputFormatStruct);

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
			globalState = PS_PLAYING;
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


	SoundPlayer *SoundSystem::playSound(SoundPriority _priority, SoundPtr _sound, SoundPlayer::Listener *_listener)
	{
		if (globalState == PS_PLAYING)
		{
			SoundPlayer *player = getPlayer(_priority);
			if (player)
			{
				player->playSound(_sound, _listener);
			}

			return player;
		}

		return nullptr;
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


	int32_t SoundSystem::getSystemSampleRate()
	{
		return InputFormatStruct.samplesPerSec / 1000;
	};


	int32_t SoundSystem::getSystemBitRate()
	{
		return InputFormatStruct.bitsPerSample;
	};


	int32_t SoundSystem::getSystemChannels()
	{
		return InputFormatStruct.numChannels;
	};


	void SoundSystem::pause()
	{
		if (globalState == PS_PLAYING)
		{
			for (UsedSoundPlayersList::iterator it = usedPlayers.begin(), itEnd = usedPlayers.end(); it != itEnd; ++it)
				(*it).second->pause();

			globalState = PS_PAUSED;
		}
	};


	void SoundSystem::resume()
	{
		if (globalState == PS_PAUSED)
		{
			for (UsedSoundPlayersList::iterator it = usedPlayers.begin(), itEnd = usedPlayers.end(); it != itEnd; ++it)
				(*it).second->resume();

			globalState = PS_PLAYING;
		}
		else if (globalState == PS_STOPPED)
		{
			globalState = PS_PLAYING;
		}
	};


	void SoundSystem::stop()
	{
		if (globalState != PS_STOPPED)
		{
			globalState = PS_STOPPED;

			for (UsedSoundPlayersList::iterator it = usedPlayers.begin(), itEnd = usedPlayers.end(); it != itEnd; ++it)
			{
				(*it).second->stop();
				freePlayers.push_back((*it).second);
			}
			usedPlayers.clear();
		}
	};


}
