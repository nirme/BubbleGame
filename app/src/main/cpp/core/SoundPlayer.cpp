#include "SoundPlayer.h"
#include "SoundSystem.h"


namespace core
{

	SLDataSource SoundPlayer::InputSourceStruct = {
			(void *) &SoundPlayer::InputLocatorStruct,
			(void *) &SoundSystem::InputFormatStruct
	};


	std::array<SLInterfaceID, 4> SoundPlayer::PlayersInterfaces = {
			SL_IID_ANDROIDCONFIGURATION,
			SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
			SL_IID_MUTESOLO,
			SL_IID_VOLUME,
	};

/*
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

}
