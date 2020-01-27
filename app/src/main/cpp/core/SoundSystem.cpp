#include "SoundSystem.h"

namespace core
{

    std::array<SLInterfaceID,1> SoundSystem::EngineInterfacesArr = {
            SL_IID_3DCOMMIT,
    };

    std::array<SLInterfaceID,1> SoundSystem::OutputInterfaces = {
            SL_IID_VOLUME,
    };

/*
    void SoundSystem::createSoundPlayer(SoundPlayer *_player)
    {
        _player->inputLocatorStruct.locatorType = SL_DATALOCATOR_BUFFERQUEUE;
        _player->inputLocatorStruct.numBuffers = 6;

        //_player->inputFormatStruct.formatType = SL_DATAFORMAT_MIME;
        //_player->inputFormatStruct.mimeType = reinterpret_cast<SLchar*>(_player->playerMimeType);
        //_player->inputFormatStruct.containerType = SL_CONTAINERTYPE_MP3;

        memset(&_player->inputFormatStruct, 0, sizeof(_player->inputFormatStruct));
        _player->inputFormatStruct.formatType = SL_DATAFORMAT_PCM;
        _player->inputFormatStruct.numChannels = 2;
        _player->inputFormatStruct.samplesPerSec = SL_SAMPLINGRATE_44_1;
        _player->inputFormatStruct.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
        _player->inputFormatStruct.containerSize = 16;  // packing of samples, best same as BPS
        _player->inputFormatStruct.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
        _player->inputFormatStruct.endianness = SL_BYTEORDER_LITTLEENDIAN;


        _player->inputSourceStruct.pLocator = (void *) &_player->inputLocatorStruct;
        _player->inputSourceStruct.pFormat = (void *) &_player->inputFormatStruct;

        std::vector<SLboolean> interfacesRequired(_player->PlayersInterfaces.size(), SL_BOOLEAN_TRUE);

        SL_ERROR_CHECK((*slEngine)->CreateAudioPlayer(slEngine,
                                                      &_player->slPlayerObject,
                                                      &_player->inputSourceStruct,
                                                      &outputSinkStruct,
                                                      _player->PlayersInterfaces.size(),
                                                      _player->PlayersInterfaces.data(),
                                                      interfacesRequired.data()
        ));

        SL_ERROR_CHECK((*_player->slPlayerObject)->Realize(_player->slPlayerObject,
                                                           SL_BOOLEAN_FALSE
        ));

        SL_ERROR_CHECK((*_player->slPlayerObject)->GetInterface(_player->slPlayerObject,
                                                                SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                                                (void *) &_player->slBufferQueue
        ));

        SL_ERROR_CHECK((*_player->slPlayerObject)->GetInterface(_player->slPlayerObject,
                                                                SL_IID_PLAY,
                                                                (void *) &_player->slPlay
        ));

        SL_ERROR_CHECK((*_player->slPlayerObject)->GetInterface(_player->slPlayerObject,
                                                                SL_IID_MUTESOLO,
                                                                (void *) &_player->slMuteSolo
        ));

        SL_ERROR_CHECK((*_player->slPlayerObject)->GetInterface(_player->slPlayerObject,
                                                                SL_IID_VOLUME,
                                                                (void *) &_player->slVolume
        ));

        unsigned int sampleSize = _player->inputFormatStruct.numChannels * _player->inputFormatStruct.bitsPerSample;

        SL_ERROR_CHECK((*_player->slBufferQueue)->Enqueue(_player->slBufferQueue,
                                                          _player->EmptyBuffer.data(),
                                                          sampleSize < _player->EmptyBuffer.size() ?
                                                              sampleSize :
                                                             _player->EmptyBuffer.size()
        ));


        SL_ERROR_CHECK((*_player->slBufferQueue)->RegisterCallback(
            _player->slBufferQueue,
            BufferQueueCallback,
            _player
        ));


        SL_ERROR_CHECK((*_player->slPlay)->SetPlayState(_player->slPlay, SL_PLAYSTATE_PAUSED ));
    };
*/

    SoundSystem::SoundSystem() :
            slEngineObject(nullptr),
            slEngine(nullptr),
            slOutputMixObject(nullptr)
    {};


