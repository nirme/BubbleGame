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

        static void free_media_extractor(AMediaExtractor *_ptr);
        static void free_media_codec(AMediaCodec *_ptr);
        static void free_media_format(AMediaFormat *_ptr);

        typedef std::unique_ptr<AMediaExtractor,void(*)(AMediaExtractor*)> AMediaExtractorUPtr;
        typedef std::unique_ptr<AMediaCodec,void(*)(AMediaCodec*)> AMediaCodecUPtr;
        typedef std::unique_ptr<AMediaFormat,void(*)(AMediaFormat*)> AMediaFormatUPtr;


    public:
        class SoundBufferIterator
        {
        public:
        	struct SampleBuffer
			{
				uint8_t *data;
				int32_t size;
			};

            virtual ~SoundBufferIterator() {};
            virtual SampleBuffer operator*() = 0; // data, size
            virtual bool next() = 0; // false on end
            virtual void reset() = 0;
            virtual bool isEnd() = 0;
            virtual SoundBufferIterator *clone() = 0;
        };
        typedef std::unique_ptr<SoundBufferIterator> SoundBufferIteratorUPtr;

        class StaticSoundBufferIterator : public SoundBufferIterator
        {
        protected:
            typedef std::shared_ptr<std::vector<uint8_t>> VectorPtr;
            VectorPtr soundData;
            bool isBegin;

        public:

            StaticSoundBufferIterator(VectorPtr _soundData);
            ~StaticSoundBufferIterator();

			SoundBufferIterator::SampleBuffer operator*();
            bool next();
            void reset();
			bool isEnd();
            SoundBufferIterator *clone();
        };

        class EncodedSoundBufferIterator : public SoundBufferIterator
        {
        protected:
            static constexpr int MaxConcurrentBuffers = 3;

			DataStreamPtr soundDataPtr;
            AMediaExtractorUPtr mediaExtractor;
            AMediaCodecUPtr mediaCodec;

            typedef std::list<ssize_t> BufferedIdsList;
            BufferedIdsList buffers;

            uint8_t *dataPtr;
			int32_t dataSize;

			bool extractorEOF;
			bool decoderEOF;

			unsigned short coderBuffersUsed;


			enum DECODING_RESULT
			{
				DR_DECODED = 0x00,
				DR_EOF = 0x01,
				DR_TRY_AGAIN_LATER = 0x02,
			};

            bool extract();
            void popLastBuffer();
			DECODING_RESULT decode();

        public:
			EncodedSoundBufferIterator(DataStreamPtr _soundDataPtr, AMediaExtractorUPtr &_mediaExtractor, AMediaCodecUPtr &_mediaCodec);
			~EncodedSoundBufferIterator();

			SoundBufferIterator::SampleBuffer operator*();
            bool next();
            void reset();
			bool isEnd();
            SoundBufferIterator *clone();
        };

    protected:


		constexpr static unsigned int MaxStaticBufferLength = (8 * 1000000); // 8s
        constexpr static unsigned int DequeTimeout = 2000; // us

        inline unsigned int getRawSampleSize(int64_t _usDuration, int32_t _sampleRate, int32_t _bitRate, int32_t _channels)
        {
            int64_t size = _sampleRate * _bitRate * _channels;
            size /= 8; // bits to bytes
            size *= _usDuration;
            size = (size / 1000000) + (size % 1000000 ? 1 : 0);
            return (unsigned int) size;
        };

        SoundSystem *soundSystem;

		SoundBufferIteratorUPtr soundBuffer;

        int32_t bufferSampleRate;
        int32_t bufferBitRate;
        int32_t bufferChannels;


    public:

        Sound(const std::string &_name, ResourceHandle _handle, const std::string &_group, SoundSystem *_soundSystem, ResourceManager *_manager = nullptr);
        virtual ~Sound();

        virtual void loadImp();
        virtual void unloadImp();

        virtual unsigned int sizeCalcImpl();

        SoundBufferIteratorUPtr getDataIterator();
    };

    typedef std::shared_ptr<Sound> SoundPtr;

}
