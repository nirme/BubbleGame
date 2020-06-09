#include "Sound.h"
#include "SoundSystem.h"

namespace core
{

    void Sound::free_media_extractor(AMediaExtractor *_ptr)
    {
        if (_ptr)
            AMediaExtractor_delete(_ptr);
    };


    void Sound::free_media_codec(AMediaCodec *_ptr)
    {
        if (_ptr)
            AMediaCodec_delete(_ptr);
    };


    void Sound::free_media_format(AMediaFormat *_ptr)
    {
        if (_ptr)
            AMediaFormat_delete(_ptr);
    };


    Sound::StaticSoundBufferIterator::StaticSoundBufferIterator(VectorPtr _soundData) :
            SoundBufferIterator(),
            soundData(_soundData),
            isBegin(true)
    {};


    Sound::StaticSoundBufferIterator::~StaticSoundBufferIterator()
    {};


	Sound::SoundBufferIterator::SampleBuffer Sound::StaticSoundBufferIterator::operator*()
    {
        return isBegin ?
			   SoundBufferIterator::SampleBuffer({soundData.get()->data(), (int32_t) soundData->size()}) :
			   SoundBufferIterator::SampleBuffer({nullptr, 0});
    };


    bool Sound::StaticSoundBufferIterator::next()
    {
        isBegin = false;
        return isBegin;
    };


    void Sound::StaticSoundBufferIterator::reset()
    {
        isBegin = true;
    };


	bool Sound::StaticSoundBufferIterator::isEnd()
	{
		return isBegin;
	};


    Sound::SoundBufferIterator *Sound::StaticSoundBufferIterator::clone()
    {
        return new StaticSoundBufferIterator(soundData);
    };


    bool Sound::EncodedSoundBufferIterator::extract()
    {
        if (!extractorEOF && coderBuffersUsed < MaxConcurrentBuffers)
        {
            ssize_t bufferIndex = AMediaCodec_dequeueInputBuffer(mediaCodec.get(), DequeTimeout);
            if (bufferIndex >= 0)
            {
                size_t bufferSize(0);
                uint8_t *bufferPtr = AMediaCodec_getInputBuffer(mediaCodec.get(), bufferIndex, &bufferSize);
                ssize_t sampleSize = AMediaExtractor_readSampleData(mediaExtractor.get(), bufferPtr, bufferSize);

                if (sampleSize < 0)
                {
                    sampleSize = 0;
                    extractorEOF = true;
                }

                media_status_t mediaStatus = AMEDIA_OK;
                if ((mediaStatus = AMediaCodec_queueInputBuffer(
                        mediaCodec.get(),
                        bufferIndex,
                        0,
                        sampleSize,
                        0, // sampleTime,
                        extractorEOF ? AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM : 0))
                    != AMEDIA_OK)
                    throw std::runtime_error(std::string("AMediaCodec_queueInputBuffer() failed with error code ") + std::to_string(mediaStatus));

                ++coderBuffersUsed;

                AMediaExtractor_advance(mediaExtractor.get());
            }

            return true;
        }
        return false;
    };


    void Sound::EncodedSoundBufferIterator::popLastBuffer()
    {
        if (buffers.size())
        {
            media_status_t mediaStatus = AMEDIA_OK;
            if ((mediaStatus = AMediaCodec_releaseOutputBuffer(
                    mediaCodec.get(),
                    buffers.back(),
                    false))
                != AMEDIA_OK)
                throw std::runtime_error(std::string("AMediaCodec_releaseOutputBuffer() failed with error code ") + std::to_string(mediaStatus));

            buffers.pop_back();
        }
    };


	Sound::EncodedSoundBufferIterator::DECODING_RESULT Sound::EncodedSoundBufferIterator::decode()
    {
        if (!decoderEOF && coderBuffersUsed)
        {
			while (buffers.size() >= MaxConcurrentBuffers)
				popLastBuffer();

            AMediaCodecBufferInfo bufferInfo;
            ssize_t bufferIndex(-1);

			do
            {
                bufferIndex = AMediaCodec_dequeueOutputBuffer(mediaCodec.get(), &bufferInfo, DequeTimeout);
                if (bufferIndex >= 0)
                {
					if (bufferInfo.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM)
						decoderEOF = true;

                	// release and loop again if no data
					if (!bufferInfo.size)
					{
						AMediaCodec_releaseOutputBuffer(mediaCodec.get(), bufferIndex, false);

						--coderBuffersUsed;

						if (decoderEOF)
						{
							dataPtr = nullptr;
							dataSize = 0;
							return DR_EOF;
						}
						continue;
					}

					buffers.push_front(bufferIndex);
					dataPtr = AMediaCodec_getOutputBuffer(mediaCodec.get(), bufferIndex, nullptr );
					dataSize = bufferInfo.size;
					--coderBuffersUsed;
                    return DR_DECODED;
                }
				else if (bufferIndex == AMEDIACODEC_INFO_TRY_AGAIN_LATER)
					return DR_TRY_AGAIN_LATER;
			}
            while (true);
		}
        else if (!decoderEOF)
        	return DR_TRY_AGAIN_LATER;

        return DR_EOF;
    };


