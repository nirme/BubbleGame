#pragma once

#include <memory>
#include <string>
#include <list>
#include <array>
#include <vector>
//#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "Exceptions.h"
#include "Sound.h"


namespace core
{
    class SoundSystem;

    static float vol = 0.0f;

	enum PLAYBACK_STATE
	{
		PS_STOPPED = 0x00,
		PS_PLAYING = 0x01,
		PS_PAUSED = 0x02,
	};

	class SoundPlayer
    {
    public:
		class Listener
		{
		public:
			virtual void onSoundEnd() = 0;
			virtual ~Listener() {};
		};


	protected:

		static constexpr int BufferQueueSize = 3; // 3 buffers, 1 playing, 1 ready to play next part, 1 empty to load nex buffer

		static constexpr SLmillibel MinimumVolume = SL_MILLIBEL_MIN * 0.2; //

        // shared input struct
        static constexpr SLDataLocator_BufferQueue InputLocatorStruct = {
            SL_DATALOCATOR_BUFFERQUEUE,
            BufferQueueSize
        };

        // cannot be const due to api restriction
        static SLDataSource InputSourceStruct;
        static std::array<SLInterfaceID, 4> PlayersInterfaces;
        //static std::array<std::pair<const SLchar *, SLint32>, 2> AndroidInputConfiguration;
        static std::array<std::pair<const SLchar *, SLint32>, 1> AndroidInputConfiguration;



        SoundSystem *soundSystem;

        SLObjectItf slPlayerObject;

        SLAndroidConfigurationItf slAndroidConfig;

        SLPlayItf slPlay;
        SLAndroidSimpleBufferQueueItf slBufferQueue;
        SLMuteSoloItf slMuteSolo;
        SLVolumeItf slVolume;


        bool reserved;

		SLmillibel maxVolume;
		float currentVolume;


		Sound::SoundBufferIteratorUPtr soundIterator;
		Listener *listener;

        typedef Sound::SoundBufferIterator::SampleBuffer SoundSampleBuffer;
        std::list<SoundSampleBuffer> bufferList;

		PLAYBACK_STATE playerState;


		void queuedBufferCallback(SLAndroidSimpleBufferQueueItf _caller);
		static void QueuedBufferCallback(SLAndroidSimpleBufferQueueItf _caller, void *_pContext);

	public:

		void freePlayer();

    	static void setInputFormatStruct(SLDataFormat_PCM *_format)
		{
			InputSourceStruct.pFormat = (void*) _format;
		};


		SoundPlayer(SoundSystem *_soundSystem = nullptr);
		SoundPlayer(const SoundPlayer &_rhs);
		~SoundPlayer();

        bool initialize(SoundSystem *_soundSystem = nullptr);
        void release();

		PLAYBACK_STATE getPlayerState();
		void playSound(SoundPtr _sound, Listener *_listener);

		void pause();
		void resume();
		void stop();

		void setVolume(float _volume)
		{
			_volume = std::clamp(_volume, 0.0f, 1.0f);

			SLmillibel newVolume = MinimumVolume + (SLmillibel)((maxVolume - MinimumVolume) * _volume);
			SL_ERROR_CHECK((*slVolume)->SetVolumeLevel(slVolume, newVolume));
			currentVolume = _volume;
		};

		void setListener(Listener *_listener)
		{
			listener = _listener;
		};

		bool isReserved()
		{
			return reserved;
		};

		void setReserved(bool _reserved)
		{
			reserved = _reserved;
		};
	};

}
