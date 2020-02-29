#pragma once

#include <memory>
#include <string>
#include <list>
#include <array>
#include <vector>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "Exceptions.h"
#include "Sound.h"


namespace core
{
    class SoundSystem;

    class SoundPlayer
    {
    protected:

		enum PLAYBACK_STATE
		{
			PS_STOPPED = 0x00,
			PS_PLAYING = 0x01,
			PS_PAUSED = 0x02,
		};


		static constexpr int BufferQueueSize = 3; // 3 buffers, 1 playing, 1 ready to play next part, 1 empty to load nex buffer

        // shared input struct
        static constexpr SLDataLocator_BufferQueue InputLocatorStruct = {
            SL_DATALOCATOR_BUFFERQUEUE,
            BufferQueueSize
        };

        // cannot be const due to api restriction
        static SLDataSource InputSourceStruct;
        static std::array<SLInterfaceID, 4> PlayersInterfaces;
        static std::array<std::pair<const SLchar *, SLint32>, 1> AndroidInputConfiguration;



        SoundSystem *soundSystem;

        SLObjectItf slPlayerObject;

        SLAndroidConfigurationItf slAndroidConfig;

        SLPlayItf slPlay;
        SLAndroidSimpleBufferQueueItf slBufferQueue;
        SLMuteSoloItf slMuteSolo;
        SLVolumeItf slVolume;


        Sound::SoundBufferIteratorUPtr soundIterator;

        typedef Sound::SoundBufferIterator::SampleBuffer SoundSampleBuffer;
        std::list<SoundSampleBuffer> bufferList;

		PLAYBACK_STATE playerState;



		void queuedBufferCallback(SLAndroidSimpleBufferQueueItf _caller)
		{
			Logger::getSingleton().write("queuedBufferCallback called");
			SLAndroidSimpleBufferQueueState queueState;
			(*slBufferQueue)->GetState(slBufferQueue, &queueState);

			Logger::getSingleton().write(std::string("Queue state, count: ") + std::to_string(queueState.count) + std::string(", index: ") + std::to_string(queueState.index));

			SLuint32 playState;
			(*slPlay)->GetPlayState(slPlay, &playState);

			Logger::getSingleton().write(std::string("Play state: ") +
				(playState == SL_PLAYSTATE_STOPPED ? "SL_PLAYSTATE_STOPPED" : (playState == SL_PLAYSTATE_PAUSED ? "SL_PLAYSTATE_PAUSED" : "SL_PLAYSTATE_PLAYING")));

		};


		static void QueuedBufferCallback(SLAndroidSimpleBufferQueueItf _caller, void *_pContext)
		{
			//reinterpret_cast<SoundPlayer*>(_pContext)
			reinterpret_cast<SoundPlayer*>(_pContext)->queuedBufferCallback(_caller);
		};


	public:

		SoundPlayer(SoundSystem *_soundSystem = nullptr);
		SoundPlayer(const SoundPlayer &_rhs);
		~SoundPlayer();

        bool initialize(SoundSystem *_soundSystem = nullptr);
        void release();

		PLAYBACK_STATE getPlayerState()
		{
			return playerState;
		};

		void playSound(SoundPtr _sound)
		{
			soundIterator = _sound->getDataIterator();

			if (playerState == PS_PLAYING)
			{
				(*slPlay)->SetPlayState(slPlay, SL_PLAYSTATE_STOPPED);
				//(*slPlayerObject)->Realize(slPlayerObject, SL_BOOLEAN_FALSE);
				(*slBufferQueue)->Clear(slBufferQueue);
				soundIterator = nullptr;
				bufferList.clear();
			}

			// enqueue first buffer and start playback
			SoundSampleBuffer buff = **soundIterator;
			bufferList.push_front(buff);

			(*slBufferQueue)->Enqueue(slBufferQueue, buff.data, buff.size);
			(*slPlay)->SetPlayState(slPlay, SL_PLAYSTATE_PLAYING);

			// enqueue next part if it exist
			if (soundIterator->next())
			{
				buff = **soundIterator;
				bufferList.push_front(buff);
				(*slBufferQueue)->Enqueue(slBufferQueue, buff.data, buff.size);

				// get the next iterator ready
				soundIterator->next();
			}

		};

    };

}