    Sound::EncodedSoundBufferIterator::EncodedSoundBufferIterator(DataStreamPtr _soundDataPtr, AMediaExtractorUPtr &_mediaExtractor, AMediaCodecUPtr &_mediaCodec) :
            SoundBufferIterator(),
            soundDataPtr(_soundDataPtr),
            mediaExtractor(std::move(_mediaExtractor)),
            mediaCodec(std::move(_mediaCodec)),
            dataPtr(nullptr),
            dataSize(0),
            extractorEOF(false),
            decoderEOF(false),
            coderBuffersUsed(0)
    {
    	assert(soundDataPtr && "No data pointer provided");
		assert(mediaExtractor && "No media extractor provided");
		assert(mediaCodec && "No media codec provided");

		reset();
	};


    Sound::EncodedSoundBufferIterator::~EncodedSoundBufferIterator()
    {};


	Sound::SoundBufferIterator::SampleBuffer Sound::EncodedSoundBufferIterator::operator*()
    {
        return SoundBufferIterator::SampleBuffer({dataPtr, dataSize});
    };


    bool Sound::EncodedSoundBufferIterator::next()
    {
        if (decoderEOF)
            return false;

        DECODING_RESULT res;
        while ((res = decode()) == DR_TRY_AGAIN_LATER)
        {
            extract();
        }

        if (res == DR_DECODED)
		{
			extract();
			return true;
		}

        return false;
    };


    void Sound::EncodedSoundBufferIterator::reset()
    {
		media_status_t mediaStatus;

		if ((mediaStatus = AMediaExtractor_seekTo(mediaExtractor.get(), 0, AMEDIAEXTRACTOR_SEEK_NEXT_SYNC)) != AMEDIA_OK)
			throw std::runtime_error(std::string("AMediaExtractor_seekTo() failed with error code ") + std::to_string(mediaStatus));

        while (buffers.size())
        {
            AMediaCodec_releaseOutputBuffer(mediaCodec.get(), buffers.front(), false);
            buffers.pop_front();
        }

		if ((mediaStatus = AMediaCodec_flush(mediaCodec.get())) != AMEDIA_OK)
			throw std::runtime_error(std::string("AMediaCodec_flush() failed with error code ") + std::to_string(mediaStatus));

		extractorEOF = false;
		decoderEOF = false;


        // fill decoder with data, func returns false if extractor is done or buffers filled are equal to MaxConcurrentBuffers
		DECODING_RESULT res;
		while ((res = decode()) == DR_TRY_AGAIN_LATER)
		{
			extract();
		}

		if (res == DR_DECODED)
		{
			extract();
		}
    };


	bool Sound::EncodedSoundBufferIterator::isEnd()
	{
		return !decoderEOF;
	};


    Sound::SoundBufferIterator *Sound::EncodedSoundBufferIterator::clone()
    {
        AMediaExtractorUPtr newExtractor(AMediaExtractor_new(), free_media_extractor);
        if (!newExtractor)
            throw std::runtime_error("AMediaExtractor_new() failed");

        media_status_t mediaStatus = AMEDIA_OK;
        FileDescriptorData fileDesc = soundDataPtr->openFileDescriptor();

        if ((mediaStatus = AMediaExtractor_setDataSourceFd(newExtractor.get(), fileDesc.file, fileDesc.startOffset, fileDesc.lenght)) != AMEDIA_OK)
            throw std::runtime_error(std::string("AMediaExtractor_setDataSourceFd() failed with error code ") + std::to_string(mediaStatus));

        if ((mediaStatus = AMediaExtractor_selectTrack(newExtractor.get(), 0)) != AMEDIA_OK)
            throw std::runtime_error(std::string("AMediaExtractor_selectTrack() failed with error code ") + std::to_string(mediaStatus));


        AMediaFormatUPtr mediaFormat(AMediaExtractor_getTrackFormat(newExtractor.get(), AMediaExtractor_getSampleTrackIndex(mediaExtractor.get())),
                                     free_media_format);

        const char *cstrMimeType;
        if (!AMediaFormat_getString(mediaFormat.get(), AMEDIAFORMAT_KEY_MIME, &cstrMimeType))
            throw std::runtime_error("AMediaFormat_getString() failed to provide mime type");

        AMediaCodecUPtr newCodec(AMediaCodec_createDecoderByType(cstrMimeType), free_media_codec);
        if (!newCodec)
            throw std::runtime_error("AMediaCodec_createDecoderByType() failed");

		if ((mediaStatus = AMediaCodec_configure(newCodec.get(), mediaFormat.get(), nullptr, nullptr, 0)) != AMEDIA_OK)
			throw std::runtime_error(std::string("AMediaCodec_configure() failed with error code ") + std::to_string(mediaStatus));

		if ((mediaStatus = AMediaCodec_start(newCodec.get())) != AMEDIA_OK)
			throw std::runtime_error(std::string("AMediaCodec_start() failed with error code ") + std::to_string(mediaStatus));

		return new EncodedSoundBufferIterator(soundDataPtr, newExtractor, newCodec);
    };


