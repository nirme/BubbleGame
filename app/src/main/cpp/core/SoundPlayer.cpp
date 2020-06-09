#include "SoundPlayer.h"
#include "SoundSystem.h"


namespace core
{

	SLDataSource SoundPlayer::InputSourceStruct = {
			(void *) &SoundPlayer::InputLocatorStruct,
			nullptr
			//(void *) &SoundSystem::InputFormatStruct
	};


	std::array<SLInterfaceID, 4> SoundPlayer::PlayersInterfaces = {
			SL_IID_ANDROIDCONFIGURATION,
			SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
			SL_IID_MUTESOLO,
			SL_IID_VOLUME,
	};

/*
	performance not available on older platforms

	std::array<std::pair<const SLchar *, SLint32>, 2> SoundPlayer::AndroidInputConfiguration = {
			std::pair<const SLchar *, SLint32>({
					SL_ANDROID_KEY_STREAM_TYPE,
					SL_ANDROID_STREAM_MEDIA
			}),
			std::pair<const SLchar *, SLint32>({
					SL_ANDROID_KEY_PERFORMANCE_MODE,
					SL_ANDROID_PERFORMANCE_LATENCY_EFFECTS
			})
	};
*/
	std::array<std::pair<const SLchar *, SLint32>, 1> SoundPlayer::AndroidInputConfiguration = {
			std::pair<const SLchar *, SLint32>({
					SL_ANDROID_KEY_STREAM_TYPE,
					SL_ANDROID_STREAM_MEDIA
			})
	};


	void SoundPlayer::queuedBufferCallback(SLAndroidSimpleBufferQueueItf _caller)
	{
		bufferList.pop_back();

		if (soundIterator->next())
		{
			SoundSampleBuffer buff = **soundIterator;

			SL_ERROR_CHECK((*slBufferQueue)->Enqueue(slBufferQueue, buff.data, buff.size));
			bufferList.push_front(buff);
		}
		else if (!bufferList.size())
		{
			// copy listener, if player not reserved then listener will be freed
			Listener* tmpListener = listener;

			if (!reserved)
				freePlayer();
			else
				stop();

			if (tmpListener)
				tmpListener->onSoundEnd();
		}
	};


	void SoundPlayer::QueuedBufferCallback(SLAndroidSimpleBufferQueueItf _caller, void *_pContext)
	{
		reinterpret_cast<SoundPlayer*>(_pContext)->queuedBufferCallback(_caller);
	};


	void SoundPlayer::freePlayer()
	{
		assert(soundSystem && "sound player must be initialized");

		if (playerState != PS_STOPPED)
		{
			playerState = PS_STOPPED;
			SL_ERROR_CHECK((*slPlay)->SetPlayState(slPlay, SL_PLAYSTATE_STOPPED));
			SL_ERROR_CHECK((*slBufferQueue)->Clear(slBufferQueue));
		}

		bufferList.clear();
		reserved = false;
		listener = nullptr;
		soundSystem->freePlayer(this);
	};


	SoundPlayer::SoundPlayer(SoundSystem *_soundSystem) :
			soundSystem(_soundSystem),
			slPlayerObject(nullptr),
			slAndroidConfig(nullptr),
			slPlay(nullptr),
			slBufferQueue(nullptr),
			slMuteSolo(nullptr),
			slVolume(nullptr),
			soundIterator(nullptr),
			playerState(PS_STOPPED)
	{};


	SoundPlayer::SoundPlayer(const SoundPlayer &_rhs) :
			soundSystem(_rhs.soundSystem),
			slPlayerObject(nullptr),
			slAndroidConfig(_rhs.slAndroidConfig),
			slPlay(nullptr),
			slBufferQueue(nullptr),
			slMuteSolo(nullptr),
			slVolume(nullptr),
			soundIterator(nullptr),
			playerState(PS_STOPPED)
	{};


	SoundPlayer::~SoundPlayer()
	{
		if (slPlayerObject)
			release();
	};


