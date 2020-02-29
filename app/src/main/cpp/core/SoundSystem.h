#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <array>
#include <vector>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <media/NdkMediaCodec.h>

#include "SingletonTemplate.h"
#include "Exceptions.h"
#include "Logger.h"

#include "Sound.h"
#include "SoundPlayer.h"




namespace core
{
    class SoundPlayer;

	class SoundSystem : public Singleton<SoundSystem>
    {
        //friend class SoundPlayer;

    public:

    	typedef uint8_t SoundPriority;


		static constexpr SLDataFormat_PCM InputFormatStruct = {
            SL_DATAFORMAT_PCM, // format, PCM needed fo buffer queue
            2, // number of channels
            SL_SAMPLINGRATE_44_1, // 44.1 kHz sampling
            SL_PCMSAMPLEFORMAT_FIXED_16, // 16bit sample depth
            16,  // packing of samples, best same as BPS
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, // channels used
            SL_BYTEORDER_LITTLEENDIAN // little endian by default
        };


    protected:

        bool initialized;

        SLObjectItf slEngineObject;
        SLEngineItf slEngine;

        SLObjectItf slOutputMixObject;
		SLVolumeItf slOutputVolume;
		SLOutputMixItf slOutputMix;

        SLDataLocator_OutputMix outputLocatorStructure;
        SLDataSink outputSinkStructure;

		unsigned int activePlayersCount;

		static constexpr unsigned int MaxActivePlayers = 28; // max active SLObjectItf is 32, engine use up to 4 objects


		// default engine options (possibly ignored on android)
        static constexpr std::array<SLEngineOption,4> EngineOptionsArr {{
            (SLuint32) SL_ENGINEOPTION_THREADSAFE,
            (SLuint32) SL_BOOLEAN_TRUE,
            (SLuint32) SL_ENGINEOPTION_LOSSOFCONTROL,
            (SLuint32) SL_BOOLEAN_FALSE,
        }};

		static std::array<SLInterfaceID,1> EngineInterfacesArray;
		static std::array<SLInterfaceID,1> OutputInterfacesArray;


		//typedef std::vector<SoundPlayer> SoundPlayerList;
        //SoundPlayerList activePlayerList;


        typedef std::vector<SoundPlayer> SoundPlayerPool;
		SoundPlayerPool playerList;

		typedef std::list<SoundPlayer*> SoundPlayerList;
		SoundPlayerList freePlayers;

		typedef std::list<std::pair<SoundPriority, SoundPlayer*>> UsedSoundPlayersList;
		UsedSoundPlayersList usedPlayers;


		SoundPlayer* getPlayer(SoundPriority _priority);



	public:

		void createSoundPlayerObject(SLObjectItf *_slPlayerObject, SLDataSource *_sInputSourceStructure, SLuint32 _interfacesNumber, const SLInterfaceID *_interfaces);

		SoundSystem();

        void initialize(unsigned int _activePlayersCount = 2);
        void uninitialize();
        void playSound(SoundPriority _priority, SoundPtr _sound);
		void freePlayer(SoundPlayer *_player);


		int32_t getSystemSampleRate()
		{
			return InputFormatStruct.samplesPerSec / 1000;
		};

		int32_t getSystemBitRate()
		{
			return InputFormatStruct.bitsPerSample;
		};

		int32_t getSystemChannels()
		{
			return InputFormatStruct.numChannels;
		};

	};

}