	Sound::Sound(const std::string &_name, ResourceHandle _handle, const std::string &_group, SoundSystem *_soundSystem, ResourceManager *_manager) :
		Resource(_name, _handle, _group, _manager),
		soundSystem(_soundSystem),
		soundBuffer(nullptr),
		bufferSampleRate(0),
		bufferBitRate(0),
		bufferChannels(0)
	{};


    Sound::~Sound()
    {};


    void Sound::loadImp()
    {
        assert(soundSystem && "No sound system to create sound resource" );

Logger::getSingleton().write(std::string("loading sound '") + this->name + "'");

        SoundBufferIteratorUPtr openSoundBuffer(nullptr);
        int32_t openBufferSampleRate(0), openBufferBitRate(0), openBufferChannels(0);

        DataStreamPtr data = manager->openResource(getName());
        FileDescriptorData fileDesc = data->openFileDescriptor();

        try
        {
            media_status_t mediaStatus = AMEDIA_OK;
            AMediaExtractorUPtr mExtractor(AMediaExtractor_new(), free_media_extractor);

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


            AMediaFormatUPtr mediaFormat( AMediaExtractor_getTrackFormat(mExtractor.get(), 0), free_media_format);
            if (!mediaFormat)
                throw std::runtime_error("AMediaExtractor_getTrackFormat() failed");


            // get mime for decoder
            const char *cstrMimeType;
            if (!AMediaFormat_getString(mediaFormat.get(), AMEDIAFORMAT_KEY_MIME, &cstrMimeType))
                throw std::runtime_error("AMediaFormat_getString() failed to provide mime type");


			std::string fullFormat = AMediaFormat_toString(mediaFormat.get());
			Logger::getSingleton().write(name + ": " + fullFormat);


			// get basic data
            int64_t usDuration(0); // us
            if (!AMediaFormat_getInt64(mediaFormat.get(), AMEDIAFORMAT_KEY_DURATION, &usDuration))
                throw std::runtime_error("AMediaFormat_getInt64() failed to provide track duration");

			// create codec with format info from extractor, this takes care of special config too
            AMediaCodecUPtr mCodec(AMediaCodec_createDecoderByType(cstrMimeType), free_media_codec);

            if ((mediaStatus = AMediaCodec_configure(mCodec.get(), mediaFormat.get(), nullptr, nullptr, 0)) != AMEDIA_OK)
                throw std::runtime_error(std::string("AMediaCodec_configure() failed with error code ") + std::to_string(mediaStatus));


			// get output format
			{
				if ((mediaStatus = AMediaCodec_start(mCodec.get())) != AMEDIA_OK)
					throw std::runtime_error(std::string("AMediaCodec_start() failed with error code ") + std::to_string(mediaStatus));

				// push in extracted buffers to fill decoder and get confirmation on output format
				int errCount(0), repeatCount(0);
				ssize_t bufferIndex(-1);
				size_t bufferSize(0);
				uint8_t *bufferPtr(nullptr);
				ssize_t sampleSize(0);
				AMediaCodecBufferInfo bufferInfo;
				bool outputReadyFlag(false);

				while (!outputReadyFlag)
				{
					errCount = 0;
					while ((bufferIndex = AMediaCodec_dequeueInputBuffer(mCodec.get(), DequeTimeout)) < 0)
					{
						if (errCount++ > 3)
							throw std::runtime_error("AMediaCodec_dequeueInputBuffer() failed 3 time in a row");
					}

					bufferPtr = AMediaCodec_getInputBuffer(mCodec.get(), bufferIndex, &bufferSize);
					sampleSize = AMediaExtractor_readSampleData(mExtractor.get(), bufferPtr, bufferSize);

					if ((mediaStatus = AMediaCodec_queueInputBuffer(
							mCodec.get(),
							bufferIndex,
							0,
							sampleSize >= 0 ? sampleSize : 0,
							0, // sampleTime,
							sampleSize < 0 ? AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM : 0))
						!= AMEDIA_OK)
						throw std::runtime_error(std::string("AMediaCodec_queueInputBuffer() failed with error code ") + std::to_string(mediaStatus));

					AMediaExtractor_advance(mExtractor.get());
					bufferIndex = -1;

					// wait for format changed message
					errCount = 0;

					do
					{
						bufferIndex = AMediaCodec_dequeueOutputBuffer(mCodec.get(), &bufferInfo, DequeTimeout);

						// output format should be ready here
						if (bufferIndex >= 0 || bufferIndex == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED)
						{
							if (bufferIndex >= 0)
								AMediaCodec_releaseOutputBuffer(mCodec.get(), bufferIndex, false);

							outputReadyFlag = true;
						}
						// not enough data provided
						else if (bufferIndex == AMEDIACODEC_INFO_TRY_AGAIN_LATER)
							break;

						// doesn't care, get next buffer
						// AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED

					}
					while (!outputReadyFlag);

					if ((++repeatCount) >= 5)
						throw std::runtime_error("AMediaCodec_dequeueOutputBuffer() failed 3 time in a row");
				}


				AMediaFormatUPtr outputFormat( AMediaCodec_getOutputFormat(mCodec.get()), free_media_format);
				if (!outputFormat)
					throw std::runtime_error("AMediaCodec_getOutputFormat() failed");

				// get the data
				if (!AMediaFormat_getInt32(outputFormat.get(), AMEDIAFORMAT_KEY_SAMPLE_RATE, &openBufferSampleRate))
					throw std::runtime_error("AMediaFormat_getInt32() failed to provide output sample rate");

				if (!AMediaFormat_getInt32(outputFormat.get(), "bit-width", &openBufferBitRate))
					throw std::runtime_error("AMediaFormat_getInt32() failed to provide output bit rate");

				if (!AMediaFormat_getInt32(outputFormat.get(), AMEDIAFORMAT_KEY_CHANNEL_COUNT, &openBufferChannels))
					throw std::runtime_error("AMediaFormat_getInt32() failed to provide output channel count");

				outputFormat.reset();

				// reset extractor
				AMediaExtractor_seekTo(mExtractor.get(), 0, AMEDIAEXTRACTOR_SEEK_NEXT_SYNC);
				AMediaCodec_flush(mCodec.get());
 			}

			assert(openBufferSampleRate == soundSystem->getSystemSampleRate() && "Sound sample rate not matching system config");
			assert(openBufferBitRate == soundSystem->getSystemBitRate() && "Sound bit rate not matching system config");
			assert(openBufferChannels == soundSystem->getSystemChannels() && "Sound channel count not matching system config");


			openSoundBuffer = SoundBufferIteratorUPtr(new EncodedSoundBufferIterator(data, mExtractor, mCodec));

            if (usDuration <= MaxStaticBufferLength)
            {
                std::shared_ptr<std::vector<uint8_t>> dataVector(new std::vector<uint8_t>());
				SoundBufferIterator::SampleBuffer partialBuffer;

                do
                {
                    partialBuffer = **openSoundBuffer;

                    unsigned int pos = dataVector->size();
					dataVector->resize(pos + partialBuffer.size);
                    memcpy(&(dataVector->data()[pos]), partialBuffer.data, partialBuffer.size);
                }
                while (openSoundBuffer->next());

				openSoundBuffer = SoundBufferIteratorUPtr(new StaticSoundBufferIterator(dataVector));
            }
        }
        catch (const std::exception &e)
        {
            Logger::getSingleton().write(e.what(), LL_ERROR);
            throw;
        }

        soundBuffer.swap(openSoundBuffer);
        bufferSampleRate = openBufferSampleRate;
        bufferBitRate = openBufferBitRate;
        bufferChannels = openBufferChannels;
    };


    void Sound::unloadImp()
    {
        soundBuffer.reset();
        bufferSampleRate = 0;
        bufferBitRate = 0;
        bufferChannels = 0;
    };


    unsigned int Sound::sizeCalcImpl()
    {
        unsigned int s = sizeof(SoundSystem*);
        s += sizeof(soundBuffer);
        s += sizeof(bufferSampleRate);
        s += sizeof(bufferBitRate);
        s += sizeof(bufferChannels);

        return s;
    };


    Sound::SoundBufferIteratorUPtr Sound::getDataIterator()
    {
        return SoundBufferIteratorUPtr(soundBuffer->clone());
    };

}