    void SoundSystem::initialize()
    {
        // all interfaces used in system are required so we use array full of SL_BOOLEAN_TRUE
        std::vector<SLboolean> interfacesRequired(10, SL_BOOLEAN_TRUE);
        /*
        = {
                SL_BOOLEAN_TRUE
        };
*/
        try
        {
            //slQueryNumSupportedEngineInterfaces
            //slQuerySupportedEngineInterfaces

            //QuerySupportedProfiles
            //SL_PROFILES_GAME

            /* Query available voices for 2D audio */
            //res = (*EngineCapabilitiesItf)->QueryAvailableVoices(
            //        EngineCapabilitiesItf, SL_VOICETYPE_2D_AUDIO, &numMax2DVoices,
            //        &isAbsoluteMax2D, &numFree2DVoices); CheckErr(res);


            /* Query number of vibra devices present in the system */
            /*
            res = (*EngineCapabilitiesItf)->QueryVibraCapabilities(
                    EngineCapabilitiesItf, &numVibraDevices, NULL, NULL); CheckErr(res);
            */

            /* 3D audio functionality is mandated only in the game profile, so
might want to query for 3D voice types only if GAME profile is supported
*/
            /*
            if (isGameProfileSupported) {
                res = (*EngineCapabilitiesItf)->QueryAvailableVoices(
                        EngineCapabilitiesItf, SL_VOICETYPE_3D_AUDIO, &numMax3DVoices,
                        &isAbsoluteMax3D, &numFree3DVoices); CheckErr(res);
                res = (*EngineCapabilitiesItf)->QueryAvailableVoices(
                        EngineCapabilitiesItf, SL_VOICETYPE_3D_MIDIOUTPUT, &numMax3DMidiVoices,
                        &isAbsoluteMax3DMidi, &numFree3DMidiVoices); CheckErr(res);
            }
            */


            // create engine and get its interfaces
            SL_ERROR_CHECK(slCreateEngine(&slEngineObject,
                                          EngineOptionsArr.size() / 2,
                                          EngineOptionsArr.data(),
                                          0,
                                          nullptr,
                                          nullptr
            ));

            SL_ERROR_CHECK((*slEngineObject)->Realize(slEngineObject,
                                                      SL_BOOLEAN_FALSE
            ));

            SL_ERROR_CHECK((*slEngineObject)->GetInterface(slEngineObject,
                                                           SL_IID_ENGINE,
                                                           (void *) &slEngine
            ));



            // create output mix and get its interfaces
            SL_ERROR_CHECK((*slEngine)->CreateOutputMix(slEngine,
                                                        &slOutputMixObject,
                                                        0,
                                                        nullptr,
                                                        nullptr
            ));

            SL_ERROR_CHECK((*slOutputMixObject)->Realize(slOutputMixObject,
                                                         SL_BOOLEAN_FALSE
            ));

            outputLocatorStruct.locatorType = SL_DATALOCATOR_OUTPUTMIX;
            outputLocatorStruct.outputMix = slOutputMixObject;

            outputSinkStruct.pLocator = (void *) &outputLocatorStruct;
            outputSinkStruct.pFormat = NULL;


            //activePlayerList.resize(1);
            //std::memset(&activePlayerList[0], 0, sizeof(SoundPlayer));

            //createSoundPlayer(&activePlayerList[0]);


        }
        catch (const std::exception &e)
        {
            // init failed
            Logger::getSingleton().write(e.what(), LL_ERROR);

            // clear all vars and release everything
            if (slOutputMixObject)
            {
                (*slOutputMixObject)->Destroy(slOutputMixObject);
                slOutputMixObject = nullptr;
            }

            if (slEngineObject)
            {
                (*slEngineObject)->Destroy(slEngineObject);
                slEngineObject = nullptr;
            }


            slEngine = nullptr;
        }

    };


    void SoundSystem::uninitialize()
    {
        if (!slEngineObject)
            return;

        (*slOutputMixObject)->Destroy(slOutputMixObject);
        (*slEngineObject)->Destroy(slEngineObject);

        slEngine = nullptr;
        slEngineObject = nullptr;
    };


    void SoundSystem::runTest(SoundPtr _sound)
    {
/*
        try
        {
            for (int i=0; i<5; ++i)
            {
                SL_ERROR_CHECK((*activePlayerList[0].slBufferQueue)->Enqueue(activePlayerList[0].slBufferQueue,
                                                                             _sound->getData().data(),
                                                                             _sound->getData().size()
                ));
            }

            SL_ERROR_CHECK((*activePlayerList[0].slPlay)->SetPlayState(activePlayerList[0].slPlay,
                                                                       SL_PLAYSTATE_PLAYING
            ));

        }
        catch (const std::exception &e)
        {
            Logger::getSingleton().write(e.what(), LL_ERROR);
        }
*/
    };


}
