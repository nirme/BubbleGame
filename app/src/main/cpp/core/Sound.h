#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaExtractor.h>

#include "Exceptions.h"
#include "Logger.h"

#include "Resource.h"
#include "ResourceManager.h"
#include "DataStream.h"


namespace core
{
    class SoundSystem;


    class Sound : public Resource
    {
    protected:
        typedef std::unique_ptr<AMediaExtractor,void(*)(AMediaExtractor*)> AMediaExtractorUPtr;
        typedef std::unique_ptr<AMediaCodec,void(*)(AMediaCodec*)> AMediaCodecUPtr;
        typedef std::unique_ptr<AMediaFormat,void(*)(AMediaFormat*)> AMediaFormatUPtr;

    public:

        class SoundBuffer
        {
        protected:
            constexpr int ExtractorRetryCount = 3;
            constexpr int MaxConcurrentBuffers = 3;

            AMediaExtractorUPtr mediaExtractor;
            AMediaCodecUPtr mediaCodec;

            bool extractorEOS, codecEOS;

            struct BufferDefinition
            {
                ssize_t id;
                uint8_t *data;
                int32_t dataSize;
            };
            typedef std::list<BufferDefinition> BufferList;
            BufferList buffers;



            void extractNextBuffer()
            {
                if (!extractorEOS)
                {
                    ssize_t bufferIndex(-1), sampleSize(0);
                    size_t bufferSize(0);
                    media_status_t mediaStatus = AMEDIA_OK;
                    uint8_t *bufferPtr(nullptr);

                    int retryCountLeft = ExtractorRetryCount;
                    do
                    {
                        --retryCountLeft;
                        bufferIndex = AMediaCodec_dequeueInputBuffer(mediaCodec.get(),
                                                                     DequeTimeout);
                        if (bufferIndex >= 0)
                        {
                            bufferPtr = AMediaCodec_getInputBuffer(mediaCodec.get(),
                                                                   bufferIndex,
                                                                   &bufferSize);

                            sampleSize = AMediaExtractor_readSampleData(mediaExtractor.get(),
                                                                        bufferPtr,
                                                                        bufferSize);

                            if (sampleSize < 0)
                            {
                                sampleSize = 0;
                                extractorEOS = true;
                            }

                            if ((mediaStatus = AMediaCodec_queueInputBuffer(mediaCodec.get(),
                                                                            bufferIndex,
                                                                            0,
                                                                            sampleSize,
                                                                            0, // sampleTime,
                                                                            extractorEOS ? AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM : 0))
                                != AMEDIA_OK)
                            {
                                throw std::runtime_error(std::string(
                                        "AMediaCodec_queueInputBuffer() failed with error code ") +
                                         std::to_string(mediaStatus));
                            }

                            AMediaExtractor_advance(mediaExtractor.get());
                        }
                    }
                    while (bufferIndex < 0 && retryCountLeft >= 0);
                }
            };

            bool pushNextBuffer()
            {
                if (!codecEOS)
                {
                    ssize_t bufferIndex(-1);
                    size_t bufferSize(0);
                    uint8_t *bufferPtr(nullptr);
                    AMediaCodecBufferInfo bufferInfo;

                    int retryCountLeft = ExtractorRetryCount;

                    do
                    {
                        --retryCountLeft;
                        bufferIndex = AMediaCodec_dequeueOutputBuffer(mediaCodec.get(), &bufferInfo, DequeTimeout);
                    }
                    while(retryCountLeft >= 0 && (
                            bufferIndex == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED ||
                            bufferIndex == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED ||
                            bufferIndex == AMEDIACODEC_INFO_TRY_AGAIN_LATER));

                    if (bufferIndex >= 0)
                    {
                        if (bufferInfo.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM)
                            codecEOS = true;

                        if (bufferInfo.size > 0)
                        {
                            bufferPtr = AMediaCodec_getOutputBuffer(mediaCodec.get(), bufferIndex, &bufferSize);
                            buffers.push_front({bufferIndex, bufferPtr, bufferInfo.size});
                            return true;
                        }
                    }
                }

                return false;
            };

