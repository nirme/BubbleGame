#include <android/log.h>
#include "Logger.h"



template<>
core::Logger* Singleton<core::Logger>::impl = nullptr;


namespace core
{

	Logger& Logger::getSingleton()
	{
		return *impl;
	};

	Logger* Logger::getSingletonPtr()
	{
		return impl;
	};


	Logger::Logger() :
		logLevel(LL_INFO)
	{};

	Logger::~Logger()
	{};


	Logger& Logger::write(const char* _text, LOG_LEVEL _logLevel)
	{
		if (_logLevel <= logLevel)
		{
			//log

			__android_log_write(ANDROID_LOG_DEBUG, "debug", _text);
		}

		return *this;
	};

	Logger& Logger::write(std::string _text, LOG_LEVEL _logLevel)
	{
		return write(_text.c_str(),_logLevel);
	};

}