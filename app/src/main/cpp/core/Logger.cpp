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
		logLevel(LL_ALL)
	{};

	Logger::~Logger()
	{};


	Logger& Logger::write(const char* _text, LOG_LEVEL _logLevel)
	{
		if (_logLevel <= logLevel)
		{
			//log
			android_LogPriority priority;

			switch (_logLevel)
			{
				case LL_INFO:
					priority = ANDROID_LOG_INFO;
					break;

				case LL_WARNING:
					priority = ANDROID_LOG_WARN;
					break;

				case LL_ERROR:
					priority = ANDROID_LOG_ERROR;
					break;

				case LL_CRITICAL:
					priority = ANDROID_LOG_FATAL;
					break;

				case LL_DEBUG:
				default:
				{
#ifdef NDEBUG
					return *this;
#else
					priority = ANDROID_LOG_DEBUG;
					break;
#endif
				}
			};

			__android_log_write(priority, "BubbleGame", _text);
		}

		return *this;
	};

	Logger& Logger::write(std::string _text, LOG_LEVEL _logLevel)
	{
		return write(_text.c_str(),_logLevel);
	};


	void Logger::debug(const char* _text)
	{
#ifndef NDEBUG
		write(_text, LL_DEBUG);
#endif
	};

	void Logger::debug(std::string _text)
	{
#ifndef NDEBUG
		write(_text, LL_DEBUG);
#endif
	};

}