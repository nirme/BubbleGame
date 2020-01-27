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



namespace core
{
    class SoundPlayer;

    class SoundSystem
    {
        friend class SoundPlayer;
    protected:
    public:

        SLObjectItf slEngineObject;
        SLEngineItf slEngine;
        SLObjectItf slOutputMixObject;

        SLDataLocator_OutputMix outputLocatorStruct;
        SLDataSink outputSinkStruct;


        static constexpr std::array<SLEngineOption,4> EngineOptionsArr {{
            (SLuint32) SL_ENGINEOPTION_THREADSAFE,
            (SLuint32) SL_BOOLEAN_TRUE,
            (SLuint32) SL_ENGINEOPTION_LOSSOFCONTROL,
            (SLuint32) SL_BOOLEAN_FALSE,
        }};

        static std::array<SLInterfaceID,1> EngineInterfacesArr;
        static std::array<SLInterfaceID,1> OutputInterfaces;



        //typedef std::vector<SoundPlayer> SoundPlayerList;
        //SoundPlayerList activePlayerList;


        void createSoundPlayer(SoundPlayer *_player);


    public:

        SoundSystem();

        void initialize();
        void uninitialize();
        void runTest(SoundPtr _sound);

/*
        static void BufferQueueCallback(SLAndroidSimpleBufferQueueItf caller, void *pContext)
        {
            SoundPlayer *player = reinterpret_cast<SoundPlayer *>(pContext);
            SLAndroidSimpleBufferQueueState state;
            (*caller)->GetState(caller, &state);

            SLuint32 playState;
            (*player->slPlay)->GetPlayState(player->slPlay, &playState);

            Logger::getSingleton().write(
                    std::string("Buffer count: ") +
                    std::to_string(state.count) +
                    std::string(" index: ") +
                    std::to_string(state.index) +
                    std::string(" play state: ") +
                    (playState == SL_PLAYSTATE_STOPPED ? "SL_PLAYSTATE_STOPPED" :
                     playState == SL_PLAYSTATE_PAUSED ? "SL_PLAYSTATE_PAUSED " :
                     "SL_PLAYSTATE_PLAYING")
            );
        };
*/
    };

}
