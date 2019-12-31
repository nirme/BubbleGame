#pragma once

#include <memory>
#include <chrono>


namespace core
{


    class Timer
    {
    protected:
        typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timepoint;

        Timepoint startTime;

        Timepoint lastTime;
        Timepoint currentTime;

        unsigned long updateCount;


    public:

        Timer()
        {};

        void reset()
        {
            startTime = std::chrono::high_resolution_clock::now();
            lastTime = startTime;
            currentTime = startTime;
            updateCount = 0;
        };


        void update()
        {
            lastTime = currentTime;
            currentTime = std::chrono::high_resolution_clock::now();
            ++updateCount;
        };


        template <class T>
        T getTotalTimeElapsed()
        {
            return std::chrono::duration<T>(currentTime - startTime).count();
        };

        template <class T>
        T getLastUpdateTime()
        {
            return std::chrono::duration<T>(currentTime - lastTime).count();
        };

        unsigned long getUpdateCount()
        {
            return updateCount;
        };

    };

    typedef std::shared_ptr<Timer> TimerPtr;

}