        public:

            SoundBuffer(AMediaExtractorUPtr _mediaExtractor, AMediaCodecUPtr _mediaCodec) :
                mediaExtractor(_mediaExtractor.get(),
                    [](AMediaExtractor *_res) { if (_res) AMediaExtractor_delete(_res); }),
                mediaCodec(_mediaCodec.get(),
                    [](AMediaCodec *_res) { if (_res) AMediaCodec_delete(_res); })
            {
                _mediaExtractor.release();
                _mediaCodec.release();

                reset();
            };

            std::pair<void*,int32_t> getNextBuffer()
            {
                if (pushNextBuffer())
                {
                    std::pair<void*,int32_t> val = std::make_pair(buffers.front().data, buffers.front().dataSize);

                    if (buffers.size() >= MaxConcurrentBuffers)
                    {
                        releaseLastBuffer();
                    }

                    extractNextBuffer();

                }

                bufferIndex = AMediaCodec_dequeueOutputBuffer(mCodec.get(), &bufferInfo, DequeTimeout);


                BufferDefinition &buff = buffers.front();
                return std::make_pair(buff.data, buff.dataSize);

            };

            void releaseBuffer(void *_buffPtr)
            {
                for (BufferList::reverse_iterator it = buffers.rbegin(); it != buffers.rend(); ++it)
                {
                    if (_buffPtr == (*it).data)
                    {
                        AMediaCodec_releaseOutputBuffer(mediaCodec.get(), (*it).id, false);
                        buffers.erase(it);
                        return;
                    }
                }
            };

            void releaseLastBuffer()
            {
                AMediaCodec_releaseOutputBuffer(mediaCodec.get(), buffers.back().id, false);
                buffers.pop_back();
            };



            void reset()
            {
                while (buffers.size())
                {
                    AMediaCodec_releaseOutputBuffer(mediaCodec.get(), buffers.front().id, false);
                    buffers.pop_front();
                }

                AMediaExtractor_seekTo(mediaExtractor.get(), 0, AMEDIAEXTRACTOR_SEEK_NEXT_SYNC);
                AMediaCodec_flush(mediaCodec.get());

                // push one buffer ahed of time
                extractNextBuffer();
            };
        };


    protected:


        constexpr static unsigned int MaxRawBufferSize = (2 << 20); // 2Mb
        //constexpr static unsigned int MaxRawBufferSize = (2 << 30); // 2Mb
        constexpr static unsigned int DequeTimeout = 2000; // us
        //constexpr static unsigned int DequeTimeout = -1; // us

        inline unsigned int getRawSampleSize(int64_t _usDuration, int32_t _sampleRate, int32_t _bitRate, int32_t _channels)
        {
            int64_t size = _sampleRate * _bitRate * _channels;
            size /= 8; // bits to bytes
            size *= _usDuration;
            size = (size / 1000000) + (size % 1000000 ? 1 : 0);
            return (unsigned int) size;
        };

        SoundSystem *soundSystem;

        DataStreamPtr soundDataPtr;
        typedef std::vector<unsigned char> Data;
        Data soundData;
        int32_t bufferSampleRate;
        int32_t bufferBitRate;
        int32_t bufferChannels;

        AMediaExtractorUPtr mediaExtractor;
        AMediaCodecUPtr mediaCodec;

/*
        int8_t *resample(int8_t *_buff, int32_t _dataSize, int32_t _buffSize)
        {

        };
*/
    public:

        Sound(const std::string &_name, ResourceHandle _handle, const std::string &_group, SoundSystem *_soundSystem, ResourceManager *_manager = nullptr);
        virtual ~Sound();

        virtual void loadImp();
        virtual void unloadImp();

        virtual unsigned int sizeCalcImpl();

        const Data& getData();
    };

    typedef std::shared_ptr<Sound> SoundPtr;

}
