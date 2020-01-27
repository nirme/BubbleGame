#include "Sound.h"

namespace core
{

    Sound::Sound(const std::string &_name, ResourceHandle _handle, const std::string &_group, SoundSystem *_soundSystem, ResourceManager *_manager) :
            Resource(_name, _handle, _group, _manager),
            soundSystem(_soundSystem),
            mediaExtractor(nullptr, [](AMediaExtractor *_res) { if (_res) AMediaExtractor_delete(_res); }),
            mediaCodec(nullptr, [](AMediaCodec *_res) { if (_res) AMediaCodec_delete(_res); })

{};


    Sound::~Sound()
    {};


    void Sound::loadImp()
    {
        assert(soundSystem && "No sound system to create sound resource" );

        DataStreamPtr data = manager->openResource(getName());
        FileDescriptorData fileDesc = data->openFileDescriptor();

        try
        {
            media_status_t mediaStatus = AMEDIA_OK;

            AMediaExtractorUPtr mExtractor(AMediaExtractor_new(),
                [](AMediaExtractor *_res) { if (_res) AMediaExtractor_delete(_res); } );

            if (!mExtractor)
                throw std::runtime_error("AMediaExtractor_new() failed to create new Media Extractor");

            // setup data source to the file
            if ((mediaStatus = AMediaExtractor_setDataSourceFd(
                        mExtractor.get(),
                        fileDesc.file,
                        fileDesc.startOffset,
                        fileDesc.lenght))
                    != AMEDIA_OK)
                throw std::runtime_error(std::string("AMediaExtractor_setDataSourceFd() failed with error code ") + std::to_string(mediaStatus));


            // make sure there is only one track
            if (AMediaExtractor_getTrackCount(mExtractor.get()) != 1)
                throw std::runtime_error("Only one track supported in data files");

            if ((mediaStatus = AMediaExtractor_selectTrack(mExtractor.get(), 0)) != AMEDIA_OK)
                throw std::runtime_error(std::string("AMediaExtractor_selectTrack() failed with error code ") + std::to_string(mediaStatus));


            AMediaFormatUPtr mediaFormat( AMediaExtractor_getTrackFormat(mExtractor.get(), 0),
                [](AMediaFormat *_res) { if (_res) AMediaFormat_delete(_res); } );
            if (!mediaFormat)
                throw std::runtime_error("AMediaExtractor_getTrackFormat() failed");

            // get mime for decoder
            const char *cstrMimeType;
            if (!AMediaFormat_getString(mediaFormat.get(), AMEDIAFORMAT_KEY_MIME, &cstrMimeType))
                throw std::runtime_error("AMediaFormat_getString() failed to provide mime type");

            int64_t usDuration(0); // us
            if (!AMediaFormat_getInt64(mediaFormat.get(), AMEDIAFORMAT_KEY_DURATION, &usDuration))
                throw std::runtime_error("AMediaFormat_getInt64() failed to provide track duration");

            Logger::getSingleton().write(
                    std::string("Input format: ") +
                    AMediaFormat_toString(mediaFormat.get()));

            // create codec with format info from extractor
            AMediaCodecUPtr mCodec(AMediaCodec_createDecoderByType(cstrMimeType),
                [](AMediaCodec *_res) { if (_res) AMediaCodec_delete(_res); } );

            if ((mediaStatus = AMediaCodec_configure(mCodec.get(), mediaFormat.get(), nullptr, nullptr, 0))
                    != AMEDIA_OK)
                throw std::runtime_error(std::string("AMediaCodec_configure() failed with error code ") + std::to_string(mediaStatus));


            bool inputEOF(false), outputEOF(false);
            ssize_t bufferIndex(0), sampleSize(0);
            size_t bufferSize(0), localBufferPos(0);
            int64_t sampleTime(0);
            uint8_t *bufferPtr(nullptr);
            AMediaCodecBufferInfo bufferInfo;

            if ((mediaStatus = AMediaCodec_start(mCodec.get())) != AMEDIA_OK)
                throw std::runtime_error(std::string("AMediaCodec_start() failed with error code ") + std::to_string(mediaStatus));

            while (!inputEOF || !outputEOF)
            {

                if (!inputEOF)
                {
                    bufferIndex = AMediaCodec_dequeueInputBuffer(mCodec.get(), DequeTimeout);
                    if (bufferIndex >= 0)
                    {
                        bufferPtr = AMediaCodec_getInputBuffer(mCodec.get(), bufferIndex, &bufferSize);
                        sampleSize = AMediaExtractor_readSampleData(mExtractor.get(), bufferPtr, bufferSize);
                        //sampleTime = AMediaExtractor_getSampleTime(mExtractor.get());

                        if (sampleSize < 0)
                        {
                            sampleSize = 0;
                            inputEOF = true;
                        }

                        if ((mediaStatus = AMediaCodec_queueInputBuffer(
                                    mCodec.get(),
                                    bufferIndex,
                                    0,
                                    sampleSize,
                                    0, // sampleTime,
                                    inputEOF ? AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM : 0))
                                != AMEDIA_OK)
                            throw std::runtime_error(std::string("AMediaCodec_queueInputBuffer() failed with error code ") + std::to_string(mediaStatus));

                        AMediaExtractor_advance(mExtractor.get());
                    }
                }


                if (!outputEOF)
                {
                    bufferIndex = AMediaCodec_dequeueOutputBuffer(mCodec.get(), &bufferInfo, DequeTimeout);

                    if (bufferIndex >= 0)
                    {
                        if (bufferInfo.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM)
                            outputEOF = true;

                        if (bufferInfo.size > 0)
                        {
                            bufferPtr = AMediaCodec_getOutputBuffer(mCodec.get(), bufferIndex, &bufferSize);

                            if (soundData.size() < localBufferPos + bufferInfo.size)
                                soundData.resize(localBufferPos + bufferInfo.size);

                            memcpy(&soundData[localBufferPos], bufferPtr, bufferInfo.size);
                            localBufferPos += bufferInfo.size;
                        }

                        if ((mediaStatus = AMediaCodec_releaseOutputBuffer(
                                    mCodec.get(),
                                    bufferIndex,
                                    false))
                                != AMEDIA_OK)
                            throw std::runtime_error(std::string("AMediaCodec_releaseOutputBuffer() failed with error code ") + std::to_string(mediaStatus));
                    }
                    else
                    {
                        // get output format
                        if (bufferIndex == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED)
                        {
                            AMediaFormatUPtr outputFormat(AMediaCodec_getOutputFormat(mCodec.get()),
                                    [](AMediaFormat *_res) { if (_res) AMediaFormat_delete(_res); });

                             if (!AMediaFormat_getInt32(outputFormat.get(), AMEDIAFORMAT_KEY_SAMPLE_RATE, &bufferSampleRate))
                                throw std::runtime_error("AMediaFormat_getInt32() failed to provide output sample rate");

                            if (!AMediaFormat_getInt32(outputFormat.get(), "bit-width", &bufferBitRate))
                                throw std::runtime_error("AMediaFormat_getInt32() failed to provide output bit rate");

                            if (!AMediaFormat_getInt32(outputFormat.get(), AMEDIAFORMAT_KEY_CHANNEL_COUNT, &bufferChannels))
                                throw std::runtime_error("AMediaFormat_getInt32() failed to provide output channel count");

                            unsigned int rawTrackSize = getRawSampleSize(usDuration, bufferSampleRate, bufferBitRate, bufferChannels);
                            if (rawTrackSize > MaxRawBufferSize)
                            {
                                soundData.resize(0);
                                soundData.shrink_to_fit();

                                soundDataPtr = DataStreamPtr(nullptr);
                                soundDataPtr.swap(data);

                                mediaExtractor = AMediaExtractorUPtr(nullptr, [](AMediaExtractor *_res) { if (_res) AMediaExtractor_delete(_res); });
                                mediaExtractor.swap(mExtractor);
                                mediaCodec = AMediaCodecUPtr(nullptr, [](AMediaCodec *_res) { if (_res) AMediaCodec_delete(_res); });
                                mediaCodec.swap(mCodec);

                                AMediaExtractor_seekTo(mediaExtractor.get(), 0, AMEDIAEXTRACTOR_SEEK_NEXT_SYNC);
                                //AMediaCodec_flush(mediaCodec.get());
                                AMediaCodec_stop(mediaCodec.get());

                                break;
                            }
                        }
                        else if (bufferIndex != AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED && bufferIndex != AMEDIACODEC_INFO_TRY_AGAIN_LATER)
                            throw std::runtime_error(std::string("AMediaCodec_dequeueOutputBuffer() failed with code ") + std::to_string(bufferIndex));
                    }
                }
            }

Logger::getSingleton().write(std::string("u: ") + std::to_string(u));
            if (mCodec)
            {
                if ((mediaStatus = AMediaCodec_stop(mCodec.get())) != AMEDIA_OK)
                    throw std::runtime_error(std::string("AMediaCodec_stop() failed with error code ") + std::to_string(mediaStatus));
            }
        }
        catch (const std::exception &e)
        {
            Logger::getSingleton().write(e.what(), LL_ERROR);
            throw;
        }

    };

    void Sound::unloadImp()
    {
        Data empty(0);
        soundData.swap(empty);
    };

    unsigned int Sound::sizeCalcImpl()
    {
        unsigned int s = sizeof(SoundSystem*);
        s += sizeof(Data);
        s += sizeof(soundData.capacity());

        return s;
    };

    const Sound::Data& Sound::getData()
    {
        return soundData;
    };


}
