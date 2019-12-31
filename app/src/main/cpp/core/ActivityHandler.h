#pragma once

#include <unistd.h>

class ActivityHandler
{
public:

    virtual void onActivate() = 0;
    virtual void onDeactivate() = 0;

    virtual void onStep() = 0;


    virtual void onInitWindow() {};
    virtual void onTermWindow() {};
    virtual void onGainedFocus() {};
    virtual void onLostFocus() {};
    virtual void onConfigChanged() {};

    virtual void onLowMemory() {};
    virtual void onStart() {};
    virtual void onResume(void* pData, size_t pSize) {};

    virtual void onSaveState(void** pData, size_t* pSize) {};
    virtual void onPause() {};
    virtual void onStop() {};
    virtual void onDestroy() {};

};

