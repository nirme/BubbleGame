#include "Debug.h"

void debugLog(std::string _t0, std::string _t1, std::string _t2, std::string _t3, std::string _t4, std::string _t5)
{
#ifndef NDEBUG
	core::Logger::getSingleton().write(_t0 + _t1 + _t2 + _t3 + _t4 + _t5, core::LL_DEBUG);
#endif
};

