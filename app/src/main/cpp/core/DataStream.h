#pragma once

#include <memory>


namespace core
{
	typedef int data_size;

	struct FileDescriptorData
	{
		int file;
		long startOffset;
		long lenght;
	};

	class DataStream
	{
	public:

		virtual ~DataStream() {};

		virtual data_size getSize() = 0;
		virtual data_size seekPosition(data_size _pos) = 0;
		virtual data_size getPosition() = 0;
		virtual data_size readData(void * _buf, data_size _length = 0) = 0;


		virtual data_size skip(data_size _length);
		virtual data_size getRemainingSize();

		virtual FileDescriptorData openFileDescriptor() = 0;
	};


	typedef std::shared_ptr<DataStream> DataStreamPtr;

}