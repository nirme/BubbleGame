#pragma once

#include <memory>
#include <string>
#include <list>
#include <array>
#include <vector>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "Exceptions.h"
#include "SoundSystem.h"


namespace core
{
    //class SoundSystem;
/*
    class SoundPlayer
    {
    protected:
        static constexpr int BufferQueueSize = 3;
        static std::array<SLInterfaceID> Interfaces;
        static std::array<int8_t, 32> EmptyBuffer;

        // shared input struct
        static constexpr SLDataLocator_BufferQueue InputLocatorStruct = {
            SL_DATALOCATOR_BUFFERQUEUE,
            BufferQueueSize
        };

        static constexpr SLDataFormat_PCM InputFormatStruct = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            16,  // packing of samples, best same as BPS
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
        };

        // cannot be const due to api restriction
        static SLDataSource InputSourceStruct = {
            (void *) &SoundPlayer::InputLocatorStruct,
            (void *) &SoundPlayer::InputFormatStruct
        };

        static constexpr std::array<std::pair<const SLchar *, SLint32>> AndroidInputConfiguration = {
            {
                SL_ANDROID_KEY_STREAM_TYPE,
                SL_ANDROID_STREAM_MEDIA
            },
            {
                SL_ANDROID_KEY_PERFORMANCE_MODE,
                //SL_ANDROID_PERFORMANCE_LATENCY
                SL_ANDROID_PERFORMANCE_LATENCY_EFFECTS
            }
        };


        SoundSystem *soundSystem;

        SLObjectItf slPlayerObject;
        SLPlayItf slPlay;
        //SLBufferQueueItf slBufferQueue;
        SLAndroidSimpleBufferQueueItf slBufferQueue;
        SLMuteSoloItf slMuteSolo;
        SLPrefetchStatusItf slPrefetchStatus;
        SLVolumeItf slVolume;

        typedef std::list<SoundPtr> SoundQueue;
        SoundQueue soundQueue;
        struct PlayedSong
        {
            bool buffered;
            SoundPtr sound;
        };


    public:

        void initialize()
        {
            assert(soundSystem && "Sound system cannot be NULL");

            std::vector<SLboolean> interfacesRequired(Interfaces.size(), SL_BOOLEAN_TRUE);

            SL_ERROR_CHECK((*soundSystem->slEngine)->CreateAudioPlayer(
                    soundSystem->slEngine,
                    &slPlayerObject,
                    &InputSourceStruct,
                    &soundSystem->outputSinkStruct,
                    Interfaces.size(),
                    Interfaces.data(),
                    interfacesRequired.data()
            ));


            SLAndroidConfigurationItf slAndroidConfigurationItf;

            SL_ERROR_CHECK((*slPlayerObject)->GetInterface(
                    slPlayerObject,
                    SL_IID_ANDROIDCONFIGURATION,
                    &slAndroidConfigurationItf
            );

            for (unsigned int i = 0; i < AndroidInputConfiguration.size(); ++i)
            {
                SL_ERROR_CHECK((*slAndroidConfigurationItf)->SetConfiguration(
                        slAndroidConfigurationItf,
                        AndroidInputConfiguration[i].first,
                        &AndroidInputConfiguration[i].second,
                        sizeof(SLint32)
                ));
            }

            SL_ERROR_CHECK((*slPlayerObject)->Realize(
                    slPlayerObject,
                    SL_BOOLEAN_FALSE
            ));


            SL_ERROR_CHECK((*slPlayerObject)->GetInterface(
                    slPlayerObject,
                    SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                    (void *) &slBufferQueue
            ));

            SL_ERROR_CHECK((*slPlayerObject)->GetInterface(
                    slPlayerObject,
                    SL_IID_PLAY,
                    (void *) &slPlay
            ));

            SL_ERROR_CHECK((*slPlayerObject)->GetInterface(
                    slPlayerObject,
                    SL_IID_MUTESOLO,
                    (void *) &slMuteSolo
            ));

        };

        std::array<SLInterfaceID, 4> SoundPlayer::PlayersInterfaces = {
                //SL_IID_BUFFERQUEUE,
                SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                SL_IID_MUTESOLO,
                SL_IID_VOLUME,
                SL_IID_ANDROIDCONFIGURATION,
        };

    }
*/
}