	bool SoundPlayer::initialize(SoundSystem *_soundSystem)
	{
		if (_soundSystem)
			soundSystem = _soundSystem;

		assert(soundSystem && "Sound system cannot be NULL");
		assert(!slPlayerObject && "Object already initialized");

		try
		{
			soundSystem->createSoundPlayerObject(&slPlayerObject, &InputSourceStruct, PlayersInterfaces.size() , PlayersInterfaces.data());

			SL_ERROR_CHECK((*slPlayerObject)->GetInterface(
					slPlayerObject,
					SL_IID_ANDROIDCONFIGURATION,
					&slAndroidConfig
			));

			for (unsigned int i = 0; i < AndroidInputConfiguration.size(); ++i)
			{
				SL_ERROR_CHECK((*slAndroidConfig)->SetConfiguration(
						slAndroidConfig,
						AndroidInputConfiguration[i].first,
						&AndroidInputConfiguration[i].second,
						sizeof(AndroidInputConfiguration[i].second)
				));
			}

			SL_ERROR_CHECK((*slPlayerObject)->Realize(
					slPlayerObject,
					SL_BOOLEAN_FALSE
			));

			// android buffer
			SL_ERROR_CHECK((*slPlayerObject)->GetInterface(
					slPlayerObject,
					SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
					(void *) &slBufferQueue
			));

			// play
			SL_ERROR_CHECK((*slPlayerObject)->GetInterface(
					slPlayerObject,
					SL_IID_PLAY,
					(void *) &slPlay
			));

			// mute
			SL_ERROR_CHECK((*slPlayerObject)->GetInterface(
					slPlayerObject,
					SL_IID_MUTESOLO,
					(void *) &slMuteSolo
			));

			// volume
			SL_ERROR_CHECK((*slPlayerObject)->GetInterface(
					slPlayerObject,
					SL_IID_VOLUME,
					(void *) &slVolume
			));

			SL_ERROR_CHECK((*slBufferQueue)->RegisterCallback(slBufferQueue, QueuedBufferCallback, this));


			SL_ERROR_CHECK((*slVolume)->GetMaxVolumeLevel(slVolume, &maxVolume));
			SL_ERROR_CHECK((*slVolume)->SetVolumeLevel(slVolume, maxVolume));
			currentVolume = 1.0f;


			soundIterator = nullptr;
			bufferList.clear();

		}
		catch(const std::exception &e)
		{
			if (slPlayerObject)
			{
				(*slPlayerObject)->Destroy(slPlayerObject);
				slPlayerObject = nullptr;
				slAndroidConfig = nullptr;
				slPlay = nullptr;
				slBufferQueue = nullptr;
				slMuteSolo = nullptr;
				slVolume = nullptr;
			}

			throw;
		}

		return slPlayerObject ? true : false;
	};


	void SoundPlayer::release()
	{
		if (slPlayerObject)
			(*slPlayerObject)->Destroy(slPlayerObject);

		slPlayerObject = nullptr;
		slAndroidConfig = nullptr;
		slPlay = nullptr;
		slBufferQueue = nullptr;
		slMuteSolo = nullptr;
		slVolume = nullptr;
	};


	PLAYBACK_STATE SoundPlayer::getPlayerState()
	{
		return playerState;
	};


	void SoundPlayer::playSound(SoundPtr _sound, Listener *_listener)
	{
		soundIterator = _sound->getDataIterator();

		if (playerState == PS_PLAYING)
		{
			SL_ERROR_CHECK((*slPlay)->SetPlayState(slPlay, SL_PLAYSTATE_STOPPED));
			SL_ERROR_CHECK((*slBufferQueue)->Clear(slBufferQueue));
			soundIterator = nullptr;
			bufferList.clear();
			playerState = PS_STOPPED;
		}

		// register listener
		listener = _listener;

		// enqueue first buffer and start playback
		SoundSampleBuffer buff = **soundIterator;
		SL_ERROR_CHECK((*slBufferQueue)->Enqueue(slBufferQueue, buff.data, buff.size));
		SL_ERROR_CHECK((*slPlay)->SetPlayState(slPlay, SL_PLAYSTATE_PLAYING));
		playerState = PS_PLAYING;
		bufferList.push_front(buff);

		// enqueue next part if it exist
		if (soundIterator->next())
		{
			buff = **soundIterator;
			SL_ERROR_CHECK((*slBufferQueue)->Enqueue(slBufferQueue, buff.data, buff.size));
			bufferList.push_front(buff);
		}

	};


	void SoundPlayer::pause()
	{
		if (playerState == PS_PLAYING)
		{
			SL_ERROR_CHECK((*slPlay)->SetPlayState(slPlay, SL_PLAYSTATE_PAUSED));
			playerState = PS_PAUSED;
		}
	};


	void SoundPlayer::resume()
	{
		if (playerState == PS_PAUSED)
		{
			SL_ERROR_CHECK((*slPlay)->SetPlayState(slPlay, SL_PLAYSTATE_PLAYING));
			playerState = PS_PLAYING;
		}
	};


	void SoundPlayer::stop()
	{
		if (playerState != PS_STOPPED)
		{
			SL_ERROR_CHECK((*slPlay)->SetPlayState(slPlay, SL_PLAYSTATE_STOPPED));
			SL_ERROR_CHECK((*slBufferQueue)->Clear(slBufferQueue));

			soundIterator.reset();
			playerState = PS_STOPPED;
		}
	};

}
